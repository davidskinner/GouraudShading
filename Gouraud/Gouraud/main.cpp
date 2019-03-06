//---------------------------------------
// Program: surface4.cpp
// Purpose: Use Phong shading to display
//          random wave surface model.
// Author:  John Gauch
// Date:    Spring 2012
//---------------------------------------

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <GLUT/glut.h>
//---------------------------------------
// Program: shading.cpp
// Purpose: Functions for Phong shading
// Author:  John Gauch
// Date:    March 2013
//---------------------------------------

// Material properties
float Ka = 0.2;
float Kd = 0.4;
float Ks = 0.4;
float Kp = 0.5;

//---------------------------------------
// Initialize material properties
//---------------------------------------
void init_material(float Ka, float Kd, float Ks, float Kp,
                   float Mr, float Mg, float Mb)
{
    // Material variables
    float ambient[] = { Ka * Mr, Ka * Mg, Ka * Mb, 1.0 };
    float diffuse[] = { Kd * Mr, Kd * Mg, Kd * Mb, 1.0 };
    float specular[] = { Ks * Mr, Ks * Mg, Ks * Mb, 1.0 };
    
    // Initialize material
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Kp);
}

//---------------------------------------
// Initialize light source
//---------------------------------------
void init_light(int light_source, float Lx, float Ly, float Lz,
                float Lr, float Lg, float Lb)
{
    // Light variables
    float light_position[] = { Lx, Ly, Lz, 0.0 };
    float light_color[] = { Lr, Lg, Lb, 1.0 };
    
    // Initialize light source
    glEnable(GL_LIGHTING);
    glEnable(light_source);
    glLightfv(light_source, GL_POSITION, light_position);
    glLightfv(light_source, GL_AMBIENT, light_color);
    glLightfv(light_source, GL_DIFFUSE, light_color);
    glLightfv(light_source, GL_SPECULAR, light_color);
    glLightf(light_source, GL_CONSTANT_ATTENUATION, 1.0);
    glLightf(light_source, GL_LINEAR_ATTENUATION, 0.0);
    glLightf(light_source, GL_QUADRATIC_ATTENUATION, 0.0);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

// Put following inside init() function
// glShadeModel(GL_SMOOTH);
// glEnable(GL_NORMALIZE);
// init_light(GL_LIGHT0, 0, 1, 1, 0.5, 0.5, 0.5);
// init_light(GL_LIGHT1, 0, 0, 1, 0.5, 0.5, 0.5);
// init_light(GL_LIGHT2, 0, 1, 0, 0.5, 0.5, 0.5);

// Put following inside display() function
// init_material(Ka, Kd, Ks, 100 * Kp, 0.8, 0.6, 0.4);
// Transformation variables
#define ROTATE 1
#define TRANSLATE 2
int xangle = 0;
int yangle = 0;
int zangle = 0;
int xpos = 0;
int ypos = 0;
int zpos = 0;
int mode = ROTATE;


// Surface variables
#define SIZE 512
float Px[SIZE + 1][SIZE + 1];
float Py[SIZE + 1][SIZE + 1];
float Pz[SIZE + 1][SIZE + 1];
float Nx[SIZE + 1][SIZE + 1];
float Ny[SIZE + 1][SIZE + 1];
float Nz[SIZE + 1][SIZE + 1];
#define STEP 0.1


//---------------------------------------
// Initialize surface points
//---------------------------------------
void init_surface(float Xmin, float Xmax, float Ymin, float Ymax)
{
    // Initialize (x,y,z) coordinates of surface
    for (int i = 0; i <= SIZE; i++)
        for (int j = 0; j <= SIZE; j++)
        {
            Px[i][j] = Xmin + i * (Xmax - Xmin) / SIZE;
            Py[i][j] = Ymin + j * (Ymax - Ymin) / SIZE;
            Pz[i][j] = 0;
        }
    
    // Add randoms waves to surface
    for (int wave = 1; wave <= 10; wave++)
    {
        int rand_i = rand() % SIZE / wave;
        int rand_j = rand() % SIZE / wave;
        float length = sqrt(rand_i * rand_i + rand_j * rand_j);
        if (length >= 10)
            for (int i = 0; i <= SIZE; i++)
                for (int j = 0; j <= SIZE; j++)
                {
                    float angle = (rand_i * i + rand_j * j) / (length * length);
                    Pz[i][j] += 0.01 * sin(angle * 2 * M_PI);
                }
    }
}

//---------------------------------------
// Initialize surface normals
//---------------------------------------
void init_normals()
{
    // Initialize surface normals
    for (int i=0; i<=SIZE; i++)
        for (int j=0; j<=SIZE; j++)
        {
            // Get tangents S and T
            float Sx = (i<SIZE) ? Px[i+1][j] - Px[i][j] : Px[i][j] - Px[i-1][j];
            float Sy = (i<SIZE) ? Py[i+1][j] - Py[i][j] : Py[i][j] - Py[i-1][j];
            float Sz = (i<SIZE) ? Pz[i+1][j] - Pz[i][j] : Pz[i][j] - Pz[i-1][j];
            float Tx = (j<SIZE) ? Px[i][j+1] - Px[i][j] : Px[i][j] - Px[i][j-1];
            float Ty = (j<SIZE) ? Py[i][j+1] - Py[i][j] : Py[i][j] - Py[i][j-1];
            float Tz = (j<SIZE) ? Pz[i][j+1] - Pz[i][j] : Pz[i][j] - Pz[i][j-1];
            
            // Calculate N = S cross T
            float Slength = sqrt(Sx*Sx + Sy*Sy + Sz*Sz);
            float Tlength = sqrt(Tx*Tx + Ty*Ty + Tz*Tz);
            if ((Slength * Tlength) > 0)
            {
                Nx[i][j] = (Sy*Tz - Sz*Ty) / (Slength * Tlength);
                Ny[i][j] = (Sz*Tx - Sx*Tz) / (Slength * Tlength);
                Nz[i][j] = (Sx*Ty - Sy*Tx) / (Slength * Tlength);
            }
        }
}

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
    // Initialize OpenGL
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float radius = 1.2;
    glOrtho(-radius, radius, -radius, radius, -radius, radius);
    glEnable(GL_DEPTH_TEST);
    
    // Initialize smooth shading
    glShadeModel(GL_SMOOTH);
    init_light(GL_LIGHT0, 1, 1, 1, 1, 1, 1);
    
    // Initialize surface
    init_surface(-1.0, 1.0, -1.0, 1.0);
    init_normals();
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
    // Incrementally rotate objects
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(xpos / 500.0, ypos / 500.0, zpos / 500.0);
    glRotatef(xangle, 1.0, 0.0, 0.0);
    glRotatef(yangle, 0.0, 1.0, 0.0);
    glRotatef(zangle, 0.0, 0.0, 1.0);
    
    // Initialize material properties
    init_material(Ka, Kd, Ks, 100 * Kp, 0.3, 0.3, 0.8);
    
    // Draw the surface
    int i, j;
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
        {
            // glBegin(GL_LINE_LOOP);
            glBegin(GL_POLYGON);
            glNormal3f(Nx[i][j], Ny[i][j], Nz[i][j]);
            glVertex3f(Px[i][j], Py[i][j], Pz[i][j]);
            glNormal3f(Nx[i + 1][j], Ny[i + 1][j], Nz[i + 1][j]);
            glVertex3f(Px[i + 1][j], Py[i + 1][j], Pz[i + 1][j]);
            glNormal3f(Nx[i + 1][j + 1], Ny[i + 1][j + 1], Nz[i + 1][j + 1]);
            glVertex3f(Px[i + 1][j + 1], Py[i + 1][j + 1], Pz[i + 1][j + 1]);
            glNormal3f(Nx[i][j + 1], Ny[i][j + 1], Nz[i][j + 1]);
            glVertex3f(Px[i][j + 1], Py[i][j + 1], Pz[i][j + 1]);
            glEnd();
        }
    glFlush();
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
    // Initialize random surface
    if (key == 'i')
    {
        init_surface(-1.0, 1.0, -1.0, 1.0);
        init_normals();
    }
    
    // Determine if we are in ROTATE or TRANSLATE mode
    if ((key == 'r') || (key == 'R'))
    {
        printf("Type x y z to decrease or X Y Z to increase ROTATION angles.\n");
        mode = ROTATE;
    }
    else if ((key == 't') || (key == 'T'))
    {
        printf
        ("Type x y z to decrease or X Y Z to increase TRANSLATION distance.\n");
        mode = TRANSLATE;
    }
    
    // Handle ROTATE
    if (mode == ROTATE)
    {
        if (key == 'x')
            xangle -= 5;
        else if (key == 'y')
            yangle -= 5;
        else if (key == 'z')
            zangle -= 5;
        else if (key == 'X')
            xangle += 5;
        else if (key == 'Y')
            yangle += 5;
        else if (key == 'Z')
            zangle += 5;
    }
    
    // Handle TRANSLATE
    if (mode == TRANSLATE)
    {
        if (key == 'x')
            xpos -= 5;
        else if (key == 'y')
            ypos -= 5;
        else if (key == 'z')
            zpos -= 5;
        else if (key == 'X')
            xpos += 5;
        else if (key == 'Y')
            ypos += 5;
        else if (key == 'Z')
            zpos += 5;
    }
    
    // Handle material properties
    if (key == 'a')
        Ka -= STEP;
    if (key == 'd')
        Kd -= STEP;
    if (key == 's')
        Ks -= STEP;
    if (key == 'p')
        Kp -= STEP;
    if (key == 'A')
        Ka += STEP;
    if (key == 'D')
        Kd += STEP;
    if (key == 'S')
        Ks += STEP;
    if (key == 'P')
        Kp += STEP;
    if (Ka < 0)
        Ka = 0;
    if (Kd < 0)
        Kd = 0;
    if (Ks < 0)
        Ks = 0;
    if (Kp < STEP)
        Kp = STEP;
    
    glutPostRedisplay();
}

//---------------------------------------
// Mouse callback for OpenGL
//---------------------------------------
void mouse(int button, int state, int x, int y)
{
    // Handle mouse down
    static int xdown, ydown;
    if (state == GLUT_DOWN)
    {
        xdown = x;
        ydown = y;
    }
    
    // Handle ROTATE
    if ((mode == ROTATE) && (state == GLUT_UP))
    {
        xangle += (y - ydown);
        yangle -= (x - xdown);
        zangle = 0;
        glutPostRedisplay();
    }
    
    // Handle TRANSLATE
    if ((mode == TRANSLATE) && (state == GLUT_UP))
    {
        xpos += (x - xdown);
        ypos -= (y - ydown);
        glutPostRedisplay();
    }
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
    // Create OpenGL window
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
    glutCreateWindow("Surface");
    init();
    printf("Type r to enter ROTATE mode or t to enter TRANSLATE mode.\n");
    
    // Specify callback function
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
