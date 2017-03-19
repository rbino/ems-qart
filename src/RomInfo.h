#ifndef _ROM_INFO_H
#define _ROM_INFO_H

#include <QObject>

namespace RomConstants {
    const int HeaderSize = 512;
    const int SmallestRomSize = 32768;
    const int LogoOffset = 0x104;
    const int TitleOffset = 0x134;
    const int CGBFlagOffset = 0x143;
    const int SGBFlagOffset = 0x146;
    const int ROMSizeOffset = 0x148;
    const int SRAMSizeOffset = 0x149;
    const int RegionOffset = 0x14a;
    const int OldLicenseOffset = 0x14b;
    const int RomVersionOffset = 0x14c;
    const int ChecksumOffset = 0x14d;
}

class RomInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(bool checksumValid READ isChecksumValid NOTIFY checksumValidChanged)
    Q_PROPERTY(int romSize READ romSize NOTIFY romSizeChanged)

    public:
        RomInfo(QObject *parent = nullptr);
        RomInfo(const QByteArray &header, QObject *parent = nullptr);

        virtual ~RomInfo();

        void updateInfo(const QByteArray &header);
        void resetInfo();

        QString title();
        bool isChecksumValid();
        int romSize();

    signals:
        void titleChanged(QString title);
        void checksumValidChanged(bool checksumValid);
        void romSizeChanged(int romSize);

    private:
        QString m_title;
        bool m_checksumValid;
        int m_romSize;
};

#endif
