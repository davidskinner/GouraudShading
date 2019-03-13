
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
#include <vector>

class V3
{
public:
    float x;
    float y;
    float z;
    
    V3(float x, float y, float z) : x(x),y(y)
    {}
};

class RGB
{
public:
    float red;
    float green;
    float blue;
    
    RGB(float r, float g, float b) : red(r), green(g), blue(b)
    {}
};



V3 L1 = V3(0,0,1);
V3 L2 = V3(0,0,0);

float a = .1;
float b = .09;
float c = .2;

RGB LightOneColor = RGB(0.1,0.2,0.3);
RGB LightTwoColor = RGB(0.9,0.8,0.7);

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
#define SIZE 100
float Px[SIZE + 1][SIZE + 1];
float Py[SIZE + 1][SIZE + 1];
float Pz[SIZE + 1][SIZE + 1];
float Nx[SIZE + 1][SIZE + 1];
float Ny[SIZE + 1][SIZE + 1];
float Nz[SIZE + 1][SIZE + 1];
#define STEP 0.1

float red[SIZE +1][SIZE +1];
float green[SIZE +1][SIZE +1];
float blue[SIZE +1][SIZE +1];

//---------------------------------------
// Initialize surface points
//---------------------------------------
void init_surface(float Xmin, float Xmax, float Ymin, float Ymax) // big boi
{
    // Initialize (x,y,z) coordinates of surface
    for (int i = 0; i <= SIZE; i++)
        for (int j = 0; j <= SIZE; j++)
        {
            Px[i][j] = Xmin + i * (Xmax - Xmin) / SIZE;
            Py[i][j] = Ymin + j * (Ymax - Ymin) / SIZE;
            Pz[i][j] = 0;
        }
    
    // Add randoms waves to surface //play with this for surface texture
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
void init_normals() // big boi
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
    // Initialize surface
    init_surface(-1.0, 1.0, -1.0, 1.0);
    init_normals();
}

//red(i,j)
//green(i,j)
//blue(i,j)



V3 calculateV(V3 light, float x, float y, float z)
{
    // x2-x1, y2-y1, z2-z1
    V3 temp = V3(light.x - x, light.y - y, light.z - z);
    return temp;
}

void dotProduct()
{
    //x1*x2 + y1*y2 + z1*z2
    
    
}



void doLighting()
{
    V3 v = V3(1,2,3);
    float magnitudeOne = 3;
    float magnitudeTwo = 5;
    float dotProduct;
    float EuclideanD;
    float inverseSquare;
    
    for (int i =0; i <= SIZE; i++) {
        for (int j = 0; j <= SIZE; j++) {
            //v
            v = V3(L1.x - Px[i][j], L1.y - Py[i][j], L1.z - Pz[i][j]);
            
            //v*n
            dotProduct = v.x * Nx[i][j] + v.y * Ny[i][j] + v.z * Nz[i][j];
            
            EuclideanD = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
            
            // 1/(a+bD+cD2)
            inverseSquare = 1/(a + b * EuclideanD + c * EuclideanD * EuclideanD);
            
            magnitudeOne = dotProduct * inverseSquare;
        
            //////////////////////////////
            //v
            v = V3(L2.x - Px[i][j], L2.y - Py[i][j], L2.z - Pz[i][j]);
            
            
            //v*n
            
            dotProduct = v.x * Nx[i][j] + v.y * Ny[i][j] + v.z * Nz[i][j];
            
            EuclideanD = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
            
            // 1/(a+bD+cD2)
            inverseSquare = 1/(a + b * EuclideanD + c * EuclideanD * EuclideanD);

            magnitudeTwo = dotProduct * inverseSquare;
            
            red[i][j] = (magnitudeOne * LightOneColor.red) + (magnitudeTwo * LightTwoColor.red);
            green[i][j] = (magnitudeOne * LightOneColor.green) + (magnitudeTwo * LightTwoColor.green);
            blue[i][j] = (magnitudeOne * LightOneColor.blue) + (magnitudeTwo * LightTwoColor.blue);
        }
    }

}

//three 2d arrays

void display()
{
    // Incrementally rotate objects
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(xangle, 1.0, 0.0, 0.0);
    glRotatef(yangle, 0.0, 1.0, 0.0);
    glRotatef(zangle, 0.0, 0.0, 1.0);
    
    doLighting();
    
    // Draw the surface
    int i, j;
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
        {
            
            glBegin(GL_POLYGON);
            glColor3f(red[i][j], green[i][j], blue[i][j]); // glColor(red(),green(),blue())
            glVertex3f(Px[i][j], Py[i][j], Pz[i][j]);
            glColor3f(red[i + 1][j], green[i + 1][j], blue[i + 1][j]);
            glVertex3f(Px[i + 1][j], Py[i + 1][j], Pz[i + 1][j]);
            glColor3f(red[i + 1][j + 1], green[i + 1][j + 1], blue[i + 1][j + 1]);
            glVertex3f(Px[i + 1][j + 1], Py[i + 1][j + 1], Pz[i + 1][j + 1]);
            glColor3f(red[i][j + 1], green[i][j + 1], blue[i][j + 1]);
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
    
    //select 1 or 2 for the light mode
    //(if(mode == 1) // if(mode == 2)
    // move light 1 or light 2 around
    
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
    glutPostRedisplay();
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
    glutMainLoop();
    return 0;
}
