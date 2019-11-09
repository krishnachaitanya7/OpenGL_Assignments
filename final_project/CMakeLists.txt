ADD_LIBRARY(CSCIx229 project.c print.c object.c loadtexbmp.c fatal.c errcheck.c CSCIx229.h)
add_executable(final_project main.cpp util.cpp)
#[[
Below file commands lets you copy all required files to the binary directory
Very useful
Else you gotta keep copying to cmake-build-debug which is goddamn pain in the ass
]]
file(COPY piper_pa18.mtl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Plane.obj DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY piper_bump.bmp DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY piper_diffuse.bmp DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY piper_refl.bmp DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(final_project glut GLU GL m CSCIx229)