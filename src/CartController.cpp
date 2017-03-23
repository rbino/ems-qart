#include "CartController.h"

#include "RomInfo.h"

#include <QDebug>
#include <QFile>
#include <QtConcurrent>

CartController::CartController(QObject *parent) :
    QObject(parent)
{
    m_busy = false;
    m_progress = 0;
    m_emsCart = EmsCart::instance();
    connect(m_emsCart, &EmsCart::readyChanged, this, &CartController::readyChanged);
    connect(m_emsCart, &EmsCart::error, this, &CartController::emsErrorUpdate);
    m_emsCart->findDevice();
}

CartController::~CartController()
{
}

void CartController::refresh()
{
    m_emsCart->findDevice();
}

bool CartController::isReady() const
{
    return m_emsCart->ready();
}

bool CartController::isBusy() const
{
    return m_busy;
}

double CartController::progress() const
{
    return m_progress;
}

QString CartController::localFilePath() const
{
    return m_localFilePath;
}

void CartController::emsErrorUpdate(QString message)
{
    emit error(message);
}

void CartController::setLocalFilePath(QUrl fileUrl, QString extension)
{
    QString localPath = fileUrl.toLocalFile();
    if (!localPath.endsWith(extension)) {
        localPath += extension;
    }
    m_localFilePath = localPath;
    emit localFilePathChanged(m_localFilePath);
}

void CartController::clearLocalFilePath()
{
    m_localFilePath = QString();
    emit localFilePathChanged(m_localFilePath);
}

void CartController::readCart(CartMemory memory, int bank, int romIndex)
{
    QtConcurrent::run(this, &CartController::readCartImpl, memory, bank, romIndex);
}

void CartController::writeCart(CartMemory memory, int bank)
{
    QtConcurrent::run(this, &CartController::writeCartImpl, memory, bank);
}

void CartController::readCartImpl(CartMemory memory, int bank, int romIndex)
{
    m_progress = 0;
    emit progressChanged(m_progress);

    m_busy = true;
    emit busyChanged(m_busy);

    if (m_localFilePath.isEmpty()) {
        emit error(QStringLiteral("You haven't selected the save location!"));
        m_busy = false;
        emit busyChanged(m_busy);
        return;
    }

    QFile outFile(m_localFilePath);
    if (!outFile.open(QIODevice::WriteOnly)) {
        emit error(QStringLiteral("Can't open file %1").arg(m_localFilePath));
        m_busy = false;
        emit busyChanged(m_busy);
        return;
    }

    if (bank < 1 || bank > 2) {
        qWarning() << "You can only select bank 1 or 2, aborting";
        m_busy = false;
        emit busyChanged(m_busy);
        return;
    }

    EmsCart::EmsMemory from;
    int totalReadSize;
    int baseAddress;

    switch (memory) {
        case (ROM):
            from = EmsCart::ROM;
            switch (bank) {
                case 1:
                    if (romIndex < 0 || romIndex >= m_emsCart->bankOne().size()) {
                        qWarning() << "ROM Index is out of bound, aborting";
                        m_busy = false;
                        emit busyChanged(m_busy);
                        return;
                    }
                    if (m_emsCart->bankOne().at(romIndex)->romSize() > 0) {
                        totalReadSize = m_emsCart->bankOne().at(romIndex)->romSize();
                    } else {
                        totalReadSize = EmsConstants::BankSize;
                    }
                    break;
                case 2:
                    if (romIndex < 0 || romIndex >= m_emsCart->bankTwo().size()) {
                        qWarning() << "ROM Index is out of bound, aborting";
                        m_busy = false;
                        emit busyChanged(m_busy);
                        return;
                    }
                    if (m_emsCart->bankTwo().at(romIndex)->romSize() > 0) {
                        totalReadSize = m_emsCart->bankTwo().at(romIndex)->romSize();
                    } else {
                        totalReadSize = EmsConstants::BankSize;
                    }
                    break;
            }
            baseAddress = (bank - 1) * EmsConstants::BankSize;
            break;

        case (SRAM):
            from = EmsCart::SRAM;
            totalReadSize = EmsConstants::SRAMSize;
            baseAddress = 0;
            break;

        default:
            qWarning() << "Invalid memory location in read, aborting";
            m_busy = false;
            emit busyChanged(m_busy);
            return;
    }

    int offset = 0;
    while (offset < totalReadSize) {
        QByteArray chunk = m_emsCart->read(from, baseAddress + offset, EmsConstants::ReadBlockSize);
        if (chunk.isEmpty()) {
            emit error(QStringLiteral("Error reading cart at address %1, aborting").arg(baseAddress + offset));
            // Is the cart still connected?
            m_emsCart->findDevice();
            m_busy = false;
            emit busyChanged(m_busy);
            return;
        }

        int result = outFile.write(chunk);
        if (result < 0) {
            emit error(QStringLiteral("Error while writing in the file, aborting"));
            m_busy = false;
            emit busyChanged(m_busy);
            return;
        }

        m_progress = (double) offset / totalReadSize;
        emit progressChanged(m_progress);

        offset += EmsConstants::ReadBlockSize;
    }

    outFile.close();

    m_busy = false;
    emit busyChanged(m_busy);
    emit transferCompleted();
}

