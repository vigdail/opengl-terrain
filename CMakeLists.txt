cmake_minimum_required(VERSION 3.0)

project (TerrainGL)

set(OpenGL_GL_PREFERENCE "GLVND")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

link_directories(${CMAKE_SOURCE_DIR}/lib)

list(APPEND CMAKE_CXX_FLAGS "-std=c++17")

find_package(glfw3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
find_package(OpenGL REQUIRED)
add_definitions(${OPENGL_DEFINITIONS})
find_package(X11 REQUIRED)
find_package(Freetype REQUIRED)

set(LIBS glfw X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread freetype)

add_library(stb_image "lib/src/stb_image.cpp")
target_include_directories(stb_image PRIVATE "lib/include/")
set(LIBS ${LIBS} stb_image)

add_library(glad "lib/src/glad.c")
target_include_directories(glad PRIVATE "lib/include/")
set(LIBS ${LIBS} glad)

add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD)
file(GLOB IMGUI_SOURCE "lib/imgui/*.cpp")
add_library(imgui ${IMGUI_SOURCE})
target_include_directories(imgui PRIVATE "lib/imgui/" "lib/include/")
set(LIBS ${LIBS} imgui)

file(GLOB SOURCE "src/**/*.cpp" "src/*.cpp")

set(APP "app")

add_executable(${APP} ${SOURCE})
target_include_directories(${APP} 
    PRIVATE "lib/include/" 
    PRIVATE "lib/" 
    PRIVATE ${FREETYPE_INCLUDE_DIRS})

set_target_properties(${APP} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

target_link_libraries(${APP} ${LIBS})
