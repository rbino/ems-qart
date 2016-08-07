#include "CartController.h"

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
    emit readyChanged(newReady);
}
