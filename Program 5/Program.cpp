// OpenGLTemplate_R1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <GL/glew.h>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/freeglut.h>
#include "OpenGL445Setup-2026.h"


// can customize the below 3 items to make canvas of ones own size and labeling

#define canvas_Width 800
#define canvas_Height 800
char canvas_Name[] = "Program 5 Mini-Modeler";

#define TID_WAIT 1
#define TID_ROT 2

#define INIT_OBJ_SIZE 50.0
#define INIT_OBJ_X 0.0f
#define INIT_OBJ_Y 0.0f


GLdouble g_Object_Max_Size = 200.0;
GLdouble g_Object_Min_Size = 5.0;

GLfloat g_ScreenMargin = 25.0f;
GLfloat g_IconMargin = 10.0f;
GLfloat g_Icon_Outer_Size = 25.0f;

GLfloat g_SphereIcon_x = -(canvas_Width / 2.0f) + g_ScreenMargin + (g_Icon_Outer_Size / 2.0f);
GLfloat g_SphereIcon_y = (canvas_Height / 2.0f) - g_ScreenMargin - (g_Icon_Outer_Size / 2.0f);

GLfloat g_CubeIcon_x = g_SphereIcon_x + g_Icon_Outer_Size + g_IconMargin;
GLfloat g_CubeIcon_y = g_SphereIcon_y;




GLfloat g_ObjectZPos = -400.0f;

GLfloat g_ObjectXPos = INIT_OBJ_X;
GLfloat g_ObjectYPos = INIT_OBJ_Y;

GLfloat g_ObjectRotPos = 0.0f;

GLdouble g_ObjectSize = 50.0;




int g_WaitForRotation = 1000;
int g_RotationTimeout = 42;

bool g_IsRotating = false;

float rotationRate = 10.0f;

char currentObj = 'C';


struct Material {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess;
};





// Color: Copper (Orange)  from https://icolorpalette.com/color/a65d37/
// RGB
// 184, 115, 51
GLfloat cCopper[] = { 184.0f / 255.0f, 155.0f / 255.0f, 51.0f / 255.0f };

Material mCopper = {
    { cCopper[0] / 5.0f, cCopper[1] / 5.0f, cCopper[2] / 5.0f, 1.0f },
    { cCopper[0] / 3.0f, cCopper[1] / 3.0f, cCopper[2] / 3.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f, 1.0f },
    120.0f
};



// Color: Silver (Gray)  from https://icolorpalette.com/color/c0c0c0/
// RGB
// 192, 192, 192
GLfloat cSilver[] = { 192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f };

Material mSilver = {
    { cSilver[0] / 4.0f, cSilver[1] / 4.0f, cSilver[2] / 4.0f, 1.0f },
    { cSilver[0] / 3.0f, cSilver[1] / 3.0f, cSilver[2] / 3.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f, 1.0f },
    120.0f
};




GLfloat lightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightSpecular[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat lightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f };

GLfloat cWhite[3] = { 1.0f, 1.0f, 1.0f };

GLfloat g_Box_Vertices[][3] = {
    {-g_Icon_Outer_Size / 2.0f, -g_Icon_Outer_Size / 2.0f, 0.0f },
    { g_Icon_Outer_Size / 2.0f, -g_Icon_Outer_Size / 2.0f, 0.0f },
    { g_Icon_Outer_Size / 2.0f, g_Icon_Outer_Size / 2.0f, 0.0f },
    { -g_Icon_Outer_Size / 2.0f, g_Icon_Outer_Size / 2.0f, 0.0f }
};

GLfloat g_Box_Colors[][3] = {
    { 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f }
};



