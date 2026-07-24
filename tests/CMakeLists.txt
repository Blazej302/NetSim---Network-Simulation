include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(unit_tests
        tests_PackageAndStorage.cpp
        tests_nodes.cpp
)

target_sources(unit_tests PRIVATE
        ../src/Package.cpp
        ../src/storage_types.cpp
        ../src/helpers.cpp
        ../src/Ramp.cpp
        ../src/receiver_types.cpp
        ../src/storage_types.cpp
        ../src/Worker.cpp
)

target_include_directories(unit_tests PRIVATE ../include)

target_link_libraries(unit_tests PRIVATE gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)