#include "Allocator.h"

#include "EmsCart.h"
#include "RomInfo.h"

#include <QDebug>

#define MAX_ORDER 7

Allocator::Allocator(const QList<RomInfo*> &initialRoms)
{
    // First, we add all the possible smallest blocks (the smallest possible ROM)
    QList<int> order0Blocks;
    for (int addr = 0; addr < EmsConstants::BankSize; addr += RomConstants::SmallestRomSize) {
        order0Blocks.append(addr);
    }
    m_orderToFreeAddresses.insert(0, order0Blocks);

    // The remaining orders are just empty sets for now
    for (int order = 1; order <= MAX_ORDER; order++) {
        m_orderToFreeAddresses.insert(order, QList<int>());
    }

    // For every ROM, we remove all the order0 blocks it covers
    for (const RomInfo *rom : initialRoms) {
        for (int addr = 0; addr < rom->romSize(); addr += RomConstants::SmallestRomSize) {
            m_orderToFreeAddresses[0].removeOne(rom->offset() + addr);
        }
    }

    // Finally, we merge the buddy blocks
    mergeBuddies();
}

Allocator::~Allocator()
{
}

void Allocator::mergeBuddies()
{
    for (int order = 0; order < MAX_ORDER; order++) {
        int orderBlockSize = RomConstants::SmallestRomSize << order;
        QSet<int> toBeRemoved;
        for (int freeAddr : m_orderToFreeAddresses.value(order)) {
            int buddyAddress = freeAddr ^ orderBlockSize;
            if (m_orderToFreeAddresses.value(order).contains(buddyAddress)
                && freeAddr < buddyAddress) {
                // We "merge" the two and we insert the lower address in the higher order blocks
                m_orderToFreeAddresses[order + 1].append(freeAddr);

                toBeRemoved.insert(freeAddr);
                toBeRemoved.insert(buddyAddress);
            }
        }
        for (int el : toBeRemoved) {
            m_orderToFreeAddresses[order].removeOne(el);
        }
    }
}
