#ifndef _BANK_MODEL_H_
#define _BANK_MODEL_H_

#include "EmsCart.h"

#include "RomListModel.h"

class BankModel : public RomListModel
{
    Q_OBJECT

    Q_PROPERTY(EmsCart::Bank bank READ bank WRITE setBank)

    public:
        BankModel(QObject *parent = nullptr);
        virtual ~BankModel();

        EmsCart::Bank bank() const;

        void setBank(EmsCart::Bank bank);

    private slots:
        void refreshData();

    private:
        EmsCart::Bank m_bank;
};

#endif
