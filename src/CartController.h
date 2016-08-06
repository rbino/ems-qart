#ifndef _CART_CONTROLLER_H
#define _CART_CONTROLLER_H

#include <QObject>

#include "EmsCart.h"

class CartController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool ready READ isReady NOTIFY readyChanged)

    public:
        CartController(QObject *parent = nullptr);

        virtual ~CartController();

        void refresh();

        bool isReady();

    signals:
        void readyChanged(bool ready);

    private slots:
        void readyUpdate(bool newReady);

    private:
        EmsCart *m_emsCart;
};

#endif
