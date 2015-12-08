project(example)
add_executable(example src/example.cpp)
target_link_libraries(example SDL2 GLEW GL Pulsar ExampleUtils)
