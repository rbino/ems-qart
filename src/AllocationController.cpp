#include "AllocationController.h"

#include "EmsCart.h"
#include "RomInfo.h"
#include "RomListModel.h"

AllocationController::AllocationController(QObject *parent)
    : QObject(parent)
    , m_romsModel(new RomListModel())
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
}
