cmake_minimum_required(VERSION 3.16)

project(opengl_from_scratch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")

include_directories(${CMAKE_SOURCE_DIR}/includes)
link_directories(${CMAKE_SOURCE_DIR}/libs)

find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
find_package(OpenGL REQUIRED)
message(STATUS "Found OpenGL in ${OPENGL_INCLUDE_DIR}")
find_package(X11 REQUIRED)
message(STATUS "Found X11 in ${X11_INCLUDE_DIR}")

add_library(GLAD "${CMAKE_SOURCE_DIR}/src/glad.c")

set(LIBS ${GLFW3_LIBRARY} X11 GL GLAD dl pthread)

add_executable(ofs_window src/hello_window.cpp)
target_link_libraries(ofs_window ${LIBS})

add_executable(ofs_triangle src/triangle.cpp)
target_link_libraries(ofs_triangle ${LIBS})

add_executable(ofs_rectangle src/rectangle.cpp)
target_link_libraries(ofs_rectangle ${LIBS})

add_executable(ofs_shader_normal src/shader_normal.cpp)
target_link_libraries(ofs_shader_normal ${LIBS})

add_executable(ofs_shader_uniform src/shader_uniform.cpp)
target_link_libraries(ofs_shader_uniform ${LIBS})

add_executable(ofs_shader_location src/shader_location.cpp)
target_link_libraries(ofs_shader_location ${LIBS})
