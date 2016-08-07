#include "CartController.h"

#include <QDebug>

CartController::CartController(QObject *parent) :
    QObject(parent)
{
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
    }
    emit readyChanged(newReady);
}
