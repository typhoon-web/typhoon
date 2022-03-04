cmake_minimum_required(VERSION 3.5.1)

project(typhoon)

set(CMAKE_CXX_STANDARD 14)

set(TARGET_NAME ${PROJECT_NAME})

find_package (PkgConfig REQUIRED)
pkg_check_modules(NLOHMANN_JSON REQUIRED nlohmann_json)

include_directories(
    ${NLOHMANN_JSON_INCLUDE_DIRS}
)

link_directories (
    ${NLOHMANN_JSON_LIBRARY_DIRS}
)

add_subdirectory(example)




