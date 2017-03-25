#include "BankModel.h"

BankModel::BankModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_bank(InvalidBank)
{
    m_roleNames.insert(Qt::UserRole, "title");
    m_roleNames.insert(Qt::UserRole + 1, "size");
    m_roleNames.insert(Qt::UserRole + 2, "offset");
}

BankModel::~BankModel()
{
}

BankModel::Bank BankModel::bank() const
{
    return m_bank;
}

QHash<int, QByteArray> BankModel::roleNames() const
{
    return m_roleNames;
}

int BankModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_titles.count();
}

void BankModel::setBank(Bank bank)
{
    m_bank = bank;
}
