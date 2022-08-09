cmake_minimum_required(VERSION 3.5.1)
project(typhoon_callback)

set(TARGET_NAME ${PROJECT_NAME})

add_executable(${TARGET_NAME}_runner main.cc)

target_link_libraries(${TARGET_NAME}_runner
  typhoon 
)

