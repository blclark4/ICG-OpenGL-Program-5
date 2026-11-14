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



struct Material {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat

};

// Color: Copper (Orange)  from https://icolorpalette.com/color/a65d37/
// RGB
// 184, 115, 51
GLfloat cCopper[] = { 184.0f / 255.0f, 155.0f / 255.0f, 51.0f / 255.0f };



// Color: Silver (Gray)  from https://icolorpalette.com/color/c0c0c0/
// RGB
// 192, 192, 192
GLfloat cSilver[] = { 192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f };




void display_func()


{
    // this is a display callback to draw a line,
    // learner's not: display callbacks are automatically invoked after GLUT finds that a window needs to be
    // displayed or redisplayed
   
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw a short greeen line
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex2i(25, 25);
    glVertex2i(50, 75);
    glEnd();

    glFlush();   // may need to uncomment this line!
}


// #define canvas_name "Program 5 Mini-Modeler";


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    my_setup(canvas_Width, canvas_Height, canvas_Name);

    glutDisplayFunc(display_func);

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
