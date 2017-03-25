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

QVariant BankModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row >= m_titles.count()) {
        return QVariant();
    }

    switch (role) {
        case Qt::UserRole:
            return QVariant(m_titles.value(row));

        case Qt::UserRole + 1:
            return QVariant(m_sizes.value(row));

        case Qt::UserRole + 2:
            return QVariant(m_offsets.value(row));

        default:
            return QVariant();
    }
}

void BankModel::setBank(Bank bank)
{
    m_bank = bank;
}
