project(example1)
add_executable(example1 "src/example.cpp")
target_link_libraries(example1 SDL2 GLEW GL Pulsar)