GLfloat g_CircleIcon_Vertices[23][3]{
    {7.5000000f, 0.0000000f, 0.0f},
    {7.2444437f, 1.9411143f, 0.0f},
    {6.4951905f, 3.7500000f, 0.0f},
    {5.3033009f, 5.3033009f, 0.0f},
    {3.7500000f, 6.4951905f, 0.0f },
    {1.9411143f, 7.2444437f, 0.0f},
    {0.0000000f, 7.5000000f, 0.0f},
    {-1.9411143f, 7.2444437f, 0.0f},
    {-3.7500000f, 6.4951905f, 0.0f },
    {-5.3033009f, 5.3033009f, 0.0f},
    {-6.4951905f, 3.7500000f, 0.0f},
    {-7.5000000f, 0.0000000f, 0.0f},
    {-7.2444437f, -1.9411143f, 0.0f},
    {-6.4951905f, -3.7500000f, 0.0f},
    {-5.3033009f, -5.3033009f, 0.0f},
    {-3.7500000f, -6.4951905f, 0.0f },
    {-1.9411143f, -7.2444437f, 0.0f},
    {-0.0000000f, -7.5000000f, 0.0f},
    {1.9411143f, -7.2444437f, 0.0f},
    {3.7500000f, -6.4951905f, 0.0f },
    {5.3033009f, -5.3033009f, 0.0f},
    {6.4951905f, -3.7500000f, 0.0f},
    {7.5000000f, 0.0000000f, 0.0f}
};


GLfloat g_CircleIcon_Colors[23][3] = {
    {1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f}
};  // init in main

GLfloat g_SquareIcon_Vertices[][3] = {
    {-7.5f, -7.5f, 0.0f},
    {7.5f, -7.5f, 0.0f},
    {7.5f, 7.5f, 0.0f},
    {-7.5f, 7.5f, 0.0f}
};

GLfloat g_SquareIcon_Colors[][3] = {
    {1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f}
};


GLfloat g_PlusIcon_Vertices[][3] = {
    {-7.5f, 0.0f, 0.0f},
    {7.5f, 0.0f, 0.0f},
    {0.0f, -7.5f, 0.0f},
    {0.0f, 7.5f, 0.0f}
};

GLfloat g_PlusIcon_Colors[][3] = {
    {1.5f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.0f}
};


void setMaterial(Material current) {

    glMaterialfv(GL_FRONT, GL_AMBIENT, current.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, current.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, current.specular);
    glMaterialf(GL_FRONT, GL_SHININESS, current.shininess);
}


void setLighting() {
    // deploy a lighting source at the origin

    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glEnable(GL_LIGHT0);
}


void drawCube(float x, float y, float rotation, float size) {
    glPushMatrix();

    glTranslatef(x, y, g_ObjectZPos);
    glRotatef(rotation, 0.0f, 1.0f, 0.0f);

    // set material values
    //setMaterial(mCopper);
    setMaterial(mSilver);
    glutSolidCube(size / 2.0);

    glPopMatrix();
}




void DrawButtons() {
}


void timerFunc(int timerId) {
    if (timerId == TID_WAIT) {
        g_IsRotating = true;
        glutTimerFunc(g_RotationTimeout, timerFunc, TID_ROT);
    }
    else if (timerId == TID_ROT && g_IsRotating) {
        g_ObjectRotPos += rotationRate;
        glutPostRedisplay();
        glutTimerFunc(g_RotationTimeout, timerFunc, TID_ROT);
    }
}


void drawSphere(float x, float y, float rotation, float size) {

    glPushMatrix();

    glTranslatef(x, y, g_ObjectZPos);
    glRotatef(rotation, 0.0f, 1.0f, 0.0f);

    // set material values
    setMaterial(mCopper);

    glutSolidSphere(size / 2.0, 50, 50);

    glPopMatrix();
}



void objectGeneralInit() {
    g_ObjectXPos = INIT_OBJ_X;
    g_ObjectYPos = INIT_OBJ_Y;
    g_ObjectRotPos = 0.0f;
    g_ObjectSize = INIT_OBJ_SIZE;
    g_IsRotating = false;
    glutTimerFunc(1000, timerFunc, TID_WAIT);
}



