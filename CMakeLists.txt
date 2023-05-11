cmake_minimum_required(VERSION 3.24)
project(cube C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)

set(GLFW_LIBRARY_TYPE STATIC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

find_package(OpenGL REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/glad/include)
file(GLOB BUTTERFLIES_SOURCES_C ${CMAKE_CURRENT_SOURCE_DIR} *.c glad/src/gl.c)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glfw)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glm)

set(CUBE_LIB_HEADERS
    lib/glad/include/glad/gl.h
    lib/glad/src/gl.c
    lib/glfw/include/GLFW/glfw3.h
    lib/glm/glm/glm.hpp
    lib/glm/glm/gtc/matrix_transform.hpp
    lib/glm/glm/gtc/type_ptr.hpp
    lib/glm/glm/gtx/string_cast.hpp)

set(CUBE_HEADERS
    src/engine/Shader.h
    src/engine/Camera.h
    )
set(CUBE_SOURCES
    src/main.cpp
    src/callbacks.h
    src/engine/primitives/RubikAtomCube.h)

add_executable(cube ${CUBE_LIB_HEADERS} ${CUBE_HEADERS} ${CUBE_SOURCES} ${BUTTERFLIES_SOURCES_C})

target_link_libraries(cube glfw OpenGL::GL glm::glm)
