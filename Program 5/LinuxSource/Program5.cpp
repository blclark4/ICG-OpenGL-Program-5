#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "OpenGL445Setup-2026.h"




#define canvas_Width 800
#define canvas_Height 800
char canvas_Name[] = "Program 5 Mini-Modeler";



#define TID_ROT 2

#define INIT_OBJ_SIZE 50.0
#define INIT_OBJ_X 0.0f
#define INIT_OBJ_Y 0.0f


GLdouble g_Object_Max_Size = 200.0;
GLdouble g_Object_Min_Size = 5.0;


// Positioning information for the UI
GLfloat g_ScreenMargin = 25.0f;    // space between canvas edge and first button
GLfloat g_IconMargin = 10.0f;      // space between individual buttons
GLfloat g_Icon_Outer_Size = 25.0f; // overall size of each button

// center point of the sphere button x and y
GLfloat g_SphereIcon_x = -(canvas_Width / 2.0f) + g_ScreenMargin + (g_Icon_Outer_Size / 2.0f);
GLfloat g_SphereIcon_y = (canvas_Height / 2.0f) - g_ScreenMargin - (g_Icon_Outer_Size / 2.0f);

// center point of the cube button in x and y
GLfloat g_CubeIcon_x = g_SphereIcon_x + g_Icon_Outer_Size + g_IconMargin;
GLfloat g_CubeIcon_y = g_SphereIcon_y;

// center point of the plus button
GLfloat g_PlusIcon_x = g_SphereIcon_x; // horizontally in line with the sphere button
GLfloat g_PlusIcon_y = -(canvas_Height / 2.0f) + g_ScreenMargin + (g_Icon_Outer_Size / 2.0f);

// center point of the minus button
GLfloat g_MinusIcon_x = g_CubeIcon_x;  // horizontally in line with the cube button
GLfloat g_MinusIcon_y = g_PlusIcon_y;  // vertically in line with the plus icon

// Global flag for turning perspective view on and off
bool g_IsPerpectiveView = false;

// Button hot spots in screen coordinates
// Set by setButtonRanges() at startup
// Array order:  [X-Low, Y-Low, X-High, Y-High]
int sphRng[4];  // sphere range
int cubRng[4];  // cube range

int plsRng[4];  // plus range
int minRng[4];  // minus range


// Global z position for everything
GLfloat g_ObjectZPos = -400.0f;

// Global variables for the current object position
GLfloat g_ObjectXPos = INIT_OBJ_X;
GLfloat g_ObjectYPos = INIT_OBJ_Y;

GLfloat g_ObjectRotPos = 0.0f;   // current object rotational position

GLdouble g_ObjectSize = INIT_OBJ_SIZE;    // current object size


int g_WaitForRotation = 1000;   // msec to wait before starting rotation
int g_RotationTimeout = 42;     // rotation animation timeout

int g_WaitLeft = 0;             // countdown timer for object rotation
bool g_IsRotating = false;      // flag to track whether rotation is happening

float rotationRate = 10.0f;     // rate of rotation per frame

char currentObj = 'C';   // Object being modeled. C = Cube, S = Sphere

// LIGHTING
// Lighting Values for Light Source
GLfloat lightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightSpecular[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat lightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f };


// COLOR AND MATERIAL PROPERTY DATA

GLfloat cWhite[3] = { 1.0f, 1.0f, 1.0f };

// Handy structure object material properties
struct Material {
    char type;  // material code to allow identification
    GLfloat ambient[4];  // ambient reflection values
    GLfloat diffuse[4];  // diffuse reflection values
    GLfloat specular[4]; // specular reflection values
    GLfloat shininess;   // shininess value
};


// Color: Copper (Orange)  from https://icolorpalette.com/color/a65d37/
// RGB
// 184, 115, 51
GLfloat cCopper[] = { 184.0f / 255.0f, 155.0f / 255.0f, 51.0f / 255.0f };

// Setting copper material properties based on cCopper
Material mCopper = {
    'C',
    { cCopper[0] / 4.0f, cCopper[1] / 4.0f, cCopper[2] / 4.0f, 1.0f },
    { cCopper[0] / 2.0f, cCopper[1] / 2.0f, cCopper[2] / 2.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f, 1.0f },
    120.0f
};