void mouseFunction(int button, int state, int x, int y) {
    std::cout << "x: " << x << "   y: " << y << std::endl;
    // TODO:  Determine strategy for mapping screen coordinates to button striker locations.
    // Perhaps calcluate mouse ranges in stertup and do simpler comparisions here

    if (button == GLUT_LEFT_BUTTON && (float)x > (g_CubeIcon_x - (g_Icon_Outer_Size/2.0f)) 
        && (float)x < (g_CubeIcon_x + (g_Icon_Outer_Size/2.0f))
        && (float)y > (g_CubeIcon_y - (g_Icon_Outer_Size/2.0f))
        && (float)y < (g_CubeIcon_y - (g_Icon_Outer_Size / 2.0f)))
    {
        currentObj = 'C';
        objectGeneralInit();
    }
    else if (button == GLUT_LEFT_BUTTON && (float)x > (g_SphereIcon_x - (g_Icon_Outer_Size / 2.0f))
        && (float)x < (g_SphereIcon_x + (g_Icon_Outer_Size / 2.0f))
        && (float)y >(g_SphereIcon_y - (g_Icon_Outer_Size / 2.0f))
        && (float)y < (g_SphereIcon_y - (g_Icon_Outer_Size / 2.0f)))
    {
        currentObj = 'S';
        objectGeneralInit();
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // move the object to equivalent x,y position in the world
        g_ObjectXPos = (GLfloat)x - 400;
        g_ObjectYPos = 400 - (GLfloat)y;
        glutPostRedisplay();
    }
}




void drawUI() {
    // disable the lighting while rendering these items
    glDisable(GL_LIGHTING);

    //// Circle Icon
    //glColor3f(1.0f, 1.0f, 1.0f);

    //glBegin(GL_LINE_LOOP);

    //glVertex3f(g_SphereIcon_x, g_SphereIcon_y, g_ObjectZPos);
    //glVertex3f(g_SphereIcon_x + g_Icon_Outer_Size, g_SphereIcon_y, g_ObjectZPos);
    ////glVertex3f(g_SphereIcon_x + g_Icon_Outer_Size, g_SphereIcon_y, g_ObjectZPos);
    //glVertex3f(g_SphereIcon_x + g_Icon_Outer_Size, g_SphereIcon_y + g_Icon_Outer_Size, g_ObjectZPos);
    //glVertex3f(g_SphereIcon_x, g_SphereIcon_y + g_Icon_Outer_Size, g_ObjectZPos);

    //glEnd();

    //float cen_x = g_SphereIcon_x + (g_Icon_Outer_Size / 2.0f);
    //float cen_y = g_SphereIcon_y + (g_Icon_Outer_Size / 2.0f);


    glPushMatrix();

    // Sphere button
    glTranslatef(g_SphereIcon_x, g_SphereIcon_y, g_ObjectZPos);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, g_Box_Vertices);
    glColorPointer(3, GL_FLOAT, 0, g_Box_Colors);
    glDrawArrays(GL_LINE_LOOP, 0, 4);

    glVertexPointer(3, GL_FLOAT, 0, g_CircleIcon_Vertices);
    glColorPointer(3, GL_FLOAT, 0, g_CircleIcon_Colors);
    glDrawArrays(GL_LINE_LOOP, 0, 23);

    glTranslatef(g_IconMargin + g_Icon_Outer_Size, 0.0f, 0.0f);

    glVertexPointer(3, GL_FLOAT, 0, g_Box_Vertices);
    glColorPointer(3, GL_FLOAT, 0, g_Box_Colors);
    glDrawArrays(GL_LINE_LOOP, 0, 4);

    glVertexPointer(3, GL_FLOAT, 0, g_SquareIcon_Vertices);
    glVertexPointer(3, GL_FLOAT, 0, g_SquareIcon_Colors);
    glDrawArrays(GL_LINE_LOOP, 0, 4);


    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glPopMatrix();



    glEnable(GL_LIGHTING);
}

void display_func()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setLighting();

    if (currentObj == 'S') {
        drawSphere(g_ObjectXPos, g_ObjectYPos, 0.0f, 100);
    }
    else if (currentObj == 'C') {
        drawCube(g_ObjectXPos, g_ObjectYPos, g_ObjectRotPos, 100);
    }



    // draw the user interface, complete with buttons
    drawUI();

    glutSwapBuffers();
    //glFlush();   // may need to uncomment this line!
}


// #define canvas_name "Program 5 Mini-Modeler";


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    my_setup(canvas_Width, canvas_Height, canvas_Name);

    glutDisplayFunc(display_func);
    glutMouseFunc(mouseFunction);


    // temporary just to get things "rolling"
    objectGeneralInit();
    glutMainLoop();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
