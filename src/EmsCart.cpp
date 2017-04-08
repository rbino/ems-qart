#include "EmsCart.h"

#include "RomInfo.h"

#include <QApplication>
#include <QDebug>
#include <QDataStream>
#include <QTimer>
#include <QtConcurrent>
#include <QtEndian>

EmsCart *EmsCart::s_instance;

EmsCart::EmsCart(QObject *parent) :
    QObject(parent)
{
    m_deviceHandle = nullptr;
    m_interfaceClaimed = false;
    m_busy = false;

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

QByteArray EmsCart::read(Memory from, uint32_t offset, uint32_t count)
{
    int result, transferred;
    uint8_t cmd;

    switch (from) {
        case (ROM):
            cmd = EmsConstants::ReadROMCommand;
            break;
        case (SRAM):
            cmd = EmsConstants::ReadSRAMCommand;
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

bool EmsCart::write(Memory to, QByteArray data, uint32_t offset, uint32_t count)
{
    int result, transferred;
    uint8_t cmd;

    switch (to) {
        case (ROM):
            cmd = EmsConstants::WriteROMCommand;
            break;
        case (SRAM):
            cmd = EmsConstants::WriteSRAMCommand;
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

double EmsCart::progress() const
{
    return m_progress;
}

QList<RomInfo *> EmsCart::bankOne() const
{
    return m_bankOne;
}

QList<RomInfo *> EmsCart::bankTwo() const
{
    return m_bankTwo;
}

void EmsCart::setBusy(bool busy)
{
    m_busy = busy;
    emit busyChanged(this->busy());
}

void EmsCart::setProgress(double progress)
{
    m_progress = progress;
    emit progressChanged(progress);
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
    QMetaObject::invokeMethod(this, "updateInfoImpl", Qt::QueuedConnection);
}

void EmsCart::updateInfoImpl()
{
    setBusy(true);
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
        QApplication::processEvents();
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
        QApplication::processEvents();
    }
    emit bankTwoChanged(m_bankTwo);
    setBusy(false);
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

void EmsCart::readCart(const QUrl &outFileUrl, int memory, int bank, int romIndex)
{
    setBusy(true);
    QFutureWatcher<void> *watcher = new QFutureWatcher<void>();
    connect(watcher, &QFutureWatcher<void>::finished, this, [this, watcher] {
        setBusy(false);
        watcher->deleteLater();
    });

    QFuture<void> readFuture = QtConcurrent::run(this, &EmsCart::readCartImpl, outFileUrl, memory, bank, romIndex);
    watcher->setFuture(readFuture);
}

void EmsCart::writeCart(const QUrl &inFileUrl, int memory, int bank, int offset)
{
    setBusy(true);
    QFutureWatcher<void> *watcher = new QFutureWatcher<void>();
    connect(watcher, &QFutureWatcher<void>::finished, this, [this, watcher] {
        setBusy(false);
        watcher->deleteLater();
    });

    QFuture<void> writeFuture = QtConcurrent::run(this, &EmsCart::writeCartImpl, inFileUrl, memory, bank, offset);
    watcher->setFuture(writeFuture);
}

void EmsCart::readCartImpl(const QUrl &outFileUrl, int intMemory, int intBank, int romIndex)
{
    EmsCart::Memory memory = static_cast<EmsCart::Memory>(intMemory);
    EmsCart::Bank bank = static_cast<EmsCart::Bank>(intBank);
    setProgress(0);

    QFile outFile(outFileUrl.toLocalFile());
    if (!outFile.open(QIODevice::WriteOnly)) {
        emit error(QStringLiteral("Can't open file %1").arg(outFileUrl.toLocalFile()));
        return;
    }

    int totalReadSize;
    int baseAddress;

    switch (memory) {
        case EmsCart::ROM:
            switch (bank) {
                case EmsCart::BankOne:
                    if (romIndex < 0 || romIndex >= bankOne().size()) {
                        qWarning() << "ROM Index is out of bound, aborting";
                        return;
                    }
                    totalReadSize = bankOne().at(romIndex)->romSize();
                    baseAddress = bankOne().at(romIndex)->offset();
                    break;
                case EmsCart::BankTwo:
                    if (romIndex < 0 || romIndex >= bankTwo().size()) {
                        qWarning() << "ROM Index is out of bound, aborting";
                        return;
                    }
                    totalReadSize = bankTwo().at(romIndex)->romSize();
                    baseAddress = EmsConstants::BankSize + bankTwo().at(romIndex)->offset();
                    break;
                default:
                    qWarning() << "Invalid bank in read, aborting";
                    return;
            }
            break;

        case EmsCart::SRAM:
            totalReadSize = EmsConstants::SRAMSize;
            baseAddress = 0;
            break;

        default:
            qWarning() << "Invalid memory location in read, aborting";
            return;
    }

    int offset = 0;
    while (offset < totalReadSize) {
        QByteArray chunk = read(memory, baseAddress + offset, EmsConstants::ReadBlockSize);
        if (chunk.isEmpty()) {
            emit error(QStringLiteral("Error reading cart at address %1, aborting").arg(baseAddress + offset));
            // Is the cart still connected?
            findDevice();
            return;
        }

        int result = outFile.write(chunk);
        if (result < 0) {
            emit error(QStringLiteral("Error while writing in the file, aborting"));
            return;
        }

        setProgress((double) offset / totalReadSize);

        offset += EmsConstants::ReadBlockSize;
    }

    outFile.close();

    emit transferCompleted();
}

void EmsCart::writeCartImpl(const QUrl &inFileUrl, int intMemory, int intBank, int romOffset)
{
    EmsCart::Memory memory = static_cast<EmsCart::Memory>(intMemory);
    EmsCart::Bank bank = static_cast<EmsCart::Bank>(intBank);
    setProgress(0);

    QFile sourceFile(inFileUrl.toLocalFile());
    if (!sourceFile.open(QIODevice::ReadOnly)) {
        emit error(QStringLiteral("Can't open file %1").arg(inFileUrl.toLocalFile()));
        return;
    }

    int totalWriteSize;
    int baseAddress;

    switch (memory) {
        case EmsCart::ROM:
            totalWriteSize = qMin(EmsConstants::BankSize, (int) sourceFile.size());
            switch (bank) {
                case EmsCart::BankOne:
                    baseAddress = romOffset;
                    break;
                case EmsCart::BankTwo:
                    baseAddress = EmsConstants::BankSize + romOffset;
                    break;
                default:
                    qWarning() << "Invalid bank in write, aborting";
                    return;
            }
            break;

        case EmsCart::SRAM:
            totalWriteSize = qMin(EmsConstants::SRAMSize, (int) sourceFile.size());
            baseAddress = 0;
            break;

        default:
            qWarning() << "Invalid memory location in write, aborting";
            return;
    }

    int offset = 0;
    while (offset < totalWriteSize && sourceFile.bytesAvailable()) {
        QByteArray chunk = sourceFile.read(EmsConstants::WriteBlockSize);
        if (chunk.isEmpty()) {
            emit error(QStringLiteral("Error while reading the source file, aborting"));
            return;
        }

        if (!write(memory, chunk, baseAddress + offset, EmsConstants::WriteBlockSize)) {
            emit error(QStringLiteral("Error writing to cart at address %1, aborting").arg(baseAddress + offset));
            // Is the cart still connected?
            findDevice();
            return;
        }

        setProgress((double) offset / totalWriteSize);

        offset += EmsConstants::WriteBlockSize;
    }

    sourceFile.close();

    emit transferCompleted();

    // Update cart informations
    updateInfo();
}