// Color: Silver (Gray)  from https://icolorpalette.com/color/c0c0c0/
// RGB
// 192, 192, 192
GLfloat cSilver[] = { 192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f };

// setting silver material properties based on cSilver
Material mSilver = {
    'S',
    { cSilver[0] / 4.0f, cSilver[1] / 4.0f, cSilver[2] / 4.0f, 1.0f },
    { cSilver[0] / 2.0f, cSilver[1] / 2.0f, cSilver[2] / 2.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f, 1.0f },
    120.0f
};


// EXTRA CREDIT:  Global variable to track current material
Material g_ObjectMat = mCopper;


// EXTRA CREDIT:  Global parameter for Camera Position in perspective view
GLfloat g_CameraZPos = 0.0f;
GLfloat g_CameraZMax = 0.0f;
GLfloat g_CameraZMin = -500.0f;


// USER INTERFACE BUTTONS

// VERTEX AND COLOR ARRAYS FOR BUTTONS
// Enclosing box, used for all buttons
GLfloat g_Box_Vertices[][3] = {
    -g_Icon_Outer_Size / 2.0f, -g_Icon_Outer_Size / 2.0f, 0.0f ,
    g_Icon_Outer_Size / 2.0f, -g_Icon_Outer_Size / 2.0f, 0.0f,
    g_Icon_Outer_Size / 2.0f, g_Icon_Outer_Size / 2.0f, 0.0f,
    -g_Icon_Outer_Size / 2.0f, g_Icon_Outer_Size / 2.0f, 0.0f 
};

GLfloat g_Box_Colors[] = {
     1.0f, 1.0f, 1.0f ,
     1.0f, 1.0f, 1.0f ,
     1.0f, 1.0f, 1.0f ,
     1.0f, 1.0f, 1.0f 
};


// Circle Icon - Precalculated points to dispense with need for 
// cmath library
GLfloat g_CircleIcon_Vertices[]{
     7.5000000f,  0.0000000f, 0.0f,
     7.2444437f,  1.9411143f, 0.0f,
     6.4951905f,  3.7500000f, 0.0f,
     5.3033009f,  5.3033009f, 0.0f,
     3.7500000f,  6.4951905f, 0.0f,
     1.9411143f,  7.2444437f, 0.0f,
     0.0000000f,  7.5000000f, 0.0f,
    -1.9411143f,  7.2444437f, 0.0f,
    -3.7500000f,  6.4951905f, 0.0f,
    -5.3033009f,  5.3033009f, 0.0f,
    -6.4951905f,  3.7500000f, 0.0f,
    -7.5000000f,  0.0000000f, 0.0f,
    -7.2444437f, -1.9411143f, 0.0f,
    -6.4951905f, -3.7500000f, 0.0f,
    -5.3033009f, -5.3033009f, 0.0f,
    -3.7500000f, -6.4951905f, 0.0f,
    -1.9411143f, -7.2444437f, 0.0f,
    -0.0000000f, -7.5000000f, 0.0f,
     1.9411143f, -7.2444437f, 0.0f,
     3.7500000f, -6.4951905f, 0.0f,
     5.3033009f, -5.3033009f, 0.0f,
     6.4951905f, -3.7500000f, 0.0f,
     7.5000000f,  0.0000000f, 0.0f
};

// Circle Icon color array
GLfloat g_CircleIcon_Colors[] = {
    1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f
};  // init in main


// Square Icon Vertex array
GLfloat g_SquareIcon_Vertices[] = {
    -7.5f, -7.5f, 0.0f,
     7.5f, -7.5f, 0.0f,
     7.5f,  7.5f, 0.0f,
    -7.5f,  7.5f, 0.0f
};
// Square Icon Color array
GLfloat g_SquareIcon_Colors[] = {
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f
};

// Plus Icon Vertex Array
GLfloat g_PlusIcon_Vertices[] = {
    -7.5f, -1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    -1.0f, -7.5f, 0.0f,
     1.0f, -7.5f, 0.0f,
     1.0f, -1.0f, 0.0f,
     7.5f, -1.0f, 0.0f,
     7.5f,  1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
     1.0f,  7.5f, 0.0f,
    -1.0f,  7.5f, 0.0f,
    -1.0f,  1.0f, 0.0f,
    -7.5f,  1.0f, 0.0f,
    -7.5f, -1.0f, 0.0f
};
// Plus Icon Color Array
GLfloat g_PlusIcon_Colors[] = {
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f
};
// Minus Icon Vertex Array
GLfloat g_MinusIcon_Vertices[] = {
    -7.5f, -1.0f,  0.0f,
     7.5f, -1.0f,  0.0f,
     7.5f,  1.0f,  0.0f,
    -7.5f,  1.0f,  0.0f
};
// Minus Icon Color Array
GLfloat g_MinusIcon_Colors[] = {
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f
};


