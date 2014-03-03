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
int peaShell;
int showPea = 1;
int isPerspectiveView = 0;

int guessesLeft = 0;
int playerScore;

void drawShell(int shellNum) {
    glColor3f(shells[shellNum]->color.red, 
              shells[shellNum]->color.green, 
              shells[shellNum]->color.blue);
    glPushMatrix();
        glTranslatef(shells[shellNum]->translation.x, 
                     shells[shellNum]->translation.y, 
                     0);
        // Draw the bottom v(0, 1, 2, 3)
        glBegin(GL_POLYGON);
            glVertex3f(shells[shellNum]->vertices[0].x,
                       shells[shellNum]->vertices[0].y,
                       shells[shellNum]->vertices[0].z);
            glVertex3f(shells[shellNum]->vertices[1].x,
                       shells[shellNum]->vertices[1].y,
                       shells[shellNum]->vertices[1].z);
            glVertex3f(shells[shellNum]->vertices[2].x,
                       shells[shellNum]->vertices[2].y,
                       shells[shellNum]->vertices[2].z);
            glVertex3f(shells[shellNum]->vertices[3].x,
                       shells[shellNum]->vertices[3].y,
                       shells[shellNum]->vertices[3].z);
        glEnd();

        // Draw the top v(4, 5, 6, 7)
        glBegin(GL_POLYGON);
            glVertex3f(shells[shellNum]->vertices[4].x,
                       shells[shellNum]->vertices[4].y,
                       shells[shellNum]->vertices[4].z);
            glVertex3f(shells[shellNum]->vertices[5].x,
                       shells[shellNum]->vertices[5].y,
                       shells[shellNum]->vertices[5].z);
            glVertex3f(shells[shellNum]->vertices[6].x,
                       shells[shellNum]->vertices[6].y,
                       shells[shellNum]->vertices[6].z);
            glVertex3f(shells[shellNum]->vertices[7].x,
                       shells[shellNum]->vertices[7].y,
                       shells[shellNum]->vertices[7].z);
        glEnd();

        // Draw the right v(0, 1, 5, 4)
        glBegin(GL_POLYGON);
            glVertex3f(shells[shellNum]->vertices[0].x,
                       shells[shellNum]->vertices[0].y,
                       shells[shellNum]->vertices[0].z);
            glVertex3f(shells[shellNum]->vertices[1].x,
                       shells[shellNum]->vertices[1].y,
                       shells[shellNum]->vertices[1].z);
            glVertex3f(shells[shellNum]->vertices[5].x,
                       shells[shellNum]->vertices[5].y,
                       shells[shellNum]->vertices[5].z);
            glVertex3f(shells[shellNum]->vertices[4].x,
                       shells[shellNum]->vertices[4].y,
                       shells[shellNum]->vertices[4].z);
        glEnd();

        // Draw the left v(2, 3, 7, 6)
        glBegin(GL_POLYGON);
            glVertex3f(shells[shellNum]->vertices[2].x,
                       shells[shellNum]->vertices[2].y,
                       shells[shellNum]->vertices[2].z);
            glVertex3f(shells[shellNum]->vertices[3].x,
                       shells[shellNum]->vertices[3].y,
                       shells[shellNum]->vertices[3].z);
            glVertex3f(shells[shellNum]->vertices[7].x,
                       shells[shellNum]->vertices[7].y,
                       shells[shellNum]->vertices[7].z);
            glVertex3f(shells[shellNum]->vertices[6].x,
                       shells[shellNum]->vertices[6].y,
                       shells[shellNum]->vertices[6].z);
        glEnd();

        // Draw the back v(0, 3, 7, 4)
        glBegin(GL_POLYGON);
            glVertex3f(shells[shellNum]->vertices[0].x,
                       shells[shellNum]->vertices[0].y,
                       shells[shellNum]->vertices[0].z);
            glVertex3f(shells[shellNum]->vertices[3].x,
                       shells[shellNum]->vertices[3].y,
                       shells[shellNum]->vertices[3].z);
            glVertex3f(shells[shellNum]->vertices[7].x,
                       shells[shellNum]->vertices[7].y,
                       shells[shellNum]->vertices[7].z);
            glVertex3f(shells[shellNum]->vertices[4].x,
                       shells[shellNum]->vertices[4].y,
                       shells[shellNum]->vertices[4].z);
        glEnd();

        // Draw the front v(1, 2, 6, 5)
        glBegin(GL_POLYGON);
            glVertex3f(shells[shellNum]->vertices[1].x,
                       shells[shellNum]->vertices[1].y,
                       shells[shellNum]->vertices[1].z);
            glVertex3f(shells[shellNum]->vertices[2].x,
                       shells[shellNum]->vertices[2].y,
                       shells[shellNum]->vertices[2].z);
            glVertex3f(shells[shellNum]->vertices[6].x,
                       shells[shellNum]->vertices[6].y,
                       shells[shellNum]->vertices[6].z);
            glVertex3f(shells[shellNum]->vertices[5].x,
                       shells[shellNum]->vertices[5].y,
                       shells[shellNum]->vertices[5].z);
        glEnd();
    glPopMatrix();
}

