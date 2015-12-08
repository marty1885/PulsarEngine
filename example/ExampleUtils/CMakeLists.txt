project(ExampleUtils)
add_library(ExampleUtils STATIC src/ExampleUtils.cpp)
target_link_libraries(ExampleUtils SDL2 GLEW GL)
