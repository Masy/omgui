enable_testing()
include(CTest)

add_executable(omgui_tests
        TestMain.cpp
TestEvents.cpp)

target_include_directories(omgui_tests PRIVATE deps/doctest ../src)
target_link_libraries(omgui_tests omgui)
