add_library(ariamis camera.cc engine.cc glad.cc light.cc material.cc mesh.cc obj_loader.cc object.cc renderer.cc scene.cc shader.cc texture.cc)
target_include_directories(ariamis PUBLIC "${PROJECT_SOURCE_DIR}/include")
