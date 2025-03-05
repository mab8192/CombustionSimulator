cmake_minimum_required(VERSION 3.10)
project(RocketEngineSimulation)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

# Fetch GLFW and ImGui
include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.89.9
)
FetchContent_MakeAvailable(glfw imgui)

# ImGui source files
set(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

# Application source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "glad.c")
file(GLOB_RECURSE HEADERS "src/*.hpp" "src/*.h")

# Executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${IMGUI_SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    "include"
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE glfw OpenGL::GL)
