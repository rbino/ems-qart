#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include <QHash>
#include <QSet>

class RomInfo;

class Allocator
{
    public:
        Allocator();
        Allocator(const QList<RomInfo*> &initialRoms);
        ~Allocator();

        bool allocate(RomInfo *rom);

        int freeSpace() const;

    private:
        void mergeBuddies();

        QHash<int, QList<int>> m_orderToFreeAddresses;
};

#endif // _ALLOCATOR_H_
