#ifndef OPENGL_ASSIGNMENTS_LORENZ_H
#define OPENGL_ASSIGNMENTS_LORENZ_H

class utils {
public:
    static float horizontal_angle;
    static float vertical_angle;
    static void reshape_window(int width,int height);
    static void arrow_keys_pressed(int key,int x,int y);
    static void display_3d_scene();
    static void draw_house(float translation);
    static float get_rand();
};
#endif //OPENGL_ASSIGNMENTS_LORENZ_H
