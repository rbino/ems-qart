QT += quick widgets svg concurrent

CONFIG += c++11 windows release link_pkgconfig
PKGCONFIG += libusb-1.0

TARGET = ems-qart

DEFINES += QT_NO_CAST_FROM_ASCII

HEADERS = src/CartController.h src/EmsCart.h src/RomInfo.h
SOURCES = src/main.cpp src/CartController.cpp src/EmsCart.cpp src/RomInfo.cpp
RESOURCES = resources.qrc

unix:target.path = /usr/bin/
INSTALLS += target
