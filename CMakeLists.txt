cmake_minimum_required(VERSION 3.10)

# set the project name
project(example)
set(CMAKE_CXX_FLAGS "-g")

add_subdirectory(src)
file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# add the executable
add_executable(example example.cc)
target_link_libraries(example PUBLIC ariamis)
target_include_directories(example PUBLIC ${PROJECT_SOURCE_DIR}/include)