// COORDINDATE CONVERSION FUNCTIONS
// Mapping between screen coordinates and world coordinates

// Quick conversion from Y at object layer to screen coordinate
// only used when setting button hotspots, so camera position is ignored.
int toMouseY(float y) {
    return (canvas_Height / 2) - (int)y;  // subtract screen Y from half canvas height
}

// Quick conversion from Y in screen coordinates to object layer
// Adjusts for camera position when in perspective mode 
float toWorldY(int y) {
    float camAdjust = 0;
    if (g_IsPerpectiveView) camAdjust = g_CameraZPos;  // in perspective mode, use camera position
    // subtract screen y from half of canvas height, multiply by scaling factor when camera has moved. 
    return ((canvas_Height/2.0f) - (GLfloat)y) * (((canvas_Height / 2.0f) + camAdjust) / (canvas_Height / 2.0f));
}

// Quick conversion from X at object layer to screen coordinate
// only used when setting button hotspots, so camera position is ignored.
int toMouseX(float x) {
    return (int)x + (canvas_Width / 2); // add half canvas size to world X coordinate
}

// Quick conversion from X in screen coordinates to object layer
// Adjusts for cameral position when in perspective mode
float toWorldX(int x) {
    float camAdjust = 0;
    if (g_IsPerpectiveView) camAdjust = g_CameraZPos; // in perspective mode, use camera position
    // subtract half canvase size from screen X, multiply by scaling factor when camera is moved.
    return ((GLfloat)x - (canvas_Width / 2)) * ( ((canvas_Width / 2.0f) + camAdjust) / (canvas_Width / 2.0f));
}


// Sets the current material being used to create objects
void setMaterial(Material current) {

    glMaterialfv(GL_FRONT, GL_AMBIENT, current.ambient);  // set ambient values
    glMaterialfv(GL_FRONT, GL_DIFFUSE, current.diffuse);  // set diffuse values
    glMaterialfv(GL_FRONT, GL_SPECULAR, current.specular);  // set specular values
    glMaterialf(GL_FRONT, GL_SHININESS, current.shininess); // set shininess
}

// Sets the lighting values for the modeled objects in the scene
void setLighting() {
    // deploy a lighting source at the origin

    glEnable(GL_LIGHTING); 

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);   // set ambient lighting for 0
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);   // set diffuse lighting for 0
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular); // set specular lighting for 0
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition); // set position of light source

    glEnable(GL_LIGHT0);  // enable the light source
}


// Draws the cube object in the specified position
void drawCube(float x, float y, float rotation, float size) {
    glPushMatrix(); // ready to go back

    glTranslatef(x, y, g_ObjectZPos);         // Object x,y
    glRotatef(rotation, 0.0f, 1.0f, 0.0f);    // rotate around y

    // set material values
    setMaterial(g_ObjectMat);

    glutSolidCube(size);    // draw the cube

    glPopMatrix();  // reset transformation
}

// Draws the sphere object in the specified position
void drawSphere(float x, float y, float rotation, float size) {

    glPushMatrix();  // save the current transform

    glTranslatef(x, y, g_ObjectZPos);        // Ob
    glRotatef(rotation, 0.0f, 1.0f, 0.0f);

    // set material values
    setMaterial(g_ObjectMat);

    glutSolidSphere(size / 2.0, 10 * size, 10 * size);

    glPopMatrix();
}

