cmake_minimum_required(VERSION 3.10)
project(Simulation)

set(CMAKE_CXX_STANDARD 20)

cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)

add_library(glad vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC vendor/glad/include)

file(GLOB IMGUI_SRC
        vendor/imgui/*.cpp
        vendor/imgui/backends/imgui_impl_opengl3.cpp
        vendor/imgui/backends/imgui_impl_glfw.cpp
)

add_library(imgui ${IMGUI_SRC})
target_include_directories(imgui PUBLIC vendor/imgui vendor/imgui/backends)

add_executable(Simulation
        main.cpp
        src/shader.cpp
        src/viewport.cpp
)

target_include_directories(Simulation PUBLIC vendor)
target_include_directories(Simulation PUBLIC vendor/glad/include)

find_package(OpenGL REQUIRED)

target_link_libraries(Simulation
        glad
        imgui
        glfw
        OpenGL::GL
        dl
)
