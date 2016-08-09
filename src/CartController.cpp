#include "CartController.h"

#include <QDebug>

CartController::CartController(QObject *parent) :
    QObject(parent)
{
    m_busy = false;
    m_progress = 0;
    m_emsCart = new EmsCart(this);
    m_bankOne = new RomInfo(this);
    m_bankTwo = new RomInfo(this);
    connect(m_emsCart, &EmsCart::readyChanged, this, &CartController::readyUpdate);
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
