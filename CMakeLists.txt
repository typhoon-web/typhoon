cmake_minimum_required(VERSION 3.5.1)

project(typhoon)

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread)
pkg_check_modules(NLOHMANNJSON REQUIRED nlohmann_json)




