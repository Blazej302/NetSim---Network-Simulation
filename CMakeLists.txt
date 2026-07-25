cmake_minimum_required(VERSION 3.30)
project(NetSim CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

include_directories(include)

add_subdirectory(tests)

add_executable(NetSim
        src/main.cpp
        src/Factory.cpp
        src/helpers.cpp
        src/Package.cpp
        src/Ramp.cpp
        src/receiver_types.cpp
        src/storage_types.cpp
        src/Worker.cpp
)