ADD_LIBRARY(CSCIx229 project.c print.c object.c loadtexbmp.c fatal.c errcheck.c CSCIx229.h)
add_executable(final_project main.cpp util.cpp)
#[[
Below file commands lets you copy all required files to the binary directory
Very useful
Else you gotta keep copying to cmake-build-debug which is goddamn pain in the ass
]]
file(COPY rock_texture.bmp DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY crate.bmp DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(final_project glut GLU GL m CSCIx229)