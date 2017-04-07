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

    public:
        CartController(QObject *parent = nullptr);

        virtual ~CartController();

        Q_INVOKABLE void refresh();
        Q_INVOKABLE void setLocalFilePath(QUrl fileUrl, QString extension);
        Q_INVOKABLE void clearLocalFilePath();
        Q_INVOKABLE void readCart(const QUrl &outFileUrl, int memory, int bank, int romIndex);
        Q_INVOKABLE void writeCart(int memory, int bank);

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

        void readCartImpl(const QUrl &outFileUrl, int intMemory, int intBank, int romIndex);
        void writeCartImpl(int intMemory, int intBank);

        void setBusy(bool busy);
};

#endif
