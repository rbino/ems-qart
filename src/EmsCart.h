#ifndef _EMS_CART_H
#define _EMS_CART_H

#include <QObject>

#include <libusb.h>

namespace EmsConstants {
    const int UsbVID = 0x4670;
    const int UsbPID = 0x9394;
    const int ReadBlockSize = 4096;
    const int WriteBlockSize = 32;
    const int SendEndpoint = 2 | LIBUSB_ENDPOINT_OUT;
    const int ReceiveEndpoint = 1 | LIBUSB_ENDPOINT_IN;
    const int BankSize = 0x400000;
    const int SRAMSize = 0x020000;
    const uint8_t WriteSRAMCommand = 0x4d;
    const uint8_t ReadSRAMCommand = 0x6d;
    const uint8_t WriteROMCommand = 0x57;
    const uint8_t ReadROMCommand = 0xff;
}

class RomInfo;

class EmsCart : public QObject
{
    Q_OBJECT

    Q_ENUMS(Memory Bank)

    public:
        enum Memory {
            ROM = 1,
            SRAM = 2
        };

        enum Bank {
            InvalidBank = 0,
            BankOne = 1,
            BankTwo = 2
        };

        virtual ~EmsCart();

        static EmsCart *instance();

        void findDevice();
        QByteArray read(Memory from, uint32_t offset, uint32_t count);
        bool write(Memory to, QByteArray data, uint32_t offset, uint32_t count);
        void updateInfo();

        void readCart(const QUrl &outFileUrl, int memory, int bank = -1, int romIndex = -1);
        void writeCart(const QUrl &inFileUrl, int memory, int bank = -1, int offset = -1);

        bool ready() const;
        bool busy() const;
        double progress() const;

        QList<RomInfo *> bankOne() const;
        QList<RomInfo *> bankTwo() const;

    signals:
        void readyChanged(bool ready);
        void error(QString message);
        void transferCompleted();
        void progressChanged(double progress);

        void busyChanged(bool busy);

        void bankOneChanged(const QList<RomInfo *> &bankOne);
        void bankTwoChanged(const QList<RomInfo *> &bankTwo);

    private slots:
        void readyUpdate(bool newReady);
        void setBusy(bool busy);

        void readCartImpl(const QUrl &outFileUrl, int intMemory, int intBank = -1, int romIndex = -1);
        void writeCartImpl(const QUrl &inFileUrl, int intMemory, int intBank = -1, int offset = -1);

        void updateInfoImpl();

    private:
        EmsCart(QObject *parent = nullptr);

        static EmsCart *s_instance;

        struct libusb_device_handle *m_deviceHandle;
        bool m_interfaceClaimed;
        bool m_busy;
        double m_progress;

        QList<RomInfo *> m_bankOne;
        QList<RomInfo *> m_bankTwo;

        QByteArray createCommandBuffer(uint8_t command, uint32_t offset, uint32_t count);
        bool isValidHeader(const QByteArray &header, int offset);
};

#endif
