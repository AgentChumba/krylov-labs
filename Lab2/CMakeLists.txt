cmake_minimum_required(VERSION 3.28.3)

project(krilov_lab_2 CXX C)

list(APPEND CMAKE_MODULE_PATH F:/qtwindows/6.3.2/msvc2019_64/cmake/)
list(APPEND CMAKE_PREFIX_PATH F:/qtwindows/6.3.2/msvc2019_64/)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(
  Qt5
  COMPONENTS Core Widgets Gui
  REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE headers CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_LIST_DIR}/include/*")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*")

add_executable(lab_2 ${headers} ${sources})

target_link_libraries(lab_2 PRIVATE usb-1.0 Qt::Core Qt::Widgets Qt::Gui
                                    qcustomplot-qt5)
