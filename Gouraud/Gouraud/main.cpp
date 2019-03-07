
//---------------------------------------
// Program: surface.cpp
// Purpose: Generate fractal surface using
//          recursive rectangle splitting.
// Author:  John Gauch
// Date:    September 2008
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GLUT/glut.h>

// Global variables
int xangle = 0;
int yangle = 0;
int zangle = 0;
#define SIZE 31

// Surface coordinates
float Px[SIZE + 1][SIZE + 1];
float Py[SIZE + 1][SIZE + 1];
float Pz[SIZE + 1][SIZE + 1];

//---------------------------------------
// Calculate random value between [-R..R]
//---------------------------------------
float myrand(float R)
{
    return (2 * R * rand()) / RAND_MAX - R;
}

//---------------------------------------
// Recursive function to split surface
//---------------------------------------
void split(int xlow, int xhigh, int ylow, int yhigh, float radius)
{
    // Check terminating condition
    if ((xhigh > xlow+1) || (yhigh > ylow+1))
    {
        // Calculate length of diagonal
        int xmid = (xhigh + xlow) / 2;
        int ymid = (yhigh + ylow) / 2;
        float dx = Px[xhigh][yhigh] - Px[xlow][ylow];
        float dy = Py[xhigh][yhigh] - Py[xlow][ylow];
        float dz = Pz[xhigh][yhigh] - Pz[xlow][ylow];
        float length = sqrt(dx * dx + dy * dy + dz * dz) / radius;
        
        // Generate five midpoints with random displacements
        Px[xlow][ymid] = (Px[xlow][ylow] + Px[xlow][yhigh]) / 2 + myrand(length);
        Py[xlow][ymid] = (Py[xlow][ylow] + Py[xlow][yhigh]) / 2 + myrand(length);
        Pz[xlow][ymid] = (Pz[xlow][ylow] + Pz[xlow][yhigh]) / 2 + myrand(length);
        
        Px[xhigh][ymid] = (Px[xhigh][ylow] + Px[xhigh][yhigh]) / 2 + myrand(length);
        Py[xhigh][ymid] = (Py[xhigh][ylow] + Py[xhigh][yhigh]) / 2 + myrand(length);
        Pz[xhigh][ymid] = (Pz[xhigh][ylow] + Pz[xhigh][yhigh]) / 2 + myrand(length);
        
        Px[xmid][ylow] = (Px[xlow][ylow] + Px[xhigh][ylow]) / 2 + myrand(length);
        Py[xmid][ylow] = (Py[xlow][ylow] + Py[xhigh][ylow]) / 2 + myrand(length);
        Pz[xmid][ylow] = (Pz[xlow][ylow] + Pz[xhigh][ylow]) / 2 + myrand(length);
        
        Px[xmid][yhigh] = (Px[xlow][yhigh] + Px[xhigh][yhigh]) / 2 + myrand(length);
        Py[xmid][yhigh] = (Py[xlow][yhigh] + Py[xhigh][yhigh]) / 2 + myrand(length);
        Pz[xmid][yhigh] = (Pz[xlow][yhigh] + Pz[xhigh][yhigh]) / 2 + myrand(length);
        
        Px[xmid][ymid] = (Px[xlow][ylow] + Px[xhigh][yhigh]) / 2 + myrand(length);
        Py[xmid][ymid] = (Py[xlow][ylow] + Py[xhigh][yhigh]) / 2 + myrand(length);
        Pz[xmid][ymid] = (Pz[xlow][ylow] + Pz[xhigh][yhigh]) / 2 + myrand(length);
        
        // Perform recursive calls
        split(xlow, xmid, ylow, ymid, radius);
        split(xmid, xhigh, ylow, ymid, radius);
        split(xlow, xmid, ymid, yhigh, radius);
        split(xmid, xhigh, ymid, yhigh, radius);
    }
}

//---------------------------------------
// Initialize random surface
//---------------------------------------
void init_surface()
{
    // Initialize surface points
    Px[0][0] = -0.5;
    Py[0][0] = -0.5;
    Pz[0][0] = 0.0;
    Px[0][SIZE] = -0.5;
    Py[0][SIZE] = 0.5;
    Pz[0][SIZE] = 0.0;
    Px[SIZE][0] = 0.5;
    Py[SIZE][0] = -0.5;
    Pz[SIZE][0] = 0.0;
    Px[SIZE][SIZE] = 0.5;
    Py[SIZE][SIZE] = 0.5;
    Pz[SIZE][SIZE] = 0.0;
    split(0, SIZE, 0, SIZE, 20);
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
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
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
    glRotatef(xangle, 1.0, 0.0, 0.0);
    glRotatef(yangle, 0.0, 1.0, 0.0);
    glRotatef(zangle, 0.0, 0.0, 1.0);
    
    // Draw the surface
    int i, j;
    glColor3f(1.0, 0.0, 0.0);
    for (i = 0; i <= SIZE; i++)
    {
        glBegin(GL_LINE_STRIP);
        for (j = 0; j <= SIZE; j++)
            glVertex3f(Px[i][j], Py[i][j], Pz[i][j]);
        glEnd();
    }
    glColor3f(0.0, 1.0, 0.0);
    for (j = 0; j <= SIZE; j++)
    {
        glBegin(GL_LINE_STRIP);
        for (i = 0; i <= SIZE; i++)
            glVertex3f(Px[i][j], Py[i][j], Pz[i][j]);
        glEnd();
    }
    glFlush();
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
    // Initialize surface
    if (key == 'i')
        init_surface();
    
    // Update angles
    else if (key == 'x')
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
    
    // Redraw objects
    glutPostRedisplay();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
    glutCreateWindow("Surface");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    init();
    init_surface();
    printf("Keyboard commands:\n");
    printf("   'x' - rotate x-axis -5 degrees\n");
    printf("   'X' - rotate x-axis +5 degrees\n");
    printf("   'y' - rotate y-axis -5 degrees\n");
    printf("   'Y' - rotate y-axis +5 degrees\n");
    printf("   'z' - rotate z-axis -5 degrees\n");
    printf("   'Z' - rotate z-axis +5 degrees\n");
    glutMainLoop();
    return 0;
}
