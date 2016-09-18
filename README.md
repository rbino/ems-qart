# ems-qart
A cross-platform Qt application to flash EMS 64M USB cartridges

[![Build Status](https://travis-ci.org/rbino/ems-qart.svg?branch=master)](https://travis-ci.org/rbino/ems-qart)

![alt text](https://i.imgur.com/UU4yUpi.png)

Currently working on Linux, OSX and Windows (up to Windows 10)

## Features
- Read ROMs from both banks
- Write a single ROM to one bank
- Read and write SRAM
- Working on Windows 10 and 64 bit Windows
- Includes a signed driver for Windows, no need to disable driver signing

## Linux installation
If you're on Debian (Jessie or later) or Ubuntu (15.10 or later), go to the [latest release](https://github.com/rbino/ems-qart/releases/latest) and download the .deb package

Install it with
```
sudo dpkg -i packagename.deb
sudo apt -f install
```
Then type `ems-qart` in a terminal to start the program

If you're not on a deb based distro see "Building on Linux"

## Windows installation
Go to the [latest release](https://github.com/rbino/ems-qart/releases/latest) and download the zip for Windows

Unzip it then follow the instructions written in README.txt

## Mac OSX installation
Go to the [latest release](https://github.com/rbino/ems-qart/releases/latest) and download the dmg for Mac, open the dmg and drag ems-qart in your Applications folder

The first time you open the app, right-click on it and select `Open` from the menu. An alert will appear, then click `Open`.

## Building on Linux
Install the dependencies

On Debian/Ubuntu
```
sudo apt install build-essential pkg-config qtbase5-dev qtdeclarative5-dev libqt5svg5-dev qt5-qmake qt5-default libusb-1.0-0-dev qml-module-qtquick2 qml-module-qtquick-controls qml-module-qtquick-dialogs
```

Then navigate to the folder where you cloned the repo and
```
mkdir build
cd build
qmake ..
make
sudo make install
```
