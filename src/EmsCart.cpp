#include "EmsCart.h"

#include <QDebug>

EmsCart::EmsCart(QObject *parent) :
    QObject(parent)
{
    m_deviceHandle = nullptr;
    m_interfaceClaimed = false;
}

EmsCart::~EmsCart()
{
    if (m_interfaceClaimed) {
        libusb_release_interface(m_deviceHandle, 0);
    }

    if (m_deviceHandle) {
        libusb_close(m_deviceHandle);
    }

    libusb_exit(nullptr);
}

bool EmsCart::init()
{
    int result = libusb_init(NULL);
    if (result < 0) {
        qCritical() << "Failed to initialize libusb";
        return false;
    }
    return true;
}

bool EmsCart::ready()
{
    return (m_deviceHandle != nullptr && m_interfaceClaimed);
}
