add_executable(quagmire main.cpp FireEngine.cpp Particle.cpp Texture.cpp)
file(COPY particle.bmp DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(quagmire CSCIx229 glut GLU GL m)