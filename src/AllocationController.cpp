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
