#ifndef _CART_CONTROLLER_H
#define _CART_CONTROLLER_H

#include <QObject>
#include <QUrl>

#include "EmsCart.h"

class CartController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool ready READ isReady NOTIFY readyChanged)
    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)
    Q_PROPERTY(double progress READ progress NOTIFY progressChanged)

    public:
        CartController(QObject *parent = nullptr);

        virtual ~CartController();

        Q_INVOKABLE void refresh();
        Q_INVOKABLE void readROM(const QUrl &outFileUrl, int bank, int romIndex);
        Q_INVOKABLE void readSRAM(const QUrl &outFileUrl);
        Q_INVOKABLE void writeROM(const QUrl &inFileUrl, int bank, int offset);
        Q_INVOKABLE void writeSRAM(const QUrl &inFileUrl);

        Q_INVOKABLE static QString urlToLocalPath(const QUrl &fileUrl);

        bool isReady() const;
        bool isBusy() const;
        double progress() const;

    signals:
        void readyChanged(bool ready);
        void busyChanged(bool busy);

        void progressChanged(double progress);

        void error(QString message);
        void transferCompleted();


    private slots:
        void emsErrorUpdate(QString message);

    private:
        EmsCart *m_emsCart;
};

#endif
