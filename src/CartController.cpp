#include "CartController.h"

CartController::CartController(QObject *parent) :
    QObject(parent)
{
    m_emsCart = new EmsCart(this);
}

CartController::~CartController()
{
}

bool CartController::isReady()
{
    return m_emsCart->ready();
}
