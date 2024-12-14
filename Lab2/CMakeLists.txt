cmake_minimum_required(VERSION 3.28.3)

project(krilov_lab_2 CXX C)



find_package(
  Qt6
  COMPONENTS Core Widgets Gui
  REQUIRED)

add_library(qcustomplot SHARED IMPORTED)
set_target_properties(qcustomplot PROPERTIES 
                                  INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/
                                  IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/QCustomPlot/qcustomplotd2.dll
                                  IMPORTED_IMPLIB ${CMAKE_SOURCE_DIR}/QCustomPlot/qcustomplotd2.lib)

file(GLOB_RECURSE headers CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_LIST_DIR}/include/*")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*")

add_executable(lab_2 ${headers} ${sources})

target_link_libraries(lab_2 PRIVATE qcustomplot Qt::Core Qt::Widgets Qt::Gui)
