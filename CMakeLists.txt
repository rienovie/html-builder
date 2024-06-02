cmake_minimum_required(VERSION 3.27)
project(html-builder)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(CURL REQUIRED)
find_package(Git REQUIRED)

if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR/.git}")
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
        #imgui branch needs to modify imgui.h to ../imgui.h on some file
        set(PATCH_FILE ${CMAKE_CURRENT_SOURCE_DIR}/sys/imgui.patch)
        if(EXISTS ${PATCH_FILE})
            message(STATUS "Applying imgui submodule header patch")
            execute_process(
                COMMAND ${GIT_EXECUTABLE} apply ${PATCH_FILE}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ImGui
                RESULT_VARIABLE GIT_PATCH_RESULT
            )
            if(NOT GIT_PATCH_RESULT EQUAL "0")
                message(FATAL_ERROR "Applying imgui submodule patch failed with ${GIT_PATCH_RESULT}")
            endif()
        else()
            message(WARNING "Imgui header patch file not found: ${PATCH_FILE}")
        endif()
    endif()
endif()

set(SOURCES
    main.cpp
    UI/UI.cpp
    sys/config.cpp
    UI/UI_Windows.cpp
    sys/html.cpp
    sys/fetch.cpp
)

#TODO look into submodule sparse-checkout so instead of getting the entire Imgui repo, get only what is needed
add_library(imgui
    ImGui/imgui.cpp
    ImGui/imgui_demo.cpp
    ImGui/imgui_draw.cpp
    ImGui/imgui_widgets.cpp
    ImGui/imgui_tables.cpp
    ImGui/backends/imgui_impl_glfw.cpp
    ImGui/backends/imgui_impl_opengl3.cpp
    ImGui/misc/cpp/imgui_stdlib.cpp
)

add_library(util
    Util/util.cpp
)

target_include_directories(imgui PUBLIC
    ImGui
    ImGui/backends
    ImGui/misc/cpp
)

target_include_directories(util PUBLIC
    Util
)


add_executable(html-builder ${SOURCES})

target_link_libraries(html-builder imgui OpenGL::GL glfw ${CURL_LIBRARIES} util)
