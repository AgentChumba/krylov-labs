cmake_minimum_required(VERSION 3.28.3)

project(krylov_labs_6 CXX)

find_package(
  Qt6
  COMPONENTS Gui Core Widgets SerialPort
  REQUIRED)



add_executable(
  lab_6
  ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/MainWindow.cpp
  ${CMAKE_CURRENT_LIST_DIR}/include/MainWindow.h)
target_include_directories(lab_6 PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include
                                         ${wolfram_compiler_additions})
target_link_libraries(lab_6 PRIVATE Qt::Core Qt::Gui Qt::Widgets qcustomplot
                                    Qt::SerialPort)
