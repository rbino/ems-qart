#include "EmsCart.h"

#include "RomInfo.h"

#include <QDebug>
#include <QDataStream>
#include <QtEndian>

EmsCart *EmsCart::s_instance;

EmsCart::EmsCart(QObject *parent) :
    QObject(parent)
{
    m_deviceHandle = nullptr;
    m_interfaceClaimed = false;

    int result = libusb_init(NULL);
    if (result < 0) {
        emit error(QStringLiteral("Failed to initialize libusb"));
    }
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

EmsCart *EmsCart::instance()
{
    if (!s_instance) {
        s_instance = new EmsCart();
    }
    return s_instance;
}

void EmsCart::findDevice()
{
    int result = 0;
    if (ready()) {
        libusb_release_interface(m_deviceHandle, 0);
        result = libusb_claim_interface(m_deviceHandle, 0);
        if (result < 0) {
            qWarning() << "usb_claim_interface error " << result;
            m_interfaceClaimed = false;
            libusb_close(m_deviceHandle);
            m_deviceHandle = nullptr;
            readyUpdate(false);
        } else {
            return;
        }
    }

    ssize_t numDevices = 0;
    libusb_device **deviceList = nullptr;
    struct libusb_device_descriptor deviceDescriptor;

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
                            emit error(QStringLiteral("Device access error. Did you install udev rules? Check README"));
                        } else if (result == LIBUSB_ERROR_NOT_SUPPORTED) {
                            emit error(QStringLiteral("Device not supported. Did you install the drivers? Check README"));
                        }
                    } else {
                        result = libusb_claim_interface(m_deviceHandle, 0);
                        if (result < 0) {
                            qWarning() << "usb_claim_interface error " << result;
                        } else {
                            m_interfaceClaimed = true;
                            readyUpdate(true);
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

QByteArray EmsCart::createCommandBuffer(uint8_t command, uint32_t offset, uint32_t count)
{
    QByteArray commandBuffer;
    commandBuffer.resize(sizeof(command) + sizeof(offset) + sizeof(count));
    QDataStream commandStream(&commandBuffer, QIODevice::ReadWrite);
    commandStream.writeRawData((const char *)&command, sizeof(command));
    uint32_t bigEndianOffset = qToBigEndian(offset);
    commandStream.writeRawData((const char *)&bigEndianOffset, sizeof(bigEndianOffset));
    uint32_t bigEndianCount = qToBigEndian(count);
    commandStream.writeRawData((const char *)&bigEndianCount, sizeof(bigEndianCount));
    return commandBuffer;
}

QByteArray EmsCart::read(EmsMemory from, uint32_t offset, uint32_t count)
{
    int result, transferred;
    uint8_t cmd;

    switch (from) {
        case (ROM):
            cmd = ReadROMCommand;
            break;
        case (SRAM):
            cmd = ReadSRAMCommand;
            break;
        default:
            qWarning() << "from must be ROM or SRAM, aborting";
            return QByteArray();
    }

    QByteArray cmdBuffer = createCommandBuffer(cmd, offset, count);
    // Send the read command
    result = libusb_bulk_transfer(m_deviceHandle, EmsConstants::SendEndpoint, reinterpret_cast<uchar *>(cmdBuffer.data()), cmdBuffer.size(), &transferred, 0);
    if (result < 0) {
        return QByteArray();
    }

    QByteArray outBuffer;
    outBuffer.resize(count);
    // Read the data
    result = libusb_bulk_transfer(m_deviceHandle, EmsConstants::ReceiveEndpoint, reinterpret_cast<uchar *>(outBuffer.data()), count, &transferred,   0);
    if (result < 0) {
        return QByteArray();
    }

    return outBuffer;
}

bool EmsCart::write(EmsMemory to, QByteArray data, uint32_t offset, uint32_t count)
{
    int result, transferred;
    uint8_t cmd;

    switch (to) {
        case (ROM):
            cmd = WriteROMCommand;
            break;
        case (SRAM):
            cmd = WriteSRAMCommand;
            break;
        default:
            qWarning() << "to must be ROM or SRAM, aborting";
            return false;
    }

    QByteArray outBuffer = createCommandBuffer(cmd, offset, count);
    outBuffer.append(data);

    // Send the write command with data
    result = libusb_bulk_transfer(m_deviceHandle, EmsConstants::SendEndpoint, reinterpret_cast<uchar *>(outBuffer.data()), outBuffer.size(), &transferred, 0);
    if (result < 0) {
        return false;
    }

    return true;
}

bool EmsCart::ready() const
{
    return (m_deviceHandle != nullptr && m_interfaceClaimed);
}

bool EmsCart::busy() const
{
    return m_busy;
}

QList<RomInfo *> EmsCart::bankOne() const
{
    return m_bankOne;
}

QList<RomInfo *> EmsCart::bankTwo() const
{
    return m_bankTwo;
}

void EmsCart::readyUpdate(bool newReady)
{
    if (newReady) {
        updateInfo();
    } else {
        qDeleteAll(m_bankOne);
        m_bankOne.clear();
        emit bankOneChanged(m_bankOne);
        qDeleteAll(m_bankTwo);
        m_bankTwo.clear();
        emit bankTwoChanged(m_bankTwo);
    }
    emit readyChanged(newReady);
}

void EmsCart::updateInfo()
{
    QByteArray header;
    int offset = 0;
    // Bank 1
    while (offset < EmsConstants::BankSize) {
        header = read(EmsCart::ROM, offset, RomConstants::HeaderSize);
        if (isValidHeader(header, offset)) {
            RomInfo *rom = new RomInfo(header, this);
            rom->setOffset(offset);
            m_bankOne.append(rom);
        }
        offset += RomConstants::SmallestRomSize;
    }
    emit bankOneChanged(m_bankOne);

    offset = 0;
    // Bank 2
    while (offset < EmsConstants::BankSize) {
        header = read(EmsCart::ROM, EmsConstants::BankSize + offset, RomConstants::HeaderSize);
        if (isValidHeader(header, offset)) {
            RomInfo *rom = new RomInfo(header, this);
            rom->setOffset(offset);
            m_bankTwo.append(rom);
        }
        offset += RomConstants::SmallestRomSize;
    }
    emit bankTwoChanged(m_bankTwo);
}

bool EmsCart::isValidHeader(const QByteArray &header, int offset)
{
    uint8_t computedChecksum = 0;
    for (int i = RomConstants::TitleOffset; i < RomConstants::ChecksumOffset; i++)
    {
        computedChecksum -= (uint8_t)header.at(i) + 1;
    }
    if (computedChecksum != (uint8_t)header.at(RomConstants::ChecksumOffset)) {
        // Wrong checksum
        return false;
    }

    int sizeCode = header.at(RomConstants::ROMSizeOffset);
    if (sizeCode < 0 || sizeCode > 7) {
        // Not a power-of-2-sized ROM
        return false;
    }

    int size = 32 << (sizeCode + 10);
    if (offset % size != 0 ||                     // Unaligned ROM
        offset + size > EmsConstants::BankSize) { // Out-of-bound ROM

        return false;
    }

    return true;
}
