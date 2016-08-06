#include "CartController.h"

CartController::CartController(QObject *parent) :
    QObject(parent)
{
    m_emsCart = new EmsCart(this);
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
