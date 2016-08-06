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

void EmsCart::findDevice()
{
    ssize_t numDevices = 0;
    libusb_device **deviceList = nullptr;
    struct libusb_device_descriptor deviceDescriptor;
    int result = 0;

    numDevices = libusb_get_device_list(nullptr, &deviceList);
    if (numDevices >= 0) {
        int i;
        for (i=0; i < numDevices; ++i) {
            (void) memset(&deviceDescriptor, 0, sizeof(deviceDescriptor));
            result = libusb_get_device_descriptor(deviceList[i], &deviceDescriptor);
            if (result == 0) {
                if (deviceDescriptor.idVendor == EmsConstants::UsbVID
                    && deviceDescriptor.idProduct == EmsConstants::UsbPID) {
                    result = libusb_open(deviceList[i], &m_deviceHandle);
                    if (result != 0) {
                        /*
                         * According to the documentation, m_deviceHandle will not
                         * be populated on error, so it should remain nullptr
                         */
                        qWarning() << "Failed to open device, libusb error: " << libusb_error_name(result);
                        if (result == LIBUSB_ERROR_ACCESS) {
                            qWarning() << "Try running as root/sudo or update udev rules (check the FAQ for more info)";
                        }
                    }
                    break;
                }
            } else {
                qWarning() << "Failed to get device description, libusb error: " << libusb_error_name(result);
            }
        }
        if (i == numDevices) {
            qWarning() << "Could not find device, is it plugged in?";
        }
        libusb_free_device_list(deviceList, 1);
        deviceList = nullptr;
    } else {
      qWarning() << "Failed to get device list: " << libusb_error_name((int)numDevices);
    }
}

bool EmsCart::ready()
{
    return (m_deviceHandle != nullptr && m_interfaceClaimed);
}
