#ifndef _CART_CONTROLLER_H
#define _CART_CONTROLLER_H

#include <QObject>

#include "EmsCart.h"

class CartController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool ready READ isReady NOTIFY readyChanged)

    public:
        enum CartHeaderOffsets {
            LogoOffset = 0x104,
            TitleOffset = 0x134,
            CGBFlagOffset = 0x143,
            SGBFlagOffset = 0x146,
            ROMSizeOffset = 0x148,
            SRAMSizeOffset = 0x149,
            RegionOffset = 0x14a,
            OldLicenseOffset = 0x14b,
            RomVersionOffset = 0x14c,
            ChecksumOffset = 0x14d
        };
        Q_ENUM(CartHeaderOffsets)

        CartController(QObject *parent = nullptr);

        virtual ~CartController();

        Q_INVOKABLE void refresh();

        bool isReady();

    signals:
        void readyChanged(bool ready);

    private slots:
        void readyUpdate(bool newReady);

    private:
        EmsCart *m_emsCart;
};

#endif
