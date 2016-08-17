#ifndef _ROM_INFO_H
#define _ROM_INFO_H

#include <QObject>

class RomInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(bool checksumValid READ isChecksumValid NOTIFY checksumValidChanged)
    Q_PROPERTY(int romSize READ romSize NOTIFY romSizeChanged)

    Q_ENUMS(RomHeaderOffsets)

    public:
        enum RomHeaderOffsets {
            LogoOffset = 0x104,
            TitleOffset = 0x134,
            CGBFlagOffset = 0x143,
            SGBFlagOffset = 0x146,
            ROMSizeOffset = 0x148,
            SRAMSizeOffset = 0x149,
            RegionOffset = 0x14a,
            OldLicenseOffset = 0x14b,
            RomVersionOffset = 0x14c,
            ChecksumOffset = 0x14d
        };

        RomInfo(QObject *parent = nullptr);

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
