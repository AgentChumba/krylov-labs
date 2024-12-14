cmake_minimum_required(VERSION 3.28.3)

project(krilov_lab_2 CXX C)

list(APPEND CMAKE_MODULE_PATH F:/qtwindows/6.3.2/msvc2019_64/cmake/)
list(APPEND CMAKE_PREFIX_PATH F:/qtwindows/6.3.2/msvc2019_64/)



find_package(
  Qt6
  COMPONENTS Core Widgets Gui
  REQUIRED)

file(GLOB_RECURSE headers CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_LIST_DIR}/include/*")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*")

add_executable(lab_3 ${headers} ${sources})

target_link_libraries(lab_3 PRIVATE Qt::Core Qt::Widgets Qt::Gui)