void drawPea() {
    glColor3f(pea->color.red,
              pea->color.green,
              pea->color.blue);
    glPushMatrix();
        glTranslatef(pea->translation.x, 
                     pea->translation.y, 
                     0);

        // Draw the bottom v(0, 1, 2, 3)
        glBegin(GL_POLYGON);
            glVertex3f(pea->vertices[0].x,
                       pea->vertices[0].y,
                       pea->vertices[0].z);
            glVertex3f(pea->vertices[1].x,
                       pea->vertices[1].y,
                       pea->vertices[1].z);
            glVertex3f(pea->vertices[2].x,
                       pea->vertices[2].y,
                       pea->vertices[2].z);
            glVertex3f(pea->vertices[3].x,
                       pea->vertices[3].y,
                       pea->vertices[3].z);
        glEnd();

        // Draw the top v(4, 5, 6, 7)
        glBegin(GL_POLYGON);
            glVertex3f(pea->vertices[4].x,
                       pea->vertices[4].y,
                       pea->vertices[4].z);
            glVertex3f(pea->vertices[5].x,
                       pea->vertices[5].y,
                       pea->vertices[5].z);
            glVertex3f(pea->vertices[6].x,
                       pea->vertices[6].y,
                       pea->vertices[6].z);
            glVertex3f(pea->vertices[7].x,
                       pea->vertices[7].y,
                       pea->vertices[7].z);
        glEnd();

        // Draw the right v(0, 1, 5, 4)
        glBegin(GL_POLYGON);
            glVertex3f(pea->vertices[0].x,
                       pea->vertices[0].y,
                       pea->vertices[0].z);
            glVertex3f(pea->vertices[1].x,
                       pea->vertices[1].y,
                       pea->vertices[1].z);
            glVertex3f(pea->vertices[5].x,
                       pea->vertices[5].y,
                       pea->vertices[5].z);
            glVertex3f(pea->vertices[4].x,
                       pea->vertices[4].y,
                       pea->vertices[4].z);
        glEnd();

        // Draw the left v(2, 3, 7, 6)
        glBegin(GL_POLYGON);
            glVertex3f(pea->vertices[2].x,
                       pea->vertices[2].y,
                       pea->vertices[2].z);
            glVertex3f(pea->vertices[3].x,
                       pea->vertices[3].y,
                       pea->vertices[3].z);
            glVertex3f(pea->vertices[7].x,
                       pea->vertices[7].y,
                       pea->vertices[7].z);
            glVertex3f(pea->vertices[6].x,
                       pea->vertices[6].y,
                       pea->vertices[6].z);
        glEnd();

        // Draw the back v(0, 3, 7, 4)
        glBegin(GL_POLYGON);
            glVertex3f(pea->vertices[0].x,
                       pea->vertices[0].y,
                       pea->vertices[0].z);
            glVertex3f(pea->vertices[3].x,
                       pea->vertices[3].y,
                       pea->vertices[3].z);
            glVertex3f(pea->vertices[7].x,
                       pea->vertices[7].y,
                       pea->vertices[7].z);
            glVertex3f(pea->vertices[4].x,
                       pea->vertices[4].y,
                       pea->vertices[4].z);
        glEnd();

        // Draw the front v(1, 2, 6, 5)
        glBegin(GL_POLYGON);
            glVertex3f(pea->vertices[1].x,
                       pea->vertices[1].y,
                       pea->vertices[1].z);
            glVertex3f(pea->vertices[2].x,
                       pea->vertices[2].y,
                       pea->vertices[2].z);
            glVertex3f(pea->vertices[6].x,
                       pea->vertices[6].y,
                       pea->vertices[6].z);
            glVertex3f(pea->vertices[5].x,
                       pea->vertices[5].y,
                       pea->vertices[5].z);
        glEnd();
    glPopMatrix();
}

