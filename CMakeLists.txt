cmake_minimum_required(VERSION 3.28)
project(Testbencher LANGUAGES CXX VERSION 0.0.1)

find_package(verilator REQUIRED)
find_package(SFML 3.0.2 COMPONENTS Graphics QUIET)

if(NOT SFML_FOUND)
    message(STATUS "Cannot find SFML on system, fetching from github")

    include(FetchContent)
    FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
    FetchContent_MakeAvailable(SFML)
else()
    message(STATUS "Found SFML on system")
endif()

add_library(Testbencher 
    src/Display.cpp 
    src/Testbench.cpp
)
target_include_directories(Testbencher PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    ${VERILATOR_ROOT}/include
    ${VERILATOR_ROOT}/include/vltstd
)
add_library(Testbencher::Testbencher ALIAS Testbencher)

target_compile_features(Testbencher PUBLIC cxx_std_17)
target_link_libraries(Testbencher PUBLIC SFML::Graphics)