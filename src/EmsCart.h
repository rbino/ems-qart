#ifndef _EMS_CART_H
#define _EMS_CART_H

#include <QObject>

#include <libusb.h>

class EmsCart : public QObject
{
    Q_OBJECT

    public:
        EmsCart(QObject *parent = nullptr);

        virtual ~EmsCart();

        bool init();

    private:
        struct libusb_device_handle *m_deviceHandle;
        bool m_interfaceClaimed;
};

#endif
