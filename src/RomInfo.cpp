#include "RomInfo.h"

RomInfo::RomInfo(QObject *parent) :
    QObject(parent)
{
    m_checksumValid = false;
}

RomInfo::~RomInfo()
{
}

QString RomInfo::title()
{
    return m_title;
}

bool RomInfo::isChecksumValid()
{
    return m_checksumValid;
}
