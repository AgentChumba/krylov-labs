cmake_minimum_required(VERSION 3.28.3)

project(krylov_labs_5 CXX)

find_package(
  Qt5
  COMPONENTS Gui Core Widgets
  REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(wolfram_compiler_additions "/Your/Fucking/Path")
  add_library(wstp64i4 STATIC IMPORTED)
  set_target_properties(
    wstp64i4 PROPERTIES IMPORTED_LOCATION ${wolfram_compiler_additions}/wsdp64.a
                        INTERFACE_LINK_LIBRARIES ${wolfram_compiler_additions})
  add_library(wstp64i4m STATIC IMPORTED)
  set_target_properties(
    wstp64i4m
    PROPERTIES IMPORTED_LOCATION ${wolfram_compiler_additions}/wsdp64.a
               INTERFACE_LINK_LIBRARIES ${wolfram_compiler_additions})
  add_library(wstp64i4s STATIC IMPORTED)
  set_target_properties(
    wstp64i4s
    PROPERTIES IMPORTED_LOCATION ${wolfram_compiler_additions}/wsdp64.a
               INTERFACE_LINK_LIBRARIES ${wolfram_compiler_additions})

  add_library(wstp64 INTERFACE)
  target_link_libraries(wstp64 INTERFACE wstp64i4 wstp64i4m wstp64i4s)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(wolfram_compiler_additions
      /usr/local/Wolfram/Mathematica/13.2/SystemFiles/Links/WSTP/DeveloperKit/Linux-x86-64/CompilerAdditions/
  )
  add_library(wstp64 SHARED IMPORTED)
  set_target_properties(
    wstp64 PROPERTIES IMPORTED_LOCATION
                      ${wolfram_compiler_additions}/libWSTP64i4.so)
else()
  message(FATAL_ERROR "Unsupported platform")
endif()

add_executable(
  lab_5
  ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/MainWindow.cpp
  ${CMAKE_CURRENT_LIST_DIR}/include/MainWindow.h)
target_include_directories(lab_5 PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include
                                         ${wolfram_compiler_additions})
target_link_libraries(lab_5 PRIVATE wstp64 Qt::Core Qt::Gui Qt::Widgets
                                    qcustomplot-qt5)
