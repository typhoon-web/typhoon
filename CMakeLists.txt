cmake_minimum_required(VERSION 3.5.1)
project(typhoon)

set(CMAKE_CXX_STANDARD 14)

set(TARGET_NAME ${PROJECT_NAME})

find_package (PkgConfig REQUIRED)
pkg_check_modules(Civetweb REQUIRED civetweb)
pkg_check_modules(CivetwebCpp REQUIRED civetweb-cpp)
# pkg_check_modules(NlohmannJson REQUIRED nlohmann_json)

include_directories(
  ${Civetweb_INCLUDE_DIRS}
  ${CivetwebCpp_INCLUDE_DIRS}
  # ${NlohmannJson_INCLUDE_DIRS}
)

link_directories (
  ${Civetweb_LIBRARY_DIRS}
  ${CivetwebCpp_LIBRARY_DIRS}
  # ${NlohmannJson_LIBRARY_DIRS}
)

# add_library(${TARGET_NAME} SHARED
add_library(${TARGET_NAME} STATIC
  src/handler.cc
  typhoon.cc
)

target_link_libraries(${TARGET_NAME}
  ${Civetweb_LIBRARIES}
  ${CivetwebCpp_LIBRARIES}
  # ${NlohmannJson_LIBRARIES}
)

add_subdirectory(example)

