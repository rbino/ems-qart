#include "AllocationController.h"

#include "Allocator.h"
#include "EmsCart.h"
#include "RomInfo.h"
#include "RomListModel.h"

AllocationController::AllocationController(QObject *parent)
    : QObject(parent)
    , m_romsModel(new RomListModel())
    , m_allocator(new Allocator())
{
}

AllocationController::~AllocationController()
{
}

QObject *AllocationController::romsModel() const
{
    return m_romsModel;
}

EmsCart::Bank AllocationController::bank() const
{
    return m_bank;
}

void AllocationController::setBank(EmsCart::Bank bank)
{
    if (m_bank != bank) {
        m_bank = bank;
        emit bankChanged(bank);

        reallocateAll();
    }
}

void AllocationController::reallocateAll()
{
    if (m_allocator) {
        delete m_allocator;
    }

    switch (m_bank) {
        case EmsCart::BankOne:
            m_allocator = new Allocator(EmsCart::instance()->bankOne());
            break;

        case EmsCart::BankTwo:
            m_allocator = new Allocator(EmsCart::instance()->bankTwo());
            break;

        default:
            m_allocator = new Allocator();
            // We return, but we keep allocated ROMs so we can retry
            return;
    }

    QList<RomInfo*> roms = m_allocatedRoms;
    m_allocatedRoms.clear();
    m_romsModel->removeAll();

    for (RomInfo *rom : roms) {
        // Try to reallocate all roms
        if (!allocate(rom)) {
            // Free the ones we couldn't allocate
            delete rom;
        }
    }
}

bool AllocationController::allocate(RomInfo *rom)
{
    if (m_allocator->allocate(rom)) {
        m_romsModel->addRom(rom);
        m_allocatedRoms.append(rom);
        return true;
    }

    return false;
}

void AllocationController::remove(int romIndex)
{
    if (romIndex < 0 || romIndex > m_allocatedRoms.count()) {
        return;
    }

    m_romsModel->removeRom(romIndex);
    delete m_allocatedRoms.takeAt(romIndex);

    reallocateAll();
}
