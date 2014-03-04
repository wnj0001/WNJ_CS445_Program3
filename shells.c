/*********************************************************************

    Software Architecture Statement:
        This program is a simulation of a "Shell" Game, where a pea is 
    placed under one of three identical shells, the shells are mixed
    up, and then the player must determine which shell the pea is 
    hidden under. After the objects are initialized, the draw() 
    function uses the drawObjects(), drawButton(), drawScoreboard(), 
    and drawInstructions() functions to draw the initial scene. At any
    time, the handleKeys() function can pick up a press of the 'P' 
    key, which calls the perspectiveMode() function and changes the 
    world objects to be drawn in a perspective viewing mode. 
        When the "Start" button in the top-left corner is clicked, the
    Pea object is initialized and attached to the first shell object, 
    then a random number of rotationSteps is generated, so that the 
    total animation lasts between 1 and 2.67 seconds (3*(3-8) steps).
    The animate() function is then called after 0.11 seconds. The 
    animate() function repeats every 1/9th of a second while there are 
    still more rotations left in the rotationSteps variable. The 
    animate() function calls the update() and draw() functions each 
    time it executes. Inside the update() function, the shells and pea
    are translated between three major positions, with two minor 
    positions in-between. The draw() function draws the shells and pea
    in either orthographic or perspective modes, depending on the 
    current value of the isPerspectiveView flag, and then the start 
    button, scoreboard info, and help messages are drawn in 
    orthographic mode.
        The handleKeys() function detects a press of the 'Q' for 
    closing the program, 'S' for toggling the visibility of the pea 
    during the mixing animation, and 'P' for toggling between 
    perspective and orthographic view modes. The handleMouse() 
    function detects clicks on the start button, and clicks on each
    possible shell position.
    
    Extra Features:
        A scoring system has been implemented, with a notice of when 
    a correct guess has been made, or when the user has run out of 
    guesses. When the user guesses the correct shell on the first try, 
    they receive 10 points; and when they guess correctly on the 
    second try, they receive 5 points. A guess after this point 
    receives no points. The ability to toggle the visibility of the 
    Pea during the animation is also an extra feature, and can be 
    activated by pressing the 'S' key and then pressing the start 
    button.
    
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

// Represents a point in 3-Dimensional space.
typedef struct {
    float x;
    float y;
    float z;
} Point;

// Represents a rgb-style color.
typedef struct {
    float red;
    float green;
    float blue;
} Color;

// Represents a 3-Dimensional cube object and its attributes.
typedef struct {
    Point centerOfBase;
    Point vertices[8];
    float sideLength;
    Color color;
    int position;
    Point translation;
} Cube;

// Pointers to Point objects that represent the centers of the shells' 
// and pea's bases.
Point* firstShellCenter;
Point* secondShellCenter;
Point* thirdShellCenter;
Point* peaCenter;

// Pointers to Color objects that represent the colors used in the 
// program.
Color* bgGray;
Color* white;
Color* pastelMagenta;
Color* limeGreen;
Color* skyBlue;

// Pointers to Cube objects that represent the three shells and the pea.
Cube* shells[3];
Cube* pea;

// Represents the remaining number of steps in the shell 
// mixing animation.
int rotationSteps = 0;

// Represents the shell that the pea is hidden under.
int peaShell;

// A boolean flag of whether the Pea is visible even during the mixing
// animation and guessing stage.
int isPeaAlwaysVisible = 0;

// A boolean flag of whether the Pea is currently being drawn.
int isPeaShowing = 1;

// A boolean flag of whether the shells and pea should be drawn in 
// a perspective viewing mode.
int isPerspectiveView = 0;

// A boolean flag indicating that the player guessed correctly.
int scored = 0;

// A boolean flag indicating that the player ran out of guesses.
int failed = 0;

// Represents the number of guesses that the player currently has.
int guessesLeft = 0;

// Represents the player's current score.
int playerScore;


// ------------------------------------
// -------> Utility Functions <--------
// ------------------------------------


// Used as a constructor to initialize a new Point object.
Point* newPoint(float x, float y, float z) {
    Point* point = (Point*)malloc(sizeof(Point));

    point->x = x;
    point->y = y;
    point->z = z;

    return point;
}

// Used as a constructor to initialize a new Color object.
Color* newColor(float red, float green, float blue) {
    Color* color = (Color*)malloc(sizeof(Color));

    color->red = red;
    color->green = green;
    color->blue = blue;

    return color;
}

// Used as a constructor to initialize a new Cube object.
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

// This function enables a perspective drawing mode for all following 
// objects that are drawn.
void perspectiveMode() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(58.0, canvas_Width/canvas_Height, 100.0, 300.0); 
    glTranslatef(-200.0, -200.0, -200.0);
    glMatrixMode(GL_MODELVIEW);
}

// This function enables a orthographic drawing mode for all following 
// objects that are drawn.
void orthographicMode() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, canvas_Width, 0.0, canvas_Height, 100.0, -300.0);
    glMatrixMode(GL_MODELVIEW);
}

// Moves the objects between the 3 major positions, with two minor
// transition positions between them.
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


// ------------------------------------
// -------> Drawing Functions <--------
// ------------------------------------


// This function draws a cube to the specifications of a Shell.
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

// This function draws a cube to the specifications of the Pea.
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

// This function draws the shells onto the canvas, along with the pea 
// if the pea is flagged to be drawn
void drawObjects() {
    int i;
    for(i = 0; i < 3; i++) {
        drawShell(i);
    }
    if(isPeaShowing) {
        drawPea();
    }
    
    if(scored) {
        drawPea();
    }
    
    if(failed) {
        drawPea();
    }
}

// This function draws the "Start" button onto the canvas.
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

// This function draws the score, number of guesses left, and success
// or failure messages onto the canvas.
void drawScoreboard() {
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

    glRasterPos3f(10.0, 365.0, -100.0);
    string = "Guesses Left: ";
    for (c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
    }

    char* guesses[2];
    sprintf(guesses, "%d", guessesLeft);
    for (c = guesses; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
    }

    if(scored) {
        glRasterPos3f(10.0, 350.0, -100.0);
        string = "You Got It! Press Start to Play Again.";
        for (c = string; *c != '\0'; c++)
        {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
        }
        scored = 0;
        failed = 0;
    }
    else if(failed) {
        glRasterPos3f(10.0, 350.0, -100.0);
        string = "Too Bad... Press Start to Play Again.";
        for (c = string; *c != '\0'; c++)
        {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
        }
        failed = 0;
        scored = 0;
    }
}

// This function draws the keyboard shortcuts onto the 
// bottom of the window.
void drawInstructions() {
    if(isPeaAlwaysVisible) {
        glColor3f(0.2, 1.0, 0.2);
    }
    else {
        glColor3f(1.0, 1.0, 1.0);
    }
    glRasterPos3f(10.0, 10.0, -100.0);
    char* string = "Press S to toggle always visible Pea";
    char *c;
    for (c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
    }

    if(isPerspectiveView) {
        glColor3f(0.2, 1.0, 0.2);
    }
    else {
        glColor3f(1.0, 1.0, 1.0);
    }
    glRasterPos3f(10.0, 25.0, -100.0);
    string = "Press P to toggle Perspective/Orthographic View";
    for (c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
    }
}

// This function draws all of the objects and user interface onto 
// the canvas.
// CITATION: The use of the glPolygonMode call comes from the 
//           TextBook on page 41.
void draw() {
    glClearColor(bgGray->red, bgGray->green, bgGray->blue, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if(isPerspectiveView) {
        perspectiveMode();
    }
    else {
        orthographicMode();
    }
    drawObjects();
    orthographicMode();
    drawButton("Start");
    drawScoreboard();
    drawInstructions();
    glFlush();    
}

// This function updates the the positions of the shells and pea, 
// calls the draw function, and then calls itself again every 0.11 
// seconds as long as there are still more rotationSteps in the 
// current animation.
void animate() {
    if (rotationSteps > 0) {
        update();
        draw();
        glutTimerFunc(1000 / 9, animate, 1);
        printf("Steps: %d\n", rotationSteps);
        rotationSteps--;
    }
}


// ------------------------------------
// -----> User Input Functions <-------
// ------------------------------------


// This callback function allows the program to quit if the 'Q' key is
// pressed, toggle pea visibility during animation if the 'S' key is 
// pressed, and toggle Perspective view if the 'P' key is pressed.
void handleKeys(unsigned char c, GLint x, GLint y) {
    if ((c == 'q') || (c == 'Q'))
    {
        exit(0);
    }

    if ((c == 's') || (c == 'S')) {
        if(!isPeaAlwaysVisible) {
            isPeaAlwaysVisible = 1;
            isPeaShowing = 1;
            glutPostRedisplay();
        }
        else {
            isPeaAlwaysVisible = 0;
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

// This function allows the program to start the animation if the 
// "Start" button is clicked, and registers a guess of whichever shell
// is clicked.
void handleMouse(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        if((x > 330.0 && x < 390.0) && (y > (400 - 395.0) && y < (400 - 375.0))) {
            srand(time(NULL));
            peaShell = 0;
            if(!isPeaAlwaysVisible) {
                isPeaShowing = 0;
            }
            else {
                isPeaShowing = 1;
            }
            Point* peaCenter = &shells[peaShell]->centerOfBase;
            float peaPosition = shells[peaShell]->position;
            pea = newCube(peaCenter, 25.0, limeGreen, peaPosition);
            rotationSteps = 3 * ((rand() % (6)) + 3);
            guessesLeft = 2;
            glutTimerFunc(1000 / 9, animate, 1);
        }

        // Mouse released at bottom right position. This means that the
        // user is guessing that the pea is under the bottom right 
        // shell.
        if((x > shells[0]->vertices[3].x &&
            x < shells[0]->vertices[1].x) &&
            y > (400 - shells[0]->vertices[3].y) &&
            y < (400 - shells[0]->vertices[1].y)) {
            if(shells[peaShell]->position == 0) {
                if(guessesLeft == 2) {
                    scored = 1;
                    playerScore += 10;
                    guessesLeft = 0;
                }
                else if(guessesLeft == 1) {
                    scored = 1;
                    playerScore += 5;
                    guessesLeft = 0;
                }
            }
            else {
                if(guessesLeft > 0) {
                    guessesLeft--;
                }
            }
            if(guessesLeft == 0) {
                failed = 1;
            }
            glutTimerFunc(1000 / 9, draw, 1);
        }

        // Mouse released at bottom left position. This means that the
        // user is guessing that the pea is under the bottom left 
        // shell.
        if((x > shells[1]->vertices[3].x && 
            x < shells[1]->vertices[1].x) &&
            y > (400 - shells[1]->vertices[3].y) && 
            y < (400 - shells[1]->vertices[1].y)) {
            if(shells[peaShell]->position == 3) {
                if(guessesLeft == 2) {
                    scored = 1;
                    playerScore += 10;
                    guessesLeft = 0;
                }
                else if(guessesLeft == 1) {
                    scored = 1;
                    playerScore += 5;
                    guessesLeft = 0;
                }
            }
            else {
                if(guessesLeft > 0) {
                    guessesLeft--;
                }
            }
            if (guessesLeft == 0) {
                failed = 1;
            }
            glutTimerFunc(1000 / 9, draw, 1);
        }

        // Mouse released at top center position. This means that the
        // user is guessing that the pea is under the top center 
        // shell.
        if((x > shells[2]->vertices[3].x && 
            x < shells[2]->vertices[1].x) &&
            y > (400 - shells[2]->vertices[3].y) && 
            y < (400 - shells[2]->vertices[1].y)) {
            if(shells[peaShell]->position == 6) {
                if(guessesLeft == 2) {
                    scored = 1;
                    playerScore += 10;
                    guessesLeft = 0;
                }
                else if(guessesLeft == 1) {
                    scored = 1;
                    playerScore += 5;
                    guessesLeft = 0;
                }
            }
            else {
                if(guessesLeft > 0) {
                    guessesLeft--;
                }
            }
            if (guessesLeft == 0) {
                failed = 1;
            }
            glutTimerFunc(1000 / 9, draw, 1);
        }
    }
}


// ------------------------------------
// --------> Main Functions <----------
// ------------------------------------


// Initializes the objects and variables that will be used
void init() {
    firstShellCenter = newPoint(243.25, 175, -100);
    secondShellCenter = newPoint(156.75, 175, -100);
    thirdShellCenter = newPoint(200, 250, -100);
    
    bgGray = newColor(0.1, 0.1, 0.15);
    white = newColor(1.0, 1.0, 1.0);
    pastelMagenta = newColor(0.9, 0.6, 0.5);
    limeGreen = newColor(0.5, 0.9, 0.6);
    skyBlue = newColor(0.5, 0.6, 0.9);

    shells[0] = newCube(firstShellCenter, 75.0, skyBlue, 0);
    shells[1] = newCube(secondShellCenter, 75.0, skyBlue, 3);
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
    glutDisplayFunc(draw);
    glutKeyboardFunc(handleKeys);
    glutMouseFunc(handleMouse);
    glutMainLoop();
    return 0;
}