void CartController::writeCartImpl(CartMemory memory, int bank)
{
    m_progress = 0;
    emit progressChanged(m_progress);

    m_busy = true;
    emit busyChanged(m_busy);

    if (m_localFilePath.isEmpty()) {
        emit error(QStringLiteral("You haven't selected the source location!"));
        m_busy = false;
        emit busyChanged(m_busy);
        return;
    }

    QFile sourceFile(m_localFilePath);
    if (!sourceFile.open(QIODevice::ReadOnly)) {
        emit error(QStringLiteral("Can't open file %1").arg(m_localFilePath));
        m_busy = false;
        emit busyChanged(m_busy);
        return;
    }

    if (bank < 1 || bank > 2) {
        qWarning() << "You can only select bank 1 or 2, aborting";
        m_busy = false;
        emit busyChanged(m_busy);
        return;
    }

    EmsCart::EmsMemory to;
    int totalWriteSize;
    int baseAddress;

    switch (memory) {
        case (ROM):
            to = EmsCart::ROM;
            totalWriteSize = qMin(EmsConstants::BankSize, (int) sourceFile.size());
            baseAddress = (bank - 1) * EmsConstants::BankSize;
            break;

        case (SRAM):
            to = EmsCart::SRAM;
            totalWriteSize = qMin(EmsConstants::SRAMSize, (int) sourceFile.size());
            baseAddress = 0;
            break;

        default:
            qWarning() << "Invalid memory location in read, aborting";
            m_busy = false;
            emit busyChanged(m_busy);
            return;
    }

    int offset = 0;
    while (offset < totalWriteSize && sourceFile.bytesAvailable()) {
        QByteArray chunk = sourceFile.read(EmsConstants::WriteBlockSize);
        if (chunk.isEmpty()) {
            emit error(QStringLiteral("Error while reading the source file, aborting"));
            m_busy = false;
            emit busyChanged(m_busy);
            return;
        }

        if (!m_emsCart->write(to, chunk, baseAddress + offset, EmsConstants::WriteBlockSize)) {
            emit error(QStringLiteral("Error writing to cart at address %1, aborting").arg(baseAddress + offset));
            // Is the cart still connected?
            m_emsCart->findDevice();
            m_busy = false;
            emit busyChanged(m_busy);
            return;
        }

        m_progress = (double) offset / totalWriteSize;
        emit progressChanged(m_progress);

        offset += EmsConstants::WriteBlockSize;
    }

    sourceFile.close();

    m_busy = false;
    emit busyChanged(m_busy);
    emit transferCompleted();

    // Update cart informations
    m_emsCart->updateInfo();
}
