#ifndef _BANK_MODEL_H_
#define _BANK_MODEL_H_

#include "EmsCart.h"

#include <QAbstractListModel>

class BankModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(EmsCart::Bank bank READ bank WRITE setBank)

    public:
        BankModel(QObject *parent = nullptr);
        virtual ~BankModel();

        EmsCart::Bank bank() const;

        QHash<int, QByteArray> roleNames() const override;
        int rowCount(const QModelIndex &parent) const override;
        QVariant data(const QModelIndex &index, int role) const override;

        void setBank(EmsCart::Bank bank);

    private slots:
        void refreshData();

    private:
        EmsCart::Bank m_bank;
        QHash<int, QByteArray> m_roleNames;

        QList<QString> m_titles;
        QList<int> m_sizes;
        QList<int> m_offsets;
};

#endif
