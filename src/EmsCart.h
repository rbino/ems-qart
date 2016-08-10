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
}

class EmsCart : public QObject
{
    Q_OBJECT

    public:
        enum EmsMemory {
            ROM = 1,
            SRAM = 2
        };
        Q_ENUM(EmsMemory)

        enum EmsCommands {
            WriteSRAMCommand = 0x4d,
            ReadSRAMCommand = 0x6d,
            WriteROMCommand = 0x57,
            ReadROMCommand = 0xff
        };
        Q_ENUM(EmsCommands)

        EmsCart(QObject *parent = nullptr);

        virtual ~EmsCart();

        void findDevice();
        QByteArray read(EmsMemory from, uint32_t offset, uint32_t count);
        bool write(EmsMemory to, QByteArray data, uint32_t offset, uint32_t count);

        bool ready();

    signals:
        void readyChanged(bool ready);
        void error(QString message);

    private:
        struct libusb_device_handle *m_deviceHandle;
        bool m_interfaceClaimed;

        QByteArray createCommandBuffer(uint8_t command, uint32_t offset, uint32_t count);
};

#endif
