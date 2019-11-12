include_directories(include)
ADD_LIBRARY(CSCIx229 src/project.c src/print.c src/object.c src/loadtexbmp.c src/fatal.c src/errcheck.c include/CSCIx229.h)
add_executable(final_project main.cpp src/util.cpp src/TerrainBlock.cpp src/Terrain.cpp)
#[[
Below file commands lets you copy all required files to the binary directory
Very useful
Else you gotta keep copying to cmake-build-debug which is goddamn pain in the ass
]]

file(COPY required_files/piper_pa18.mtl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY required_files/Plane.obj DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY required_files/piper_bump.bmp DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY required_files/piper_diffuse.bmp DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY required_files/piper_refl.bmp DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY required_files/crate.bmp DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(final_project CSCIx229 glut GLU GL m)
