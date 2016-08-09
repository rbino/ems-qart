#ifndef _CART_CONTROLLER_H
#define _CART_CONTROLLER_H

#include <QObject>
#include <QUrl>

#include "EmsCart.h"
#include "RomInfo.h"

class CartController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool ready READ isReady NOTIFY readyChanged)
    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)
    Q_PROPERTY(QString localFilePath READ localFilePath NOTIFY localFilePathChanged)
    Q_PROPERTY(QObject * bankOne READ bankOne NOTIFY bankOneChanged)
    Q_PROPERTY(QObject * bankTwo READ bankTwo NOTIFY bankTwoChanged)

    public:
        enum CartMemory {
            ROM,
            SRAM
        };
        Q_ENUM(CartMemory)

        CartController(QObject *parent = nullptr);

        virtual ~CartController();

        Q_INVOKABLE void refresh();
        Q_INVOKABLE void setLocalFilePath(QUrl fileUrl, QString extension);
        Q_INVOKABLE void clearLocalFilePath();

        bool isReady();
        bool isBusy();
        QString localFilePath();
        RomInfo *bankOne();
        RomInfo *bankTwo();

    signals:
        void readyChanged(bool ready);
        void busyChanged(bool busy);
        void localFilePathChanged(QString localFilePath);

        void bankOneChanged(QObject *bankOne);
        void bankTwoChanged(QObject *bankTwo);

    private slots:
        void readyUpdate(bool newReady);

    private:
        bool m_busy;
        QString m_localFilePath;

        EmsCart *m_emsCart;
        RomInfo *m_bankOne;
        RomInfo *m_bankTwo;
};

#endif
