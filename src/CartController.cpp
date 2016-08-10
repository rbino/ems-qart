#include "CartController.h"

#include <QDebug>
#include <QFile>
#include <QtConcurrent>

CartController::CartController(QObject *parent) :
    QObject(parent)
{
    m_busy = false;
    m_progress = 0;
    m_emsCart = new EmsCart(this);
    m_bankOne = new RomInfo(this);
    m_bankTwo = new RomInfo(this);
    connect(m_emsCart, &EmsCart::readyChanged, this, &CartController::readyUpdate);
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

bool CartController::isReady()
{
    return m_emsCart->ready();
}

bool CartController::isBusy()
{
    return m_busy;
}

double CartController::progress()
{
    return m_progress;
}

QString CartController::localFilePath()
{
    return m_localFilePath;
}

RomInfo *CartController::bankOne()
{
    return m_bankOne;
}

RomInfo *CartController::bankTwo()
{
    return m_bankTwo;
}

void CartController::readyUpdate(bool newReady)
{
    if (newReady) {
        QByteArray header = m_emsCart->read(EmsCart::ROM, 0, 512);
        m_bankOne->updateInfo(header);
        header = m_emsCart->read(EmsCart::ROM, EmsConstants::BankSize, 512);
        m_bankTwo->updateInfo(header);
        qDebug() << "Bank 1";
        qDebug() << "Title: " << m_bankOne->title();
        qDebug() << "ChecksumValid: " << m_bankOne->isChecksumValid() << "\n";
        qDebug() << "Bank 2";
        qDebug() << "Title: " << m_bankTwo->title();
        qDebug() << "ChecksumValid: " << m_bankTwo->isChecksumValid() << "\n";
    } else {
        m_bankOne->resetInfo();
        m_bankTwo->resetInfo();
    }
    emit readyChanged(newReady);
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

void CartController::readCart(CartMemory memory, int bank)
{
    QtConcurrent::run(this, &CartController::readCartImpl, memory, bank);
}

void CartController::readCartImpl(CartMemory memory, int bank)
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
            totalReadSize = EmsConstants::BankSize;
            baseAddress = bank * EmsConstants::BankSize;
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
    while (offset <= totalReadSize) {
        QByteArray chunk = m_emsCart->read(from, baseAddress + offset, EmsConstants::ReadBlockSize);
        if (chunk.isEmpty()) {
            emit error(QStringLiteral("Error reading cart at address %1, aborting").arg(baseAddress + offset));
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
