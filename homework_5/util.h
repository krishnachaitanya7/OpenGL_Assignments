#ifndef OPENGL_ASSIGNMENTS_LORENZ_H
#define OPENGL_ASSIGNMENTS_LORENZ_H

class utils {
public:
    /*
     * Reference: ex13 from Moodle
     * */
    static int axes;       //  Display axes
    static int mode;       //  Projection mode
    static int move;       //  Move light
    static int th;         //  Azimuth of view angle
    static int ph;         //  Elevation of view angle
    static int fov;       //  Field of view (for perspective)
    static int light;      //  Lighting
    static double asp;     //  Aspect ratio
    static double dim;   //  Size of world
    static int one;  // Unit value
    static int distance;  // Light distance
    static int inc ;  // Ball increment
    static int smooth;  // Smooth/Flat shading
    static int local ;  // Local Viewer Model
    static int emission ;  // Emission intensity (%)
    static int ambient ;  // Ambient intensity (%)
    static int diffuse ;  // Diffuse intensity (%)
    static int specular;  // Specular intensity (%)
    static int shininess;  // Shininess (power of two)
    static float shiny;  // Shininess (value)
    static int zh;  // Light azimuth
    static int zph;  // Light azimuth
    static float ylight;  // Elevation of light

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
