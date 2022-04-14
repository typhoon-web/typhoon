cmake_minimum_required(VERSION 3.5.1)
project(typhoon VERSION 1.0.1)

set(CMAKE_CXX_STANDARD 14)
set(TARGET_NAME ${PROJECT_NAME})

include(CMakePackageConfigHelpers)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)
include(GNUInstallDirs)
include(CTest)
include("${CMAKE_CURRENT_LIST_DIR}/cmake/typhoon.cmake")
set(PROJECT_DESCRIPTION "trunk hdmap_engine")


option (BUILD_SHARED_LIBS "Build shared libraries" OFF)

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

if(NOT BUILD_SHARED_LIBS)
  add_library(${TARGET_NAME} STATIC
    src/handler.cc
    typhoon.cc
  )
else(NOT BUILD_SHARED_LIBS)
 add_library(${TARGET_NAME} SHARED
    src/handler.cc
    typhoon.cc
  )
endif (NOT BUILD_SHARED_LIBS)


target_link_libraries(${TARGET_NAME}
  ${Civetweb_LIBRARIES}
  ${CivetwebCpp_LIBRARIES}
  # ${NlohmannJson_LIBRARIES}
)

add_subdirectory(example)


##################################################################################
# config
##################################################################################
install(TARGETS ${TARGET_NAME}
    EXPORT ${TARGET_NAME}-targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY src/
    DESTINATION include/typhoon/src
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
)

install(FILES typhoon.h
    DESTINATION include/typhoon/
)

configure_package_config_file(
    "cmake/${TARGET_NAME}-config.cmake.in"
    "cmake/${TARGET_NAME}-config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/${TARGET_NAME}"
)

write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/cmake/${TARGET_NAME}-config-version.cmake"
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_file(
  cmake/${TARGET_NAME}.pc.in
  ${PROJECT_BINARY_DIR}/${TARGET_NAME}.pc 
  @ONLY
)

install(EXPORT  ${TARGET_NAME}-targets
    DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/${TARGET_NAME}"
)

install(FILES
    "${PROJECT_BINARY_DIR}/cmake/${TARGET_NAME}-config.cmake"
    "${PROJECT_BINARY_DIR}/cmake/${TARGET_NAME}-config-version.cmake"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/${TARGET_NAME}"
)

install(FILES "${PROJECT_BINARY_DIR}/${TARGET_NAME}.pc"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/pkgconfig"
)

