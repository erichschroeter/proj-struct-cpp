cmake_minimum_required(VERSION 3.5)
project(proj-struct)

# Add cmake/Modules/ to the CMake module path to find Catch2 CMake scripts.
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")

add_subdirectory(include/catch2)
add_subdirectory(include/docopt)

add_library(proj-struct-lib
    src/proj-struct.cpp
    $<$<PLATFORM_ID:Windows>:
        src/platform_win.cpp>
    $<$<PLATFORM_ID:Linux>:
        src/platform_unix.cpp>
)

add_executable(proj-struct
    src/main.cpp
)
target_include_directories(proj-struct
    PRIVATE
        include
)
target_link_libraries(proj-struct
    proj-struct-lib
    docopt_s
)

add_executable(tests
    src/tests.cpp
    src/proj-struct_tests.cpp
)
target_include_directories(tests
    PRIVATE
        include
        src
)
target_link_libraries(tests
    proj-struct-lib
    Catch2::Catch2
)

include(CTest)
include(Catch)
catch_discover_tests(tests)
