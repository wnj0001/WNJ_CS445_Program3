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
    int x;
    int y;
    int z;
} Point;

typedef struct {
    Point center;
    
} Shell;

int main(int argc, char** argv)
{
    init();
    glutInit(&argc, argv);
    my_setup(canvas_Width, canvas_Height, canvas_Name);
    glutDisplayFunc(initialDraw);
    glutKeyboardFunc(handleKeys);
    glutMainLoop();
    return 0;
}