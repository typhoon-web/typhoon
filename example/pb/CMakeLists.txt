cmake_minimum_required(VERSION 3.5.1)
project(typhoon_pb)

set(TARGET_NAME ${PROJECT_NAME})

find_package(PkgConfig REQUIRED)
pkg_check_modules(Protobuf REQUIRED protobuf)

include_directories(
  ${Protobuf_INCLUDE_DIRS}
)

link_directories(
  ${Protobuf_LIBRARY_DIRS}
)

add_executable(${TARGET_NAME}_runner main.cc)

target_link_libraries(${TARGET_NAME}_runner
  ${Protobuf_LIBRARIES}
  typhoon
)

