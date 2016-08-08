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
    Q_PROPERTY(QString localFilePath READ localFilePath NOTIFY localFilePathChanged)
    Q_PROPERTY(QObject * bankOne READ bankOne)
    Q_PROPERTY(QObject * bankTwo READ bankTwo)

    public:
        CartController(QObject *parent = nullptr);

        virtual ~CartController();

        Q_INVOKABLE void refresh();
        Q_INVOKABLE void setLocalFilePath(QUrl fileUrl, QString extension);
        Q_INVOKABLE void clearLocalFilePath();

        bool isReady();
        QString localFilePath();
        RomInfo *bankOne();
        RomInfo *bankTwo();

    signals:
        void readyChanged(bool ready);
        void localFilePathChanged(QString localFilePath);

    private slots:
        void readyUpdate(bool newReady);

    private:
        QString m_localFilePath;

        EmsCart *m_emsCart;
        RomInfo *m_bankOne;
        RomInfo *m_bankTwo;
};

#endif