void update() {
    int i;
    for(i = 0; i < 3; i++) {
        // Shell is in the Right position
        if(shells[i]->position >= 0 && shells[i]->position < 3) {
            shells[i]->translation.x -= (86.5 / 3);
            if(i == peaShell) {
                pea->translation.x = shells[i]->translation.x;
                pea->translation.y = shells[i]->translation.y;
                pea->translation.z = shells[i]->translation.z;
            }
        }
        // Shell is in the Left position
        else if(shells[i]->position >= 3 && shells[i]->position < 6) {
            shells[i]->translation.x += (43.25 / 3.0);
            shells[i]->translation.y += (75.0 / 3.0);
            if(i == peaShell) {
                pea->translation.x = shells[i]->translation.x;
                pea->translation.y = shells[i]->translation.y;
                pea->translation.z = shells[i]->translation.z;
            }
        }
        // Shell is in the Top position
        else if(shells[i]->position >= 6 && shells[i]->position < 9) {
            shells[i]->translation.x += (43.25 / 3.0);
            shells[i]->translation.y -= (75.0 / 3.0);
            if(i == peaShell) {
                pea->translation.x = shells[i]->translation.x;
                pea->translation.y = shells[i]->translation.y;
                pea->translation.z = shells[i]->translation.z;
            }
        }
        shells[i]->position++;
        if(shells[i]->position == 9) {
            shells[i]->position = 0;
        }
    }   
}

void drawObjects() {
    int i;
    for(i = 0; i < 3; i++) {
        drawShell(i);
    }
    if(showPea) drawPea();
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

void drawScore() {
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos3f(10.0, 380.0, -100.0);
    char *string = "Score: ";
    char *c;
    for (c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
    }
    char* score[20];
    sprintf(score, "%d", playerScore);
    for (c = score; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
    }
}

void drawPeaCheatText(char *string) {
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos3f(10.0, 10.0, -100.0);
    char *c;
    for (c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
    }
}

void perspectiveMode() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(58.0, canvas_Width/canvas_Height, 100.0, 300.0); 
    glTranslatef(-200.0, -200.0, -200.0);
    glMatrixMode(GL_MODELVIEW);
}

void orthographicMode() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, canvas_Width, 0.0, canvas_Height, 100.0, -300.0);
    glMatrixMode(GL_MODELVIEW);
}

