project(Pulsar)
add_library(Pulsar SHARED src/Pulsar.cpp src/PulsarMesh.cpp src/PulsarShader.cpp src/PulsarTexture.cpp src/PulsarScene.cpp)
target_link_libraries(Pulsar GLEW GL png assimp)
