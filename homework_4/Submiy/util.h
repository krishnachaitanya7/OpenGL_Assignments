#ifndef OPENGL_ASSIGNMENTS_LORENZ_H
#define OPENGL_ASSIGNMENTS_LORENZ_H

class utils {
public:

    static float azimuth_angle;
    static float elevation_angle;
    static double cam_x;
    static double cam_y;
    static double cam_z;
    static double cam_look_x;
    static double cam_look_y;
    static double cam_look_z;
    static int fov;
    static double dims;
    static int mode_of_view;
    static void reshape_window(int width,int height);
    static void keys_pressed(unsigned char ch, int x, int y);
    static void arrow_keys_pressed(int key,int x,int y);
    static void display_3d_scene();
    static void view_mode_and_display();
    static void update_projection_matrix();
    static void draw_house(float translation);
    static void print_orientation();
    static float get_rand();
    static double Sin(double x);
    static double Cos(double x);
};
#endif //OPENGL_ASSIGNMENTS_LORENZ_H
