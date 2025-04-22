cmake_minimum_required(VERSION 3.16)

project(qcustomplot VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets PrintSupport Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets PrintSupport Gui)

message(${CMAKE_CURRENT_SOURCE_DIR})

add_library( qcustomplot SHARED
    qcustomplot.h qcustomplot.cpp
)

target_link_libraries(qcustomplot PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ws2_32 Qt6::PrintSupport Qt6::Gui)
