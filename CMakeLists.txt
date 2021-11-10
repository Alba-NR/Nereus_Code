cmake_minimum_required(VERSION 3.0)
project(Nereus)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(libs/vendor/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(libs/vendor/assimp)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(libs/vendor/assimp/include/
        libs/vendor/glad/include/
        libs/vendor/glfw/include/
        libs/vendor/glm/
        libs/vendor/stb/
        libs/vendor/imgui/  # bc imgui backend files must refer to "imgui.h"
        libs/vendor/)       # for imgui files being imgui/... rather than just the header file

set(VENDORS_SOURCES libs/vendor/glad/src/glad.c
                    libs/vendor/stb/stb_image.h
                    libs/vendor/stb/stb_image_write.h
                    libs/vendor/imgui/imgui.cpp
                    libs/vendor/imgui/imgui.h
                    libs/vendor/imgui/imgui_internal.h
                    libs/vendor/imgui/imgui_draw.cpp
                    libs/vendor/imgui/imgui_widgets.cpp
                    libs/vendor/imgui/imgui_tables.cpp
                    libs/vendor/imgui/backends/imgui_impl_glfw.h
                    libs/vendor/imgui/backends/imgui_impl_glfw.cpp
                    libs/vendor/imgui/backends/imgui_impl_opengl3.h
                    libs/vendor/imgui/backends/imgui_impl_opengl3.cpp)

set(PROJECT_HEADERS src/main/nereus.h
                    src/main/constants.h
                    src/main/app_context.h
                    src/utils/image_io.h
                    src/ui/ui.h
                    src/graphics/buffers.h
                    src/graphics/shaders.h
                    src/graphics/camera.h
                    src/graphics/textures.h
                    src/graphics/core/window.h
                    src/graphics/meshes/meshes.h
                    src/graphics/meshes/test_mesh.h
                    src/graphics/meshes/ocean_mesh.h
                    src/graphics/renderers/renderers.h)

set(PROJECT_SOURCES src/main/nereus.cpp
                    src/main/app_context.cpp
                    src/utils/image_io.cpp
                    src/ui/ui.cpp
                    src/graphics/buffers.cpp
                    src/graphics/shaders.cpp
                    src/graphics/camera.cpp
                    src/graphics/textures.cpp
                    src/graphics/core/window.cpp
                    src/graphics/meshes/meshes.cpp
                    src/graphics/meshes/test_mesh.cpp
                    src/graphics/meshes/ocean_mesh.cpp
                    src/graphics/renderers/renderers.cpp)

set(PROJECT_SHADERS resources/shaders/test.frag
                    resources/shaders/test.vert)

set(PROJECT_CONFIGS CMakeLists.txt
                    .gitattributes
                    .gitignore
                    .gitmodules)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${VENDORS_SOURCES})
source_group("Shaders" FILES ${PROJECT_SHADERS})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})
target_link_libraries(${PROJECT_NAME} assimp glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/resources/shaders" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        DEPENDS ${PROJECT_SHADERS})