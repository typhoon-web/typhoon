cmake_minimum_required(VERSION 3.5.1)

project(typhoon)

set(CMAKE_CXX_STANDARD 14)

set(TARGET_NAME ${PROJECT_NAME})

find_package (PkgConfig REQUIRED)
find_package (Boost REQUIRED COMPONENTS system thread)
pkg_check_modules(NLOHMANNJSON REQUIRED nlohmann_json)

include_directories(
    include
    include/typhoon
    ${Boost_INCLUDE_DIRS}
    ${NLOHMANNJSON_INCLUDE_DIRS}
)

link_directories (
    ${Boost_LIBRARY_DIRS}
    ${NLOHMANNJSON_LIBRARY_DIRS}
)

aux_source_directory(include/typhoon TYPHOON_SOURCE_DIRS)

add_library(${TARGET_NAME} SHARED
    ${TYPHOON_SOURCE_DIRS}
)

target_link_libraries(${TARGET_NAME}
    ${Boost_LIBRARIES}
    ${NLOHMANNJSON_LIBRARIES}
)

add_subdirectory(example/demo)




