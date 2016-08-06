#include "CartController.h"

CartController::CartController(QObject *parent) :
    QObject(parent)
{
    m_emsCart = new EmsCart(this);
}

CartController::~CartController()
{
}
