cmake_minimum_required(VERSION 3.10)
project(opengl_learn)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/glad.c
)

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME}
    glfw
    OpenGL::GL
    ${CMAKE_DL_LIBS}
)