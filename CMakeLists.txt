cmake_minimum_required(VERSION 3.27)
project(html-builder)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

set(SOURCES
    main.cpp
    UI/UI.cpp
    sys/util.cpp
    sys/config.cpp
)

add_library(imgui
    UI/ImGui/imgui.cpp
    UI/ImGui/imgui_demo.cpp
    UI/ImGui/imgui_draw.cpp
    UI/ImGui/imgui_widgets.cpp
    UI/ImGui/imgui_stdlib.cpp
    UI/ImGui/imgui_tables.cpp
    UI/ImGui/backends/imgui_impl_glfw.cpp
    UI/ImGui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    UI/ImGui
    UI/ImGui/backends
)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(html-builder ${SOURCES})

target_link_libraries(html-builder imgui OpenGL::GL glfw)
