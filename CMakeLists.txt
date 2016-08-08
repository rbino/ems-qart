project(ems-qart CXX)

cmake_minimum_required(VERSION 3.1)

find_package(Qt5Core REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Widgets REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_search_module(LIBUSB1 REQUIRED libusb-1.0)

add_definitions(-DQT_NO_CAST_FROM_ASCII -DQT_USE_QSTRINGBUILDER)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(ems-qart-sources src/main.cpp src/EmsCart.cpp src/CartController.cpp
    src/RomInfo.cpp)
set(ems-qart-resources resources.qrc)

include_directories(${LIBUSB1_INCLUDE_DIRS})

add_executable(ems-qart WIN32 ${ems-qart-sources} ${ems-qart-resources})

qt5_use_modules(ems-qart Core Quick Widgets)
target_link_libraries(ems-qart ${LIBUSB1_LIBRARIES})

install(TARGETS ems-qart DESTINATION bin)
