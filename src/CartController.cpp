#include "CartController.h"

#include "RomInfo.h"

#include <QDebug>
#include <QDir>

CartController::CartController(QObject *parent) :
    QObject(parent)
{
    m_emsCart = EmsCart::instance();
    connect(m_emsCart, &EmsCart::readyChanged, this, &CartController::readyChanged);
    connect(m_emsCart, &EmsCart::error, this, &CartController::emsErrorUpdate);
    connect(m_emsCart, &EmsCart::busyChanged, this, &CartController::busyChanged);
    connect(m_emsCart, &EmsCart::progressChanged, this, &CartController::progressChanged);
    connect(m_emsCart, &EmsCart::transferCompleted, this, &CartController::transferCompleted);
    m_emsCart->findDevice();
}

CartController::~CartController()
{
}

void CartController::refresh()
{
    m_emsCart->findDevice();
}

bool CartController::isReady() const
{
    return m_emsCart->ready();
}

bool CartController::isBusy() const
{
    return m_emsCart->busy();
}

double CartController::progress() const
{
    return m_emsCart->progress();
}

QString CartController::urlToLocalPath(const QUrl &fileUrl)
{
    return QDir::toNativeSeparators(fileUrl.toLocalFile());
}

void CartController::emsErrorUpdate(QString message)
{
    emit error(message);
}

void CartController::readROM(const QUrl &outFileUrl, int bank, int romIndex)
{
    m_emsCart->readCart(outFileUrl, EmsCart::ROM, bank, romIndex);
}

void CartController::readSRAM(const QUrl &outFileUrl)
{
    m_emsCart->readCart(outFileUrl, EmsCart::SRAM);
}

void CartController::writeROM(const QUrl &inFileUrl, int bank, int offset)
{
    m_emsCart->readCart(inFileUrl, EmsCart::ROM, bank, offset);
}

void CartController::writeSRAM(const QUrl &inFileUrl)
{
    m_emsCart->readCart(inFileUrl, EmsCart::SRAM);
}
