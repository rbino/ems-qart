#ifndef _BANK_MODEL_H_
#define _BANK_MODEL_H_

#include <QAbstractListModel>

class BankModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(Bank bank READ bank WRITE setBank)

    Q_ENUMS(Bank)

    public:
        enum Bank {
            InvalidBank = 0,
            BankOne = 1,
            BankTwo = 2
        };

        BankModel(QObject *parent = nullptr);
        virtual ~BankModel();

        Bank bank() const;

        QHash<int, QByteArray> roleNames() const override;

        void setBank(Bank bank);

    private:
        Bank m_bank;
        QHash<int, QByteArray> m_roleNames;

        QList<QString> m_titles;
        QList<int> m_sizes;
        QList<int> m_offsets;
};

#endif
