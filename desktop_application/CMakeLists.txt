
cmake_minimum_required(VERSION 2.8)

project(desktop_application)


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/obj")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include_directories(include)

# find OpenGL
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message(ERROR " OpenGL not found! ")
endif(NOT OPENGL_FOUND)

find_package(GLEW REQUIRED)
if(NOT GLEW_FOUND)
    message(ERROR " GLEW not found! ")
endif(NOT GLEW_FOUND)

# find GLUT
find_package(GLUT REQUIRED)
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found! ")
endif(NOT GLUT_FOUND)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})

#Encontrando y linkeando glfw3
find_package(GLFW3 REQUIRED)
if(NOT GLFW3_FOUND)
        message(Error "GLFW not found")
endif(NOT GLFW3_FOUND)
include_directories(${GLFW_INCLUDE_DIR})

find_package(GLM REQUIRED)
if(NOT GLM_FOUND)
        message(Error "GLM not found")
endif(NOT GLM_FOUND)
include_directories(${GLM_INCLUDE_DIRS})
link_directories(${GLM_LIBRARY_DIRS})
add_definitions(${GLM_DEFINITIONS})


SET(all_libs ${OPENGL_LIBRARY} ${GLUT_LIBRARY} ${GLEW_LIBRARY} ${GLFW3_LIBRARY} ${GLM_LIBRARY})

add_subdirectory(src)
add_executable(desktop_application $<TARGET_OBJECTS:main_objects>)

target_link_libraries(desktop_application ${all_libs})
