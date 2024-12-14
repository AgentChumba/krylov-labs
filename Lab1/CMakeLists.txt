cmake_minimum_required(VERSION 3.28.3)

project(krilov_lab_1 CXX C)

find_package(
  Qt6
  COMPONENTS Core Widgets Gui SerialPort
  REQUIRED)

file(GLOB_RECURSE headers CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_LIST_DIR}/include/*")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*")

add_executable(lab_1 ${headers} ${sources})

target_link_libraries(lab_1 PRIVATE Qt::Core Qt::Widgets Qt::Gui
                                    Qt::SerialPort)
