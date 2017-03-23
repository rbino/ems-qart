QT += quick widgets svg concurrent

CONFIG += c++11 windows link_pkgconfig
PKGCONFIG += libusb-1.0

QT_CONFIG -= no-pkg-config

TARGET = ems-qart

DEFINES += QT_NO_CAST_FROM_ASCII

HEADERS = src/BankModel.h src/CartController.h src/EmsCart.h src/RomInfo.h
SOURCES = src/main.cpp src/BankModel.cpp src/CartController.cpp src/EmsCart.cpp src/RomInfo.cpp
RESOURCES = resources.qrc

RC_ICONS = images/ems-qart.ico
ICON = images/ems-qart.icns

isEmpty(PREFIX) {
    UDEV_PREFIX = /etc
    PREFIX = /usr/local
} else {
    UDEV_PREFIX = /lib
}

unix:udevrules.path = $$UDEV_PREFIX/udev/rules.d/
unix:udevrules.files = 50_ems_gb_flash.rules

unix:target.path = $$PREFIX/bin/
unix:INSTALLS += target udevrules
