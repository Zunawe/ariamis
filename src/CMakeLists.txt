set(CMAKE_CXX_STANDARD 17)

# GLM
find_package(glm REQUIRED)
message(STATUS "Found GLM in ${glm_DIR}")

# GLFW
find_package(glfw3 REQUIRED)
message(STATUS "Found GLFW3 in ${glfw3_DIR}")

# GLFW
find_package(Stb REQUIRED)
message(STATUS "Found STB in ${Stb_INCLUDE_DIR}")

add_library(ariamis camera.cc engine.cc glad.cc light.cc material.cc mesh.cc obj_loader.cc object.cc renderer.cc scene.cc shader.cc texture.cc)
target_link_libraries(ariamis PUBLIC glfw glm)
target_include_directories(ariamis PUBLIC ${PROJECT_SOURCE_DIR}/include ${Stb_INCLUDE_DIR})
