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
