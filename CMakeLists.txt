cmake_minimum_required(VERSION 3.10.0)
project(SoftwareRenderer VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

file(GLOB_RECURSE SOFTWARE_RENDERER_SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(SoftwareRenderer ${SOFTWARE_RENDERER_SOURCES})

target_include_directories(SoftwareRenderer PRIVATE include)
target_link_libraries(SoftwareRenderer PRIVATE SDL3::SDL3)

set_target_properties(SoftwareRenderer PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
