project(Pulsar)
file(GLOB SOURCES "src/*.cpp")
add_library(Pulsar SHARED ${SOURCES})
target_link_libraries(Pulsar GLEW GL png assimp)
