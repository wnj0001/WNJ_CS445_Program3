/*********************************************************************

   Software Architecture Statement:
   

   WNJ  03/2014

 ********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
#include "my_setup_3D.h"

//  Constants for use with the my_setup() function.
#define canvas_Width 400
#define canvas_Height 400
#define canvas_Name "Shell Game"


typedef struct {
    float x;
    float y;
    float z;
} Point;


typedef struct {
    float red;
    float green;
    float blue;
} Color;

/*
    
 */
typedef struct {
    Point centerOfBase;
    Point vertices[8];
    float sideLength;
    Color color;
    int position;
    Point translation;
} Cube;

Point* newPoint(float x, float y, float z) {
    Point* point = (Point*)malloc(sizeof(Point));

    point->x = x;
    point->y = y;
    point->z = z;

    return point;
}

Color* newColor(float red, float green, float blue) {
    Color* color = (Color*)malloc(sizeof(Color));

    color->red = red;
    color->green = green;
    color->blue = blue;

    return color;
}

Cube* newCube(Point* centerOfBase, float sideLength, 
              Color* color, int position) {
    Cube* cube = malloc(sizeof(Cube));
    Point vertices[8];
    int h, i, j, k;

    for (h = 0; h < 8; h++) {
        vertices[h].x = 0;
        vertices[h].y = 0;
        vertices[h].z = 0;
    }

    for (i = 0; i < 8; i++) {
        if (i < 4) {
            vertices[i].z = centerOfBase->z;
        }
        else {
            vertices[i].z = centerOfBase->z + sideLength;
        }
    }

    for (j = 0; j < 8; j++) {
        if ((j % 4) == 0) {
            vertices[j].x = centerOfBase->x + (sideLength / 2);
            vertices[j].y = centerOfBase->y + (sideLength / 2);
        }
        else if ((j % 4) == 1) {
            vertices[j].x = centerOfBase->x + (sideLength / 2);
            vertices[j].y = centerOfBase->y - (sideLength / 2);
        }
        else if ((j % 4) == 2) {
            vertices[j].x = centerOfBase->x - (sideLength / 2);
            vertices[j].y = centerOfBase->y - (sideLength / 2);
        }
        else {
            vertices[j].x = centerOfBase->x - (sideLength / 2);
            vertices[j].y = centerOfBase->y + (sideLength / 2);
        }
    }

    for (k = 0; k < 8; k++) {
        cube->vertices[k] = vertices[k];
    }
    cube->centerOfBase = *centerOfBase;
    cube->sideLength = sideLength;
    cube->color = *color;
    cube->position = position;
    cube->translation.x = 0.0;
    cube->translation.y = 0.0;
    cube->translation.z = 0.0;
    return cube;
}

Point* firstShellCenter;
Point* secondShellCenter;
Point* thirdShellCenter;
Point* peaCenter;

Color* bgGray;
Color* white;
Color* pastelMagenta;
Color* limeGreen;
Color* skyBlue;

Cube* shells[3];
Cube* pea;

int rotationSteps = 0;

void drawShell(int shellNum) {
    int i;
    glColor3f(shells[shellNum]->color.red, 
              shells[shellNum]->color.green, 
              shells[shellNum]->color.blue);
    glBegin(GL_POLYGON);
    for(i = 0; i < 8; i++) {
        glVertex3f(shells[shellNum]->vertices[i].x,
                   shells[shellNum]->vertices[i].y,
                   shells[shellNum]->vertices[i].z);
    }
    glEnd();
}

