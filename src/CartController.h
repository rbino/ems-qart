#ifndef _CART_CONTROLLER_H
#define _CART_CONTROLLER_H

#include <QObject>

#include "EmsCart.h"
#include "RomInfo.h"

class CartController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool ready READ isReady NOTIFY readyChanged)
    Q_PROPERTY(QObject * bankOne READ bankOne)
    Q_PROPERTY(QObject * bankTwo READ bankTwo)

    public:
        CartController(QObject *parent = nullptr);

        virtual ~CartController();

        Q_INVOKABLE void refresh();

        bool isReady();
        RomInfo *bankOne();
        RomInfo *bankTwo();

    signals:
        void readyChanged(bool ready);

    private slots:
        void readyUpdate(bool newReady);

    private:
        EmsCart *m_emsCart;
        RomInfo *m_bankOne;
        RomInfo *m_bankTwo;
};

#endif