// Handles glut timer callbacks, was able to use just one 
// id value.
void timerFunc(int timerId) {
    // the object is rotating  
    if (timerId == TID_ROT && g_IsRotating) {
        g_ObjectRotPos += rotationRate;
        if (g_ObjectRotPos >= 360.0f) g_ObjectRotPos -= 360.0f;
        glutPostRedisplay();
    }
    // the object is waiting to rotate
    else if (timerId == TID_ROT && g_IsRotating == false) {
        g_WaitLeft -= g_RotationTimeout; // reduce wait time
        if (g_WaitLeft <= 0) {
            // done waiting
            g_WaitLeft = 0;      // reset wait time
            g_IsRotating = true; // set the rotation flag
        }
    }
    // set the timer callback
    glutTimerFunc(g_RotationTimeout, timerFunc, TID_ROT); 
}




// object setup and reset operations to handle initial positioning, size and state of the object when first created.
void objectGeneralInit() {
    g_ObjectXPos = INIT_OBJ_X;  // Cardinal x
    g_ObjectYPos = INIT_OBJ_Y;  // Cardinal y
    g_ObjectRotPos = 0.0f;      // Cardinal rotation 
    g_ObjectSize = INIT_OBJ_SIZE;  // Cardinal size
    g_IsRotating = false;    // not rotating
    g_WaitLeft = g_WaitForRotation;  // start rotation countdown
}


// mouse click handler - Handles UI button clicks based on screen ranges calculated at startup. Also performs a repositoknwhene er the click does not match the expected position of a button. screen position ranges are calculated and stored in global var
void mouseFunction(int button, int state, int x, int y) {

    if (button == GLUT_LEFT_BUTTON &&
          x >= sphRng[0] && x <= sphRng[2] && y >= sphRng[1] && y <= sphRng[3]) 
    {
        currentObj = 'S';
        objectGeneralInit();
    }
    else if (button == GLUT_LEFT_BUTTON && 
        x >= cubRng[0] && x <= cubRng[2] && y >= cubRng[1] && y <= cubRng[3]) 
    {
        currentObj = 'C';
        objectGeneralInit();
    }
    else if (button == GLUT_LEFT_BUTTON &&
        x >= plsRng[0] && x <= plsRng[2] && y >= plsRng[1] && y <= plsRng[3])
    {
        if (g_ObjectSize <= g_Object_Max_Size)
            g_ObjectSize += 5;
    }
    else if (button == GLUT_LEFT_BUTTON &&
        x >= minRng[0] && x <= minRng[2] && y >= minRng[1] && y <= minRng[3])
    {
        if (g_ObjectSize >= g_Object_Min_Size)
            g_ObjectSize -= 5;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // move the object to equivalent x,y position in the world
        g_ObjectXPos = toWorldX(x);
        g_ObjectYPos = toWorldY(y);
    }
    glutPostRedisplay();
}




void drawUI() {
    // disable the lighting while rendering these items
    glDisable(GL_LIGHTING);

    glOrtho(-canvas_Width / 2.0, canvas_Width / 2.0, -canvas_Height / 2.0, canvas_Height / 2.0, 0.0, 800.0);
    glLoadIdentity();

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
    glColorPointer(3, GL_FLOAT, 0, g_SquareIcon_Colors);
    glDrawArrays(GL_LINE_LOOP, 0, 4);


    glPopMatrix();
    glPushMatrix();

    glTranslatef(g_PlusIcon_x, g_PlusIcon_y, g_ObjectZPos);

    glVertexPointer(3, GL_FLOAT, 0, g_Box_Vertices);
    glColorPointer(3, GL_FLOAT, 0, g_Box_Colors);
    glDrawArrays(GL_LINE_LOOP, 0, 4);

    glVertexPointer(3, GL_FLOAT, 0, g_PlusIcon_Vertices);
    glColorPointer(3, GL_FLOAT, 0, g_PlusIcon_Colors);
    glDrawArrays(GL_LINE_LOOP, 0, 13);

    glTranslatef(g_IconMargin + g_Icon_Outer_Size, 0.0f, 0.0f);

    glVertexPointer(3, GL_FLOAT, 0, g_Box_Vertices);
    glColorPointer(3, GL_FLOAT, 0, g_Box_Colors);
    glDrawArrays(GL_LINE_LOOP, 0, 4);

    glVertexPointer(3, GL_FLOAT, 0, g_MinusIcon_Vertices);
    glColorPointer(3, GL_FLOAT, 0, g_MinusIcon_Colors);
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

    glTranslatef(0.0f, 0.0f, g_CameraZPos * -1);

    setLighting();

    if (currentObj == 'S') {
        drawSphere(g_ObjectXPos, g_ObjectYPos, g_ObjectRotPos, g_ObjectSize);
    }
    else if (currentObj == 'C') {
        drawCube(g_ObjectXPos, g_ObjectYPos, g_ObjectRotPos, g_ObjectSize);
    }

    // draw the user interface, complete with buttons
    drawUI();

    glutSwapBuffers();
    //glFlush();   // may need to uncomment this line!
}

