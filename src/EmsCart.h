#ifndef _EMS_CART_H
#define _EMS_CART_H

#include <QObject>

#include <libusb.h>

namespace EmsConstants {
    const int UsbVID = 0x4670;
    const int UsbPID = 0x9394;
}

class EmsCart : public QObject
{
    Q_OBJECT

    public:
        EmsCart(QObject *parent = nullptr);

        virtual ~EmsCart();

        bool init();
        void findDevice();

        bool ready();

    signals:
        void readyChanged(bool ready);

    private:
        struct libusb_device_handle *m_deviceHandle;
        bool m_interfaceClaimed;
};

#endif
