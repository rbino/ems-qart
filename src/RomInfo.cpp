#include "RomInfo.h"

RomInfo::RomInfo(QObject *parent) :
    QObject(parent)
{
    m_checksumValid = false;
    m_romSize = 0;
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

    if (m_checksumValid) {
        int sizeCode = header.at(ROMSizeOffset);
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

    } else {
        m_romSize = 0;
    }
    emit romSizeChanged(m_romSize);
}

void RomInfo::resetInfo()
{
    m_title = QString();
    emit titleChanged(m_title);

    m_checksumValid = false;
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

int RomInfo::romSize()
{
    return m_romSize;
}
