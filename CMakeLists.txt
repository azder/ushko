cmake_minimum_required(VERSION 3.9)
project(ushko)

set(CMAKE_CXX_STANDARD 11)

#find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

add_executable(ushko main.c utils.h glut.h)

target_compile_options(ushko PRIVATE -Wno-deprecated)
target_link_libraries(ushko ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

set_target_properties( ushko PROPERTIES RUNTIME_OUTPUT_DIRECTORY .. )