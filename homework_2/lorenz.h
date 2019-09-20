#ifndef OPENGL_ASSIGNMENTS_LORENZ_H
#define OPENGL_ASSIGNMENTS_LORENZ_H

class lorenz_attractor {
public:
    // Lorenz Parameters
    static double sigma;
    static double r;
    static double b;
    const static int N = 100000;
    static double points_accumulator[N][3];
    // OpenGL Parameters
    static int azi_angle;
    static int ele_angle;
    static double box_dim;
    static void print_axes(int x, int y, int z, char* string);
    static void print_program_directions(int x, int y);
    static void lorenz_attractor_math();
    static void reshape_window(int width,int height);
    static void arrow_keys_pressed(int key,int x,int y);
    static void lorenz_keys(unsigned char key, int x, int y);
    static void display_lorenz();
};


#endif //OPENGL_ASSIGNMENTS_LORENZ_H
