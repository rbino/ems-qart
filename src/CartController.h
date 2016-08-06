#ifndef _CART_CONTROLLER_H
#define _CART_CONTROLLER_H

#include <QObject>

#include "EmsCart.h"

class CartController : public QObject
{
    Q_OBJECT

    public:
        CartController(QObject *parent = nullptr);

        virtual ~CartController();

    private:
        EmsCart *m_emsCart;
};

#endif
