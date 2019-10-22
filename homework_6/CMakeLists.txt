ADD_LIBRARY(CSCIx229 project.c print.c object.c loadtexbmp.c errcheck.c CSCIx229.h)
add_executable(hw6 main.cpp util.cpp)
target_link_libraries(hw6 glut GLU GL m CSCIx229)