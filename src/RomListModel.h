#ifndef _ROMLISTMODEL_H_
#define _ROMLISTMODEL_H_

#include "EmsCart.h"

#include <QAbstractListModel>

class RomListModel : public QAbstractListModel
{
    Q_OBJECT

    public:
        RomListModel(QObject *parent = nullptr);
        virtual ~RomListModel();

        virtual QHash<int, QByteArray> roleNames() const override;
        virtual int rowCount(const QModelIndex &parent) const override;
        virtual QVariant data(const QModelIndex &index, int role) const override;

        virtual void addRom(RomInfo *rom);
        virtual void removeRom(int index);
        virtual void removeAll();

    protected:
        QHash<int, QByteArray> m_roleNames;

        QList<QString> m_titles;
        QList<int> m_sizes;
        QList<int> m_offsets;
        QList<QString> m_sourceFiles;
};

#endif
