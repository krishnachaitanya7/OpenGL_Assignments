/*
 * Reference: ex13 from Moodle
 * */
#include <iostream>
#include "util.h"
#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#include "CSCIx229.h"
#include <math.h>
#include <vector>


void utils::Vertex(double th, double ph) {
    double x = Sin(th)*Cos(ph);
    double y = Cos(th)*Cos(ph);
    double z =         Sin(ph);
    //  For a sphere at the origin, the position
    //  and normal vectors are the same
    glNormal3d(x,y,z);
    glVertex3d(x,y,z);
}


void utils::ball(double x, double y, double z, double r){
    int th,ph;
    float yellow[] = {1.0,1.0,0.0,1.0};
    float Emission[]  = {0.0,0.0,static_cast<float>(0.01*emission),1.0};
    //  Save transformation
    glPushMatrix();
    //  Offset, scale and rotate
    glTranslated(x,y,z);
    glScaled(r,r,r);
    glMaterialf(GL_FRONT,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
    glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
    //  Bands of latitude
    for (ph=-90;ph<90;ph+=inc)
    {
        glBegin(GL_QUAD_STRIP);
        for (th=0;th<=360;th+=2*inc)
        {
            Vertex(th,ph);
            Vertex(th,ph+inc);
        }
        glEnd();
    }
    glPopMatrix();
}


void utils::display_scene(){
    const double len=2.0;  //  Length of axes
    //  Erase the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //  Enable Z-buffering in OpenGL
    glEnable(GL_DEPTH_TEST);

    //  Undo previous transformations
    glLoadIdentity();
    //  Perspective - set eye position
    if (mode)
    {
        double Ex = -2*dim*Sin(th)*Cos(ph);
        double Ey = +2*dim        *Sin(ph);
        double Ez = +2*dim*Cos(th)*Cos(ph);
        gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);

    }
        //  Orthogonal - set world orientation
    else
    {
        glRotatef(ph,1,0,0);
        glRotatef(th,0,1,0);
    }

    //  Flat or smooth shading
    glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

    //  Light switch
    if (light)
    {
        //  Translate intensity to color vectors
        float Ambient[]   = {static_cast<float>(0.01*ambient) ,static_cast<float>(0.01*ambient) ,static_cast<float>(0.01*ambient) ,1.0};
        float Diffuse[]   = {static_cast<float>(0.01*diffuse) ,static_cast<float>(0.01*diffuse) ,static_cast<float>(0.01*diffuse) ,1.0};
        float Specular[]  = {static_cast<float>(0.01*specular),static_cast<float>(0.01*specular),static_cast<float>(0.01*specular),1.0};
        //  Light position
        float Position[]  = {static_cast<float>(distance*Cos(zh)),ylight,static_cast<float>(distance*Sin(zh)),1.0};

        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);
        ball(Position[0],Position[1],Position[2] , 0.1);
//        draw_light_path();

        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
    }
    else
        glDisable(GL_LIGHTING);
    draw_house(0);
    draw_house(1);
    //  Draw axes - no lighting from here on
    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
    if (axes)
    {
        glBegin(GL_LINES);
        glVertex3d(0.0,0.0,0.0);
        glVertex3d(len,0.0,0.0);
        glVertex3d(0.0,0.0,0.0);
        glVertex3d(0.0,len,0.0);
        glVertex3d(0.0,0.0,0.0);
        glVertex3d(0.0,0.0,len);
        glEnd();
        //  Label axes
        glRasterPos3d(len,0.0,0.0);
        Print("X");
        glRasterPos3d(0.0,len,0.0);
        Print("Y");
        glRasterPos3d(0.0,0.0,len);
        Print("Z");
    }
    glWindowPos2i(5,5);
    Print("Angle=%d,%d  FOV=%d",
          th,ph,fov);

    //  Render the scene and make it visible
    ErrCheck("display");
    glFlush();
    glutSwapBuffers();

}

void utils::draw_cone() {
//    glTranslatef(0.0, -6.0, -20.0);
    glBegin(GL_TRIANGLES);
    float v[] = {0.0, 10.0, 0.0};
    float theta = 0;
    float c_f = 3.14/180;
    float v0[] = {cos(theta*c_f)*4, 0.0, sin(theta*c_f)*4};
    float v1[] = {0.0, 0.0, 0.0};
    for(theta=10; theta<=360; theta+=10){
        v1[0] = cos(theta*c_f)*4;
        v1[2] = sin(theta*c_f)*4;
        float* normal_ptr = get_me_normals(v0, v1, v);
        glNormal3f(normal_ptr[0], normal_ptr[1], normal_ptr[2]);
        glVertex3f(v[0], v[1], v[2]);
        glVertex3f(v0[0], v0[1], v0[2]);
        glVertex3fv(v1);
        v0[0] = v1[0];
        v0[2] = v1[2];
        delete normal_ptr;
    }
}


