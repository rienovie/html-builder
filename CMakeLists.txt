cmake_minimum_required(VERSION 3.27)
project(html-builder)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    main.cpp
)

add_library(imgui
    UI/imgui.cpp
    UI/imgui_demo.cpp
    UI/imgui_draw.cpp
    UI/imgui_widgets.cpp
    UI/imgui_stdlib.cpp
    UI/imgui_tables.cpp
    UI/backends/imgui_impl_glfw.cpp
    UI/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    UI
    UI/backends
)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(html-builder ${SOURCES})


target_link_libraries(html-builder imgui OpenGL::GL glfw)
