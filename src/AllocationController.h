#ifndef _ALLOCATIONCONTROLLER_H_
#define _ALLOCATIONCONTROLLER_H_

#include "EmsCart.h"

#include <QObject>

class Allocator;
class RomListModel;

class AllocationController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(EmsCart::Bank bank READ bank WRITE setBank)
    Q_PROPERTY(QObject * romsModel READ romsModel NOTIFY romsModelChanged)
    Q_PROPERTY(int freeSpace READ freeSpace NOTIFY freeSpaceChanged)

    public:
        AllocationController(QObject *parent = nullptr);
        virtual ~AllocationController();

        QObject *romsModel() const;
        EmsCart::Bank bank() const;
        int freeSpace() const;

        void setBank(EmsCart::Bank bank);

        Q_INVOKABLE bool allocate(const QUrl &romUrl);
        bool allocate(RomInfo *rom);

        void remove(int romIndex);

        void reallocateAll();

    signals:
        void romsModelChanged();
        void bankChanged(EmsCart::Bank bank);
        void freeSpaceChanged();

    private:
        RomListModel *m_romsModel;
        Allocator *m_allocator;
        QList<RomInfo*> m_allocatedRoms;

        EmsCart::Bank m_bank;
};

#endif // _ALLOCATIONCONTROLLER_H_
