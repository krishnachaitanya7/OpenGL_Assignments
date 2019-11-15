add_executable(smoke main.cpp util.cpp particle_generator.cpp shader.cpp texture.cpp resource_manager.cpp SOIL.c
        image_DXT.c image_helper.c stb_image_aug.c)
#[[
Below file commands lets you copy all required files to the binary directory
Very useful
Else you gotta keep copying to cmake-build-debug which is goddamn pain in the ass
]]
file(COPY particle.frag DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY particle.vs DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
link_directories(libs)
target_link_libraries(smoke ${/home/shivababa/CLionProjects/OpenGL_Assignments/smoke_generation/libs/libSOIL.a} glut GLU GL m GLEW)
