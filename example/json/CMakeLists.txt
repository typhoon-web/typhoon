cmake_minimum_required(VERSION 3.5.1)
project(typhoon_json)

set(TARGET_NAME ${PROJECT_NAME})

find_package(PkgConfig REQUIRED)
pkg_check_modules(Nlohmann_json REQUIRED nlohmann_json)

include_directories(
    ${Nlohmann_json_INCLUDE_DIRS}
)

link_directories (
    ${Nlohmann_json_LIBRARY_DIRS}
)

add_executable(${TARGET_NAME}_runner main.cc)

target_link_libraries(${TARGET_NAME}_runner
  typhoon 
  ${Nlohmann_json_LIBRARIES}
)

