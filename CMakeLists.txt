cmake_minimum_required(VERSION 3.16)
project(G-Man VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Options ---
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(ASSIMP_BUILD_TESTS "Build Assimp tests" OFF)
option(ASSIMP_BUILD_ASSIMP_TOOLS "Build Assimp tools" OFF)
option(GLFW_BUILD_EXAMPLES "Build GLFW examples" OFF)
option(GLFW_BUILD_TESTS "Build GLFW tests" OFF)
option(GLFW_BUILD_DOCS "Build GLFW docs" OFF)

# --- Dependencies ---
add_subdirectory(external/glfw)
add_subdirectory(external/glew)
add_subdirectory(external/glm)
add_subdirectory(external/assimp)
add_subdirectory(external/spdlog)

# --- ImGui (No CMakeLists.txt in repo, so we build it here) ---
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)
add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)
target_link_libraries(imgui PUBLIC glfw libglew_static) # ImGui needs GLFW and OpenGL (via GLEW or system)

# --- STB Image (Header only) ---
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/external/stb)

# --- Main Executable ---
add_executable(G-Man 
    src/main.cpp
    src/Shader.cpp
    src/Camera.cpp
    src/Mesh.cpp
    src/Model.cpp
    src/Scene.cpp
    src/stb_impl.cpp
)

target_include_directories(G-Man PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
)

target_link_libraries(G-Man PRIVATE
    glfw
    libglew_static
    glm::glm
    assimp::assimp
    spdlog::spdlog
    imgui
    stb
)

# Copy assets to build directory
add_custom_command(TARGET G-Man POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:G-Man>/assets
)
