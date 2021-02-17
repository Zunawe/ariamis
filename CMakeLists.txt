cmake_minimum_required(VERSION 3.10)

# set the project name
project(example)
set(CMAKE_CXX_FLAGS "-g")
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# OpenGL
# find_package(OpenGL REQUIRED)

# GLM
# find_package(GLM REQUIRED)
# message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")

# # GLFW
# find_package(GLFW3 REQUIRED)
# message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

add_subdirectory(src)
file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# add the executable
add_executable(example example.cc)
target_link_libraries(example PUBLIC ariamis ${CONAN_LIBS})
target_include_directories(example PUBLIC "${PROJECT_SOURCE_DIR}/include")
