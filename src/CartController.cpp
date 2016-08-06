#include "CartController.h"

CartController::CartController(QObject *parent) :
    QObject(parent)
{
    m_emsCart = new EmsCart(this);
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

void CartController::readyUpdate(bool newReady)
{
    emit readyChanged(newReady);
}
