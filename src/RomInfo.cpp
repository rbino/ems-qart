#include "RomInfo.h"

RomInfo::RomInfo(QObject *parent) :
    QObject(parent)
{
    m_checksumValid = false;
}

RomInfo::~RomInfo()
{
}

void RomInfo::updateInfo(const QByteArray &header)
{
    // Title max length is 16
    m_title = QLatin1String(header.mid(TitleOffset, 16));
    emit titleChanged(m_title);

    uint8_t calculatedChecksum = 0;
    for (int i = TitleOffset; i < ChecksumOffset; i++)
    {
        calculatedChecksum -= (uint8_t)header.at(i) + 1;
    }
    if (calculatedChecksum != (uint8_t)header.at(ChecksumOffset)) {
        m_checksumValid = false;
    } else {
        m_checksumValid = true;
    }
    emit checksumValidChanged(m_checksumValid);
}

QString RomInfo::title()
{
    return m_title;
}

bool RomInfo::isChecksumValid()
{
    return m_checksumValid;
}
