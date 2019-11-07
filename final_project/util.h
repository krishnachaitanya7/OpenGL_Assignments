#ifndef OPENGL_ASSIGNMENTS_LORENZ_H
#define OPENGL_ASSIGNMENTS_LORENZ_H

class utils {
public:
    /*
     * Reference: ex13 from Moodle
     * */
    static int axes;
    static int mode;
    static int move;
    static int th;
    static int ph;
    static int fov;
    static int light;
    static double asp;
    static double dim;
    static int one;
    static int distance;
    static int inc ;
    static int smooth;
    static int local ;
    static int emission ;
    static int ambient ;
    static int diffuse ;
    static int specular;
    static int shininess;
    static float shiny;
    static int zh;
    static int zph;
    static int ntex;
    static float ylight;
    static unsigned textures[2];

    static void display_scene();
    static void reshape_window(int w, int h);
    static void Vertex(double th,double ph);
    static void ball(double x,double y,double z,double r);
    static void idle();
    static void draw_light_path();
    static void key(unsigned char ch,int x,int y);
    static void special(int key,int x,int y);
    static float* get_me_normals(float* pt1, float* pt2, float *pt3);
    static void draw_house(float translation);
    static void draw_cone();

};
#endif //OPENGL_ASSIGNMENTS_LORENZ_H