void utils::idle() {
    if(move){
        double t;
        t = glutGet(GLUT_ELAPSED_TIME)/1000.0;

        zh = fmod(90*t,360.0);
        zph = fmod(90*t,360.0);
        //  Tell GLUT it is necessary to redisplay the scene
    }
    glutPostRedisplay();
}

void utils::reshape_window(int width, int height) {
    //  Ratio of the width to the height of the window
    asp = (height>0) ? (double)width/height : 1;
    //  Set the viewport to the entire window
    glViewport(0,0, width,height);
    //  Set projection
    Project(mode?fov:0,asp,dim);
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void utils::special(int key,int x,int y)
{
    //  Right arrow key - increase angle by 5 degrees
    if (key == GLUT_KEY_RIGHT)
        th += 5;
        //  Left arrow key - decrease angle by 5 degrees
    else if (key == GLUT_KEY_LEFT)
        th -= 5;
        //  Up arrow key - increase elevation by 5 degrees
    else if (key == GLUT_KEY_UP)
        ph += 5;
        //  Down arrow key - decrease elevation by 5 degrees
    else if (key == GLUT_KEY_DOWN)
        ph -= 5;
        //  PageUp key - increase dim
    else if (key == GLUT_KEY_PAGE_DOWN)
        dim += 0.1;
        //  PageDown key - decrease dim
    else if (key == GLUT_KEY_PAGE_UP && dim>1)
        dim -= 0.1;
        //  Smooth color model
    else if (key == GLUT_KEY_F1)
        smooth = 1-smooth;
        //  Local Viewer
    else if (key == GLUT_KEY_F2)
        local = 1-local;
    else if (key == GLUT_KEY_F3)
        distance = (distance==1) ? 5 : 1;
        //  Toggle ball increment
    else if (key == GLUT_KEY_F8)
        inc = (inc==10)?3:10;
        //  Flip sign
    else if (key == GLUT_KEY_F9)
        one = -one;
    //  Keep angles to +/-360 degrees
    th %= 360;
    ph %= 360;
    //  Update projection
    Project(mode?fov:0,asp,dim);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}


void utils::draw_house(float translation) {
    if(ntex){
        float white[] = {1,1,1,1};
        float Emission[]  = {0.0,0.0,static_cast<float>(0.01*emission),1.0};
        glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
        glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
        glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
        glColor3f(1,1,1);
        glBindTexture(GL_TEXTURE_2D,textures[0]);
    } else{
        glColor3f (1,1,0);
    }

    glBegin(GL_TRIANGLES);
    std::vector<float>  point_1 {static_cast<float>(0.8+translation), 0.6, 0.0};
    std::vector<float> point_2 {static_cast<float>(0.8+translation), 0.6, 0.5};
    std::vector<float> point_3 {static_cast<float>(0.8+translation), 0.8, 0.25};
    float* normals_ptr = get_me_normals(point_1.data(), point_2.data(), point_3.data());
    glNormal3f(normals_ptr[0], normals_ptr[1], normals_ptr[2]);
    glTexCoord2f(0,0); glVertex3f (point_1[0], point_1[1], point_1[2]);
    glTexCoord2f(1,0); glVertex3f (point_2[0], point_2[1], point_2[2]);
    glTexCoord2f(0,1); glVertex3f (point_3[0], point_3[1], point_3[2]);
    glEnd();
    delete normals_ptr;


    glBegin(GL_TRIANGLES);
    point_1 = {static_cast<float>(0.1+translation), 0.6, 0.0};
    point_2 = {static_cast<float>(0.1+translation), 0.6, 0.5};
    point_3 = {static_cast<float>(0.1+translation), 0.8, 0.25};
    normals_ptr = get_me_normals(point_1.data(), point_2.data(), point_3.data());
    glNormal3f(-normals_ptr[0], -normals_ptr[1], -normals_ptr[2]);
    glTexCoord2f(0,0); glVertex3f (point_1[0], point_1[1], point_1[2]);
    glTexCoord2f(1,0); glVertex3f (point_2[0], point_2[1], point_2[2]);
    glTexCoord2f(0,1); glVertex3f (point_3[0], point_3[1], point_3[2]);
    delete normals_ptr;
    glEnd();


    glBegin(GL_POLYGON);
    point_1 = {static_cast<float>(0.1+translation), 0.6, 0.0};
    point_2 = {static_cast<float>(0.8+translation), 0.6, 0.0};
    point_3 = {static_cast<float>(0.8+translation), 0.8, 0.25};
    normals_ptr = get_me_normals(point_1.data(), point_2.data(), point_3.data());
    glTexCoord2f(0,0); glNormal3f(normals_ptr[0], normals_ptr[1], normals_ptr[2]);
    glTexCoord2f(1,0); glVertex3f (point_1[0], point_1[1], point_1[2]);
    glTexCoord2f(1,1); glVertex3f (point_2[0], point_2[1], point_2[2]);
    glTexCoord2f(0,1); glVertex3f (point_3[0], point_3[1], point_3[2]);
    delete normals_ptr;
    glVertex3f (0.1+translation, 0.8, 0.25);
    glEnd();


    glBegin(GL_POLYGON);
    point_1 = {static_cast<float>(0.1+translation), 0.6, 0.5};
    point_2 = {static_cast<float>(0.8+translation), 0.6, 0.5};
    point_3 = {static_cast<float>(0.8+translation), 0.8, 0.25};
    normals_ptr = get_me_normals(point_1.data(), point_2.data(), point_3.data());
    glTexCoord2f(0,0); glNormal3f(-normals_ptr[0], -normals_ptr[1], -normals_ptr[2]);
    glTexCoord2f(1,0); glVertex3f (point_1[0], point_1[1], point_1[2]);
    glTexCoord2f(1,1); glVertex3f (point_2[0], point_2[1], point_2[2]);
    glTexCoord2f(0,1); glVertex3f (point_3[0], point_3[1], point_3[2]);
    delete normals_ptr;
    glVertex3f (0.1+translation, 0.8, 0.25);
    glEnd();

    ntex ? glBindTexture(GL_TEXTURE_2D,textures[1]) : glColor3f (0,1,1);
    glBegin(GL_POLYGON);
    point_1 = {static_cast<float>(0.1+translation), 0.1, 0.0};
    point_2 = {static_cast<float>(0.8+translation), 0.1, 0.0};
    point_3 = {static_cast<float>(0.8+translation), 0.6, 0.0};
    normals_ptr = get_me_normals(point_1.data(), point_2.data(), point_3.data());
    glNormal3f(normals_ptr[0], normals_ptr[1], normals_ptr[2]);
    glTexCoord2f(0,0); glVertex3f (point_1[0], point_1[1], point_1[2]);
    glTexCoord2f(1,0); glVertex3f (point_2[0], point_2[1], point_2[2]);
    glTexCoord2f(1,1); glVertex3f (point_3[0], point_3[1], point_3[2]);
    delete normals_ptr;
    glTexCoord2f(0,1); glVertex3f (0.1+translation, 0.6, 0.0);
    glEnd();


    glBegin(GL_POLYGON);
    point_1 = {static_cast<float>(0.1+translation), 0.1, 0.5};
    point_2 = {static_cast<float>(0.8+translation), 0.1, 0.5};
    point_3 = {static_cast<float>(0.8+translation), 0.6, 0.5};
    normals_ptr = get_me_normals(point_1.data(), point_2.data(), point_3.data());
    glNormal3f(-normals_ptr[0], -normals_ptr[1], -normals_ptr[2]);
    glTexCoord2f(0,0); glVertex3f (point_1[0], point_1[1], point_1[2]);
    glTexCoord2f(1,0); glVertex3f (point_2[0], point_2[1], point_2[2]);
    glTexCoord2f(1,1); glVertex3f (point_3[0], point_3[1], point_3[2]);
    delete normals_ptr;
    glTexCoord2f(0,1); glVertex3f (0.1+translation, 0.6, 0.5);
    glEnd();



    glBegin(GL_POLYGON);
    point_1 = {static_cast<float>(0.1+translation), 0.1, 0.5};
    point_2 = {static_cast<float>(0.1+translation), 0.1, 0.0};
    point_3 = {static_cast<float>(0.1+translation), 0.6, 0.0};
    normals_ptr = get_me_normals(point_1.data(), point_2.data(), point_3.data());
    glNormal3f(normals_ptr[0], normals_ptr[1], normals_ptr[2]);
    glTexCoord2f(0,0); glVertex3f (point_1[0], point_1[1], point_1[2]);
    glTexCoord2f(1,0); glVertex3f (point_2[0], point_2[1], point_2[2]);
    glTexCoord2f(1,1); glVertex3f (point_3[0], point_3[1], point_3[2]);
    delete normals_ptr;
    glTexCoord2f(0,1); glVertex3f (0.1+translation, 0.6, 0.5);
    glEnd();



    glBegin(GL_POLYGON);
    point_1 = {static_cast<float>(0.8+translation), 0.1, 0.5};
    point_2 = {static_cast<float>(0.8+translation), 0.1, 0.0};
    point_3 = {static_cast<float>(0.8+translation), 0.6, 0.0};
    normals_ptr = get_me_normals(point_1.data(), point_2.data(), point_3.data());
    glNormal3f(-normals_ptr[0], -normals_ptr[1], -normals_ptr[2]);
    glTexCoord2f(0,0); glVertex3f (point_1[0], point_1[1], point_1[2]);
    glTexCoord2f(1,0); glVertex3f (point_2[0], point_2[1], point_2[2]);
    glTexCoord2f(1,1); glVertex3f (point_3[0], point_3[1], point_3[2]);
    delete normals_ptr;
    glTexCoord2f(0,1); glVertex3f (0.8+translation, 0.6, 0.5);
    glEnd();

    if(ntex){
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }

}

/*
 *  GLUT calls this routine when a key is pressed
 */
void utils::key(unsigned char ch,int x,int y)
{
    /*
     * Preserved from ex13, for ease of correction
     * */
    //  Exit on ESC
    if (ch == 27)
        exit(0);
        //  Reset view angle
    else if (ch == '0')
        th = ph = 0;
        //  Toggle axes
    else if (ch == 'x' || ch == 'X')
        axes = 1-axes;
        //  Toggle lighting
    else if (ch == 'l' || ch == 'L')
        light = 1-light;
        //  Switch projection mode
    else if (ch == 'p' || ch == 'P')
        mode = 1-mode;
        //  Toggle light movement
    else if (ch == 'm' || ch == 'M')
        move = 1-move;
        //  Move light
    else if (ch == 't')
        ntex = 1-ntex;
    else if (ch == '<')
        zh += 1;
    else if (ch == '>')
        zh -= 1;
        //  Change field of view angle
    else if (ch == '-' && ch>1)
        fov++;
    else if (ch == '+' && ch<179)
        fov--;
        //  Light elevation
    else if (ch=='[')
        ylight -= 0.1;
    else if (ch==']')
        ylight += 0.1;
        //  Ambient level
    else if (ch=='a' && ambient>0)
        ambient -= 5;
    else if (ch=='A' && ambient<100)
        ambient += 5;
        //  Diffuse level
    else if (ch=='d' && diffuse>0)
        diffuse -= 5;
    else if (ch=='D' && diffuse<100)
        diffuse += 5;
        //  Specular level
    else if (ch=='s' && specular>0)
        specular -= 5;
    else if (ch=='S' && specular<100)
        specular += 5;
        //  Emission level
    else if (ch=='e' && emission>0)
        emission -= 5;
    else if (ch=='E' && emission<100)
        emission += 5;
        //  Shininess level
    else if (ch=='n' && shininess>-1)
        shininess -= 1;
    else if (ch=='N' && shininess<7)
        shininess += 1;
    //  Translate shininess power to value (-1 => 0)
    shiny = shininess<0 ? 0 : pow(2.0,shininess);
    //  Reproject
    Project(mode?fov:0,asp,dim);
    //  Animate if requested
    glutIdleFunc(move?idle:NULL);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

void utils::draw_light_path() {
    int zh=0, zph=0, i;
    glBegin(GL_POINTS);
    for(i=0; i<361; i++){
        float Position[]  = {static_cast<float>(distance*Cos(zh)),ylight,static_cast<float>(distance*Sin(zh)),1.0};
        float Position1[]  = {ylight, static_cast<float>(distance*Cos(zph)),static_cast<float>(distance*Sin(zph)),1.0};
        glVertex3d(Position[0], Position[1], Position[2]);
        glVertex3d(Position1[0], Position1[1], Position1[2]);
        zh++;
        zph++;
    }
    glEnd();
}

float *utils::get_me_normals(float *pt1, float *pt2, float *pt3) {
    float X[] = {pt2[0]-pt1[0], pt2[1]-pt1[1], pt2[2]-pt1[2]};
    float Y[] = {pt3[0]-pt1[0], pt3[1]-pt1[1], pt3[2]-pt1[2]};
    float *result;
    result=new float[3];
    result[0] = (X[1] * Y[2] - X[2] * Y[1]);
    result[1] = - (X[0] * Y[2] - X[2] * Y[0]);
    result[2] = (X[0] * Y[1] - X[1] * Y[0]);
    float mod_result =  sqrt(result[0]*result[0] + result[1]*result[1] + result[2]*result[2]);
    result[0] = -result[0]/mod_result;
    result[1] = -result[1]/mod_result;
    result[2] = -result[2]/mod_result;
    return result;

}