void setButtonRanges() {
    int halfSpan = g_Icon_Outer_Size / 2 + 1;
    sphRng[0] = toMouseX(g_SphereIcon_x) - halfSpan;
    sphRng[1] = toMouseY(g_SphereIcon_y) - halfSpan;
    sphRng[2] = toMouseX(g_SphereIcon_x) + halfSpan;
    sphRng[3] = toMouseY(g_SphereIcon_y) + halfSpan;

    cubRng[0] = toMouseX(g_CubeIcon_x) - halfSpan;
    cubRng[1] = toMouseY(g_CubeIcon_y) - halfSpan;
    cubRng[2] = toMouseX(g_CubeIcon_x) + halfSpan;
    cubRng[3] = toMouseY(g_CubeIcon_y) + halfSpan;

    plsRng[0] = toMouseX(g_PlusIcon_x) - halfSpan;
    plsRng[1] = toMouseY(g_PlusIcon_y) - halfSpan;
    plsRng[2] = toMouseX(g_PlusIcon_x) + halfSpan;
    plsRng[3] = toMouseY(g_PlusIcon_y) + halfSpan;

    minRng[0] = toMouseX(g_MinusIcon_x) - halfSpan;
    minRng[1] = toMouseY(g_MinusIcon_y) - halfSpan;
    minRng[2] = toMouseX(g_MinusIcon_x) + halfSpan;
    minRng[3] = toMouseY(g_MinusIcon_y) + halfSpan;



}
// #define canvas_name "Program 5 Mini-Modeler";


void my_perspective_projection(int width, int height)
{
    GLdouble width_bound, height_bound;
    width_bound = (GLdouble)width;  height_bound = (GLdouble)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-10, 10, -10, 10, 10, 800);
    
    //gluPerspective(90.0, (GLdouble)width/(GLdouble)height, -1*g_CameraZPos, 800);
    //glFrustum(-width_bound / 2.0, width_bound / 2.0, -height_bound / 2.0, height_bound / 2.0, 10.0, 800.0);
    glMatrixMode(GL_MODELVIEW);
}


void keyboardFunction(unsigned char value, int x, int y) {
    if (value == 'P' || value == 'p') {
        if (g_IsPerpectiveView) {
            glutReshapeFunc(my_3d_projection);
            my_3d_projection(canvas_Width, canvas_Height);
        }
        else
        {
            glutReshapeFunc(my_perspective_projection);
            my_perspective_projection(canvas_Width, canvas_Height);
        }
        g_IsPerpectiveView = !g_IsPerpectiveView;
    }
    // EXTRA CREDIT:  On user input of C, change material
    else if (value == 'C' || value == 'c')
    {
        if (g_ObjectMat.type == 'C') {
            g_ObjectMat = mSilver;
        }
        else if (g_ObjectMat.type == 'S') {
            g_ObjectMat = mCopper;
        }
    }
    // EXTRA CREDIT:  On user input of F, zoom in
    //                and on user input of B, zoom out
    else if (g_IsPerpectiveView && (value == 'F' || value == 'f'))
    {
        if (g_CameraZPos > g_CameraZMin) {
            g_CameraZPos -= 10;
            my_perspective_projection(canvas_Width, canvas_Height);
        }
    }
    else if (g_IsPerpectiveView && (value == 'B' || value == 'b'))
    {
        if (g_CameraZPos < g_CameraZMax)
        {
            g_CameraZPos += 10;
            my_perspective_projection(canvas_Width, canvas_Height);
        }
    }
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    my_setup(canvas_Width, canvas_Height, canvas_Name);

    setButtonRanges();

    glutDisplayFunc(display_func);
    glutMouseFunc(mouseFunction);

    glutTimerFunc(g_RotationTimeout, timerFunc, TID_ROT);
    glutKeyboardFunc(keyboardFunction);
    objectGeneralInit();
    glutMainLoop();
    return 0;
}

