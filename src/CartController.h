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
    Q_PROPERTY(double progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QString localFilePath READ localFilePath NOTIFY localFilePathChanged)
    Q_PROPERTY(QList<RomInfo *> bankOne READ bankOne NOTIFY bankOneChanged)
    Q_PROPERTY(QList<RomInfo *> bankTwo READ bankTwo NOTIFY bankTwoChanged)

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
        Q_INVOKABLE void readCart(CartMemory memory, int bank, int romIndex);
        Q_INVOKABLE void writeCart(CartMemory memory, int bank);

        bool isReady();
        bool isBusy();
        double progress();
        QString localFilePath();
        QList<RomInfo *> bankOne();
        QList<RomInfo *> bankTwo();

    signals:
        void readyChanged(bool ready);
        void busyChanged(bool busy);
        void localFilePathChanged(QString localFilePath);

        void progressChanged(double progress);

        void error(QString message);
        void transferCompleted();

        void bankOneChanged(const QList<RomInfo *> &bankOne);
        void bankTwoChanged(const QList<RomInfo *> &bankTwo);

    private slots:
        void readyUpdate(bool newReady);
        void emsErrorUpdate(QString message);

    private:
        bool m_busy;
        double m_progress;
        QString m_localFilePath;

        EmsCart *m_emsCart;
        QList<RomInfo *> m_bankOne;
        QList<RomInfo *> m_bankTwo;

        void readCartImpl(CartMemory memory, int bank, int romIndex);
        void writeCartImpl(CartMemory memory, int bank);
        void updateInfo();
        bool isValidHeader(const QByteArray &header, int offset);
};

#endif
