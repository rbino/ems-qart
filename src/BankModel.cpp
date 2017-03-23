#include "BankModel.h"

BankModel::BankModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_bank(InvalidBank)
{
}

BankModel::~BankModel()
{
}

BankModel::Bank BankModel::bank() const
{
    return m_bank;
}

void BankModel::setBank(Bank bank)
{
    m_bank = bank;
}
