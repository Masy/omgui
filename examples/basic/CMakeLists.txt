link_libraries(omgui)
add_executable(omgui_basic main.cpp)
target_include_directories(omgui_basic PRIVATE ../../src/)

if(${OMG_BUILD_SHARED} STREQUAL "ON")
    if (MSVC)
        set(DLL_NAME "omgui.dll")
    else()
        set(DLL_NAME "libomgui.dll")
    endif()

    add_custom_command(TARGET omgui_basic POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_BINARY_DIR}/../../${DLL_NAME}
            ${CMAKE_CURRENT_BINARY_DIR}/${DLL_NAME})
endif()
