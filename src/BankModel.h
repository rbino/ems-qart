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

        void setBank(Bank bank);

    private:
        Bank m_bank;
};

#endif
