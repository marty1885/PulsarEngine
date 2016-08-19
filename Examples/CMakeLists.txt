add_subdirectory(example1)

# Copy data
file(COPY data DESTINATION ${CMAKE_BINARY_DIR})
