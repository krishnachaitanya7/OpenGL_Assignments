#ifndef OPENGL_ASSIGNMENTS_LORENZ_H
#define OPENGL_ASSIGNMENTS_LORENZ_H

class utils {
public:
    /*
     * Reference: ex13 from Moodle
     * */
    static int axes;       //  Display axes
    static int mode;       //  Shader mode
    static int move;       //  Move light
    static int proj;       //  Projection type
    static int th;         //  Azimuth of view angle
    static int ph;         //  Elevation of view angle
    static int fov;       //  Field of view (for perspective)
    static float spc;     //  Specular intensity
    static double asp;     //  Aspect ratio
    static double dim;   //  Size of world
    static double scale; //  Image scale
    static int zh;        //  Light azimuth
    static float Ylight;   //  Light elevation
    static int obj;          //  Object display list
    static float RGBA[4];  //  Colors
    static void display();
    static void idle();
    static void special(int key,int x,int y);
    static void key(unsigned char ch,int x,int y);
    static void reshape(int width,int height);

};
#endif //OPENGL_ASSIGNMENTS_LORENZ_H
