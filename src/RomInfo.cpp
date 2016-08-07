#include "RomInfo.h"

RomInfo::RomInfo(QObject *parent) :
    QObject(parent)
{
}

RomInfo::~RomInfo()
{
}

QString RomInfo::title()
{
    return m_title;
}