void drawShells() {
    int i;
    for(i = 0; i < 3; i++) {
        printf("Shell: %d Position: %d\n", i, shells[i]->position);
        // Shell is in the Right position
        if(shells[i]->position >= 0 && shells[i]->position < 3) {
            glPushMatrix();
                shells[i]->translation.x -= (86.5 / 3);
                shells[i]->translation.y = 0;
                glTranslatef(shells[i]->translation.x, 
                             shells[i]->translation.y, 
                             0);
                drawShell(i);
            glPopMatrix();
        }
        // Shell is in the Left position
        else if(shells[i]->position >= 3 && shells[i]->position < 6) {
            glPushMatrix();
                shells[i]->translation.x += (43.25 / 3.0);
                shells[i]->translation.y += (75.0 / 3.0);
                glTranslatef(shells[i]->translation.x, 
                             shells[i]->translation.y, 
                             0);
                drawShell(i);
            glPopMatrix();
        }
        // Shell is in the Top position
        else if(shells[i]->position >= 6 && shells[i]->position < 9) {
            glPushMatrix();
                shells[i]->translation.x += (43.25 / 3.0);
                shells[i]->translation.y -= (75.0 / 3.0);
                glTranslatef(shells[i]->translation.x, 
                             shells[i]->translation.y, 
                             0);
                drawShell(i);
            glPopMatrix();
        }
        shells[i]->position++;
        if(shells[i]->position == 9) {
            shells[i]->position = 0;
        }
    }
}

void drawPea() {
    int i;
    glColor3f(pea->color.red,
              pea->color.green,
              pea->color.blue);
    glBegin(GL_POLYGON);
        for(i = 0; i < 8; i++) {
            glVertex3f(pea->vertices[i].x,
                       pea->vertices[i].y,
                       pea->vertices[i].z);
        }
    glEnd();
}

void drawButton(char *string) {
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos3f(340.0, 380.0, -100.0);
    char *c;
    for (c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
    }
    glBegin(GL_LINE_LOOP);
        glVertex3f(330.0, 395.0, -100.0);
        glVertex3f(390.0, 395.0, -100.0);
        glVertex3f(390.0, 375.0, -100.0);
        glVertex3f(330.0, 375.0, -100.0);
    glEnd();
}

/*
    CITATION: The use of the glPolygonMode call comes from the 
              TextBook on page 41.
*/
void initialDraw() {
    glClearColor(bgGray->red, bgGray->green, bgGray->blue, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawButton("Start");
    drawShell(0);
    drawShell(1);
    drawShell(2);
    drawPea();
    glFlush();    
}

void draw() {
    glClearColor(bgGray->red, bgGray->green, bgGray->blue, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawButton("Start");
    drawShells();
    drawPea();
    glFlush();  
}

void animate() {
    if (rotationSteps > 0) {
        draw();
        glutTimerFunc(1000 / 9, animate, 1);
        printf("Steps: %d\n", rotationSteps);
        rotationSteps--;
    }
}

void handleKeys(unsigned char c, GLint x, GLint y) {
    if ((c == 'q') || (c == 'Q'))
    {
        exit(0);
    }
}

void handleMouse(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        if((x > 330.0 && x < 390.0) && (y > (400 - 395.0) && y < (400 - 375.0))) {
            srand(time(NULL));
            int peaShell = (rand() % (3)) + 0;
            pea = newCube(&shells[peaShell]->centerOfBase, 25.0, limeGreen, 0);
            rotationSteps = 3 * ((rand() % (6)) + 3);
            glutTimerFunc(1000 / 9, animate, 1);
        }
    }
}

void init() {
    firstShellCenter = newPoint(243.25, 175, -100);
    secondShellCenter = newPoint(156.75, 175, -100);
    thirdShellCenter = newPoint(200, 250, -100);
    
    bgGray = newColor(0.1, 0.1, 0.15);
    white = newColor(1.0, 1.0, 1.0);
    pastelMagenta = newColor(0.9, 0.6, 0.5);
    limeGreen = newColor(0.5, 0.9, 0.6);
    skyBlue = newColor(0.5, 0.6, 0.9);

    shells[0] = newCube(firstShellCenter, 75.0, white, 0);
    shells[1] = newCube(secondShellCenter, 75.0, pastelMagenta, 3);
    shells[2] = newCube(thirdShellCenter, 75.0, skyBlue, 6);
    
    srand(time(NULL));
    int peaShell = (rand() % (3)) + 0;
    pea = newCube(&shells[peaShell]->centerOfBase, 25.0, limeGreen, 0);
    
}

int main(int argc, char** argv)
{
    init();
    glutInit(&argc, argv);
    my_setup(canvas_Width, canvas_Height, canvas_Name);
    glutDisplayFunc(initialDraw);
    glutKeyboardFunc(handleKeys);
    glutMouseFunc(handleMouse);
    glutMainLoop();
    return 0;
}