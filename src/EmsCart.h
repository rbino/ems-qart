#ifndef _EMS_CART_H
#define _EMS_CART_H

#include <QObject>

#include <libusb.h>

namespace EmsConstants {
    const int UsbVID = 0x4670;
    const int UsbPID = 0x9394;
    const int ReadBlocksize = 4096;
    const int WriteBlocksize = 32;
    const int SendEndpoint = 2 | LIBUSB_ENDPOINT_OUT;
    const int ReceiveEndpoint = 1 | LIBUSB_ENDPOINT_IN;
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

        bool ready();

    signals:
        void readyChanged(bool ready);

    private:
        struct libusb_device_handle *m_deviceHandle;
        bool m_interfaceClaimed;
};

#endif
