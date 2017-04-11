#include "RomInfo.h"

RomInfo::RomInfo(QObject *parent) :
    QObject(parent)
{
    m_romSize = 0;
    m_offset = -1;
}

RomInfo::RomInfo(const QByteArray &header, QObject *parent)
    : RomInfo(parent)
{
    updateInfo(header);
}

RomInfo::~RomInfo()
{
}

void RomInfo::updateInfo(const QByteArray &header)
{
    // Title max length is 16
    m_title = QLatin1String(header.mid(RomConstants::TitleOffset, 16));

    int sizeCode = header.at(RomConstants::ROMSizeOffset);
    switch (sizeCode) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            m_romSize = 32 << (10 + sizeCode);
            break;

        case 0x52:
            m_romSize = 1179648;
            break;

        case 0x53:
            m_romSize = 1310720;
            break;

        case 0x54:
            m_romSize = 1572864;
            break;

        default:
            m_romSize = 0;
    }
}

void RomInfo::resetInfo()
{
    m_title = QString();

    m_romSize = 0;
    m_offset = -1;
}

void RomInfo::setOffset(int offset)
{
    m_offset = offset;
}

QString RomInfo::title() const
{
    return m_title;
}

int RomInfo::romSize() const
{
    return m_romSize;
}

int RomInfo::offset() const
{
    return m_offset;
}
