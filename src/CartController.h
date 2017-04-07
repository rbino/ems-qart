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
    Q_PROPERTY(QString localFilePath READ localFilePath NOTIFY localFilePathChanged)

    Q_ENUMS(CartMemory)

    public:
        enum CartMemory {
            ROM,
            SRAM
        };

        CartController(QObject *parent = nullptr);

        virtual ~CartController();

        Q_INVOKABLE void refresh();
        Q_INVOKABLE void setLocalFilePath(QUrl fileUrl, QString extension);
        Q_INVOKABLE void clearLocalFilePath();
        Q_INVOKABLE void readCart(const QUrl &outFileUrl, CartMemory memory, int intBank, int romIndex);
        Q_INVOKABLE void writeCart(CartMemory memory, int bank);

        Q_INVOKABLE static QString urlToLocalPath(const QUrl &fileUrl);

        bool isReady() const;
        bool isBusy() const;
        double progress() const;
        QString localFilePath() const;

    signals:
        void readyChanged(bool ready);
        void busyChanged(bool busy);
        void localFilePathChanged(QString localFilePath);

        void progressChanged(double progress);

        void error(QString message);
        void transferCompleted();


    private slots:
        void emsErrorUpdate(QString message);

    private:
        bool m_busy;
        double m_progress;
        QString m_localFilePath;

        EmsCart *m_emsCart;

        void readCartImpl(const QUrl &outFileUrl, CartMemory memory, EmsCart::Bank bank, int romIndex);
        void writeCartImpl(CartMemory memory, int bank);

        void setBusy(bool busy);
};

#endif