/*
    CITATION: The use of the glPolygonMode call comes from the 
              TextBook on page 41.
*/
void initialDraw() {
    glClearColor(bgGray->red, bgGray->green, bgGray->blue, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    perspectiveMode();

    drawObjects();

    orthographicMode();
        
    drawButton("Start");
    drawScore();
    drawPeaCheatText("Press S to toggle Pea in animation.");

    glFlush();    
}

void draw() {
    glClearColor(bgGray->red, bgGray->green, bgGray->blue, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawButton("Start");
    drawScore();
    drawPeaCheatText("Press S to toggle Pea in animation.");
    if(isPerspectiveView) {
    }
    drawObjects();
    glFlush();  
}

void drawAfterGuess() {
    glClearColor(bgGray->red, bgGray->green, bgGray->blue, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawButton("Start");
    drawScore();
    drawPeaCheatText("Press S to toggle Pea in animation.");
    if(isPerspectiveView) {
    }
    drawObjects();
    glFlush();
}

void animate() {
    if (rotationSteps > 0) {
        update();
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

    if ((c == 's') || (c == 'S')) {
        if(!showPea) {
            showPea = 1;
            glutPostRedisplay();
        }
        else {
            showPea = 0;
            glutPostRedisplay();
        }
    }

    if ((c == 'p') || (c == 'P')) {
        if(!isPerspectiveView) {
            isPerspectiveView = 1;
            glutPostRedisplay();
        }
        else {
            isPerspectiveView = 0;
            glutPostRedisplay();
        }
    }}

void handleMouse(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        if((x > 330.0 && x < 390.0) && (y > (400 - 395.0) && y < (400 - 375.0))) {
            srand(time(NULL));
            peaShell = 0;
            Point* peaCenter = &shells[peaShell]->centerOfBase;
            float peaPosition = shells[peaShell]->position;
            pea = newCube(peaCenter, 25.0, limeGreen, peaPosition);
            rotationSteps = 3 * ((rand() % (6)) + 3);
            guessesLeft = 2;
            glutTimerFunc(1000 / 9, animate, 1);
        }

        // Mouse released at bottom right position
        if((x > shells[0]->vertices[3].x &&
            x < shells[0]->vertices[1].x) &&
            y > (400 - shells[0]->vertices[3].y) &&
            y < (400 - shells[0]->vertices[1].y)) {
            if(shells[peaShell]->position == 0) {
                if(guessesLeft == 2) {
                    playerScore += 10;
                    guessesLeft = 0;
                    glutTimerFunc(1000 / 9, drawAfterGuess, 1);
                }
                else if(guessesLeft == 1) {
                    playerScore += 5;
                    guessesLeft = 0;
                    glutTimerFunc(1000 / 9, drawAfterGuess, 1);
                }
            }
            else {
                guessesLeft--;
            }
        }

        // Mouse released at bottom left position
        if((x > shells[1]->vertices[3].x && 
            x < shells[1]->vertices[1].x) &&
            y > (400 - shells[1]->vertices[3].y) && 
            y < (400 - shells[1]->vertices[1].y)) {
            if(shells[peaShell]->position == 3) {
                if(guessesLeft == 2) {
                    playerScore += 10;
                    guessesLeft = 0;
                    glutTimerFunc(1000 / 9, drawAfterGuess, 1);
                }
                else if(guessesLeft == 1) {
                    playerScore += 5;
                    guessesLeft = 0;
                    glutTimerFunc(1000 / 9, drawAfterGuess, 1);
                }
            }
            else {
                guessesLeft--;

            }
            
        }

        // Mouse released at top center position
        if((x > shells[2]->vertices[3].x && 
            x < shells[2]->vertices[1].x) &&
            y > (400 - shells[2]->vertices[3].y) && 
            y < (400 - shells[2]->vertices[1].y)) {
            if(shells[peaShell]->position == 6) {
                if(guessesLeft == 2) {
                    playerScore += 10;
                    guessesLeft = 0;
                    glutTimerFunc(1000 / 9, drawAfterGuess, 1);
                }
                else if(guessesLeft == 1) {
                    playerScore += 5;
                    guessesLeft = 0;
                    glutTimerFunc(1000 / 9, drawAfterGuess, 1);
                }
            }
            else {
                guessesLeft--;
            }
            glutTimerFunc(1000 / 9, drawAfterGuess, 1);
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
    peaShell = 0;
    pea = newCube(&shells[peaShell]->centerOfBase, 25.0, limeGreen, &shells[peaShell]->position);
    
    playerScore = 0;
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