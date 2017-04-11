#include "RomListModel.h"

#include "RomInfo.h"

RomListModel::RomListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roleNames.insert(Qt::UserRole, "title");
    m_roleNames.insert(Qt::UserRole + 1, "size");
    m_roleNames.insert(Qt::UserRole + 2, "offset");
    m_roleNames.insert(Qt::UserRole + 3, "sourceFile");
}

RomListModel::~RomListModel()
{
}

QHash<int, QByteArray> RomListModel::roleNames() const
{
    return m_roleNames;
}

int RomListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_titles.count();
}

QVariant RomListModel::data(const QModelIndex &index, int role) const
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

        case Qt::UserRole + 3:
            return QVariant(m_sourceFiles.value(row));

        default:
            return QVariant();
    }
}

void RomListModel::addRom(RomInfo *rom)
{
    beginInsertRows(QModelIndex(), m_titles.count(), m_titles.count());
    m_titles.append(rom->title());
    m_sizes.append(rom->romSize());
    m_offsets.append(rom->offset());
    m_sourceFiles.append(rom->sourceFile());
    endInsertRows();
}

void RomListModel::removeRom(int index)
{
    if (index < 0 || index > m_titles.size()) {
        return;
    }
    beginRemoveRows(QModelIndex(), index, index);
    m_titles.removeAt(index);
    m_sizes.removeAt(index);
    m_offsets.removeAt(index);
    m_sourceFiles.removeAt(index);
    endRemoveRows();
}

void RomListModel::removeAll()
{
    beginResetModel();
    m_titles.clear();
    m_sizes.clear();
    m_offsets.clear();
    m_sourceFiles.clear();
    endResetModel();
}
