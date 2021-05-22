cmake_minimum_required(VERSION 3.5.1)

project(typhoon_server_demo)

set(TARGET_NAME ${PROJECT_NAME})

aux_source_directory(src DEMO_VIEW_SOURCE_DIRS)

add_library(${TARGET_NAME} SHARED
    ${DEMO_VIEW_SOURCE_DIRS}
)

target_link_libraries(${TARGET_NAME}
    typhoon
)

add_executable(${TARGET_NAME}_runner main.cc)

target_link_libraries(${TARGET_NAME}_runner
    ${TARGET_NAME}
)
