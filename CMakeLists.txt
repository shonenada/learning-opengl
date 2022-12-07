cmake_minimum_required(VERSION 3.16)

project(opengl_from_scratch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

if(UNIX)
    set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
endif(UNIX)

set(CMAKE_BUILD_TYPE Debug)

include_directories(${CMAKE_SOURCE_DIR}/includes)
link_directories(${CMAKE_SOURCE_DIR}/libs)

if(WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/thirdparties/includes)

    add_library(GLAD "${CMAKE_SOURCE_DIR}/src/glad.c")
    add_library(STB_IMAGE "${CMAKE_SOURCE_DIR}/src/stb_image.cpp")
    set(LIBS ${CMAKE_SOURCE_DIR}/libs/glfw3.dll GLAD STB_IMAGE)

    file(COPY ${CMAKE_SOURCE_DIR}/libs/glfw3.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
endif(WIN32)

if(UNIX)
    find_package(GLFW3 REQUIRED)
    message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
    find_package(OpenGL REQUIRED)
    message(STATUS "Found OpenGL in ${OPENGL_INCLUDE_DIR}")
    find_package(X11 REQUIRED)
    message(STATUS "Found X11 in ${X11_INCLUDE_DIR}")

    add_library(GLAD "${CMAKE_SOURCE_DIR}/src/glad.c")
    add_library(STB_IMAGE "${CMAKE_SOURCE_DIR}/src/stb_image.cpp")

    set(LIBS ${GLFW3_LIBRARY} X11 GL GLAD STB_IMAGE dl pthread)
endif(UNIX)

function(add_project name)
    add_executable(ofs_${name} src/${name}.cpp)
    target_link_libraries(ofs_${name} ${LIBS})
endfunction()

add_executable(glm_demo src/glm_demo.cpp)
target_link_libraries(glm_demo ${LIBS})

add_project(hello_window)
add_project(triangle)
add_project(rectangle)
add_project(shader_normal)
add_project(shader_uniform)
add_project(shader_location)
add_project(shader_class)
add_project(texture)
add_project(transformations)
add_project(start_3d)
add_project(camera)
add_project(camera_movement)
add_project(camera_movement_look_around)
add_project(camera_movement_zoom)
add_project(camera_class)
add_project(lighting_color)
add_project(lighting_basic)
add_project(lighting_specular)
add_project(material)
add_project(diffuse_map)
add_project(specular_map)
add_project(lighting_casters_directional)
add_project(lighting_casters_point)
add_project(lighting_casters_spotlight)
add_project(lighting_casters_spotlight_softedges)
