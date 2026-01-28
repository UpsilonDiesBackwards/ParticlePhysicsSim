cmake_minimum_required(VERSION 3.10)
project(Simulation)

set(CMAKE_CXX_STANDARD 20)

cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)

# ---- glad ----
add_library(glad
        vendor/glad/src/glad.c
)
target_include_directories(glad PUBLIC
        vendor/glad/include
)

# ---- imgui ----
file(GLOB IMGUI_SRC
        vendor/imgui/*.cpp
        vendor/imgui/backends/imgui_impl_opengl3.cpp
        vendor/imgui/backends/imgui_impl_glfw.cpp
)

add_library(imgui ${IMGUI_SRC})
target_include_directories(imgui PUBLIC
        vendor/imgui
        vendor/imgui/backends
)

# ---- executable ----
add_executable(Simulation
        main.cpp
        src/shader.cpp
        src/viewport.cpp
        src/io/input.cpp
        include/io/input.h
        src/io/inputmanager.cpp
        include/io/inputmanager.h
        src/application.cpp
        include/application.h
        src/time.cpp
        include/time.h
        include/sim/system.h
        include/sim/particletypes.h
        src/sim/particle.cpp
        src/sim/particle.cpp
        include/sim/particle.h
        src/sim/system.cpp
)

target_include_directories(Simulation PUBLIC
        vendor
        vendor/glad/include
)

# ---- OpenGL ----
find_package(OpenGL REQUIRED)

target_link_libraries(Simulation
        glad
        imgui
        glfw
        OpenGL::GL
        dl
)