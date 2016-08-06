project(ems-qart CXX)

cmake_minimum_required(VERSION 3.1)

find_package(Qt5Core REQUIRED)
find_package(Qt5Quick REQUIRED)

add_definitions(-DQT_NO_CAST_FROM_ASCII -DQT_USE_QSTRINGBUILDER)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(ems-qart-sources src/main.cpp)
set(ems-qart-resources resources.qrc)

add_executable(ems-qart ${ems-qart-sources} ${ems-qart-resources})

qt5_use_modules(ems-qart Core Quick)

install(TARGETS ems-qart DESTINATION bin)
