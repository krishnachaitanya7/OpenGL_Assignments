ADD_LIBRARY(CSCIx229 project.c print.c object.c loadtexbmp.c fatal.c errcheck.c CSCIx229.h)
add_executable(hw5 main.cpp util.cpp)
target_link_libraries(hw5 glut GLU GL m CSCIx229)
