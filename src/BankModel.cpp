#include "BankModel.h"

#include "RomInfo.h"

BankModel::BankModel(QObject *parent)
    : RomListModel(parent)
    , m_bank(EmsCart::InvalidBank)
{
}

BankModel::~BankModel()
{
}

EmsCart::Bank BankModel::bank() const
{
    return m_bank;
}

void BankModel::setBank(EmsCart::Bank bank)
{
    m_bank = bank;

    // Disconnect all signals
    disconnect(EmsCart::instance(), 0, this, 0);

    switch (bank) {
        case EmsCart::BankOne:
            connect(EmsCart::instance(), &EmsCart::bankOneChanged, this, &BankModel::refreshData);
            break;

        case EmsCart::BankTwo:
            connect(EmsCart::instance(), &EmsCart::bankTwoChanged, this, &BankModel::refreshData);
            break;

        default:
            // Avoid compiler whining
            break;
    }

    refreshData();
}

void BankModel::refreshData()
{

    QList<RomInfo *> roms;
    switch (m_bank) {
        case EmsCart::BankOne:
            roms = EmsCart::instance()->bankOne();
            break;

        case EmsCart::BankTwo:
            roms = EmsCart::instance()->bankTwo();
            break;

        default:
            return;
    }

    if (roms.isEmpty()) {
        return;
    }

    for (RomInfo *rom : roms) {
        addRom(rom);
    }
}
