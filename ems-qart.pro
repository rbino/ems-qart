QT += quick widgets svg

CONFIG += c++11 windows release link_pkgconfig
PKGCONFIG += libusb-1.0

TARGET = ems-qart

HEADERS = src/CartController.h src/EmsCart.h src/RomInfo.h
SOURCES = src/main.cpp src/CartController.cpp src/EmsCart.cpp src/RomInfo.cpp
RESOURCES = resources.qrc
