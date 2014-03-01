/*********************************************************************

   Software Architecture Statement:
   

   WNJ  03/2014

 ********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <GL/glut.h>
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
    float redValue;
    float greenValue;
    float blueValue;
} Color;

/*
    
 */
typedef struct {
    Point centerOfBase;
    Point vertices[8];
    float sideLength;
    Color color;
} Cube;

Point* newPoint(float x, float y, float z) {
    Point* point = (Point*)malloc(sizeof(Point));

    point->x = x;
    point->y = y;
    point->z = z;

    return point;
}

Color* newColor(float redValue, float greenValue, float blueValue) {
    Color* color = (Color*)malloc(sizeof(Color));

    color->redValue = redValue;
    color->greenValue = greenValue;
    color->blueValue = blueValue;

    return color;
}

Cube* newCube(Point* centerOfBase, float sideLength, Color* color) {
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
    return cube;
}

int main(int argc, char** argv)
{
    Point* firstShellCenter = newPoint(243.25, 175, -100);
    Point* secondShellCenter = newPoint(156.75, 175, -100);
    Point* thirdShellCenter = newPoint(200, 250, -100);
    
    Color* pastelMagenta = newColor(238, 153, 187);
    Color* limeGreen = newColor(153, 238, 187);
    Color* skyBlue = newColor(153, 187, 238);

    Color*  = newColor(255, 255, 255);
    Shell shellList[]
    Cube* firstShell = newCube(firstShellCenter, 75.0, skyBlue);
    Cube* secondShell = newCube(secondShellCenter, 75.0, skyBlue);
    Cube* thirdShell = newCube(thirdShellCenter, 75.0, skyBlue);
    // init();
    // glutInit(&argc, argv);
    // my_setup(canvas_Width, canvas_Height, canvas_Name);
    // glutDisplayFunc(initialDraw);
    // glutKeyboardFunc(handleKeys);
    // glutMainLoop();
    return 0;
}