# ems-qart
A Qt application to flash EMS 64M cartridges

## Linux installation
Go to the [latest release](https://github.com/rbino/ems-qart/releases/latest) and download the .deb package

Install it with
```
dpkg -i packagename.deb
sudo apt -f install
```
Then type `ems-qart` in a terminal to start the program

## Windows installation
Go to the [latest release](https://github.com/rbino/ems-qart/releases/latest) and download the zip for Windows

Unzip it then follow the instructions written in README.txt

## Mac OSX installation
Coming soon

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
