cmake_minimum_required(VERSION 3.21)
project(omgui VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
option(OMG_BUILD_SHARED "Build the library as shared library" ON)
option(OMG_BUILD_EXAMPLES "Build the OMG example programs" ON)

if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

if(${OMG_BUILD_SHARED} STREQUAL "ON")
    set(OMG_LIBRARY_TYPE "SHARED")
else()
    set(OMG_LIBRARY_TYPE "STATIC")
endif()

file(GLOB_RECURSE HEADERS src/*.hpp)
file(GLOB_RECURSE SOURCES src/*.cpp)

add_library(omgui ${OMG_LIBRARY_TYPE} ${HEADERS} ${SOURCES})

target_include_directories(omgui PUBLIC include)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /W3 /WX")
    if (${CMAKE_BUILD_TYPE} STREQUAL "DEBUG")
        target_compile_definitions(omgui PUBLIC DLL_EXPORT_SYMBOLS)
    endif ()
endif ()

if(OMG_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

file(COPY ${HEADERS} DESTINATION ${CMAKE_SOURCE_DIR}/include/omg)
