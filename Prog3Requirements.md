#   CS445 - Program 3 Requirements


##  Assignment:
    In this assignment, you'll write a short program that implements an animation of a simple game. The game involves hiding a pea-like object, scrambling up that shell among other shells, and then seeing if someone can guess where the shell went.

    The program will be implemented in OpenGL with GLUT and must run on the Linux workstations in the CS Department.

    Your program should be well-structured. Be sure to document the major variables and data structures. Using accurate, descriptive variables and function/method names can help reduce your need to explicitly document the non-major variables. Please note that the program must include an architecture statement, as defined later in this assignment handout.

##  Shell Particulars:
    There are three shells. Each is a size 75 by 75 by 75 cube. They are axis-aligned. They are rendered in wireframe mode, just as we did with the spinner, although now there are three clear dimensions, at least for perspective viewing. They are to be distinctive from the background, but a different color from the pea. At start, the center of the base of the shells are at positions: (243.25, 175, -100), (156.75, 175, -100), and (200, 250, -100). Thus, the shells rest on the z = -100 plane. If a pea is underneath a shell resting on this plane, the pea will not be drawn. (When shells are lifted, exposing a pea, the pea is drawn, however.)

##  Pea Particulars:
    The pea will be modeled as a simplified 25 x 25 x 25 axis aligned cube. It will be rendered in the wireframe mode. The pea is to be green, but only drawn to the screen while the shell it is hidden under is lifted.

##  User Interface:
    The canvas is to be 400 x 400 pixels. There is to be a rectangular box in the upper right corner with the word "START" in it. This box will act as a button. If the user releases the mouse button within the box area, an animation will begin. Details of the animation are presented in the Animation section. Note that each time the user presses the "START" button, the animation indicated below begins. The START letters and button outline are to be in a color distinctive from the background, differing from the pea and shell.

##  Viewing:
    For orthographic viewing, the center line of the view volume is a line offset from the z axis by 282.84 units - it is the line through (200, 200, 0) and through (200, 200, -100). This center line passes through the center of the view window. The front window of the view volume is size 400 by 400, centered about the location the center line intersects the front plane. The front plane is the plane z = 100. The back plane is z = -300.

##  Animation:
    When the user clicks the START button, the program will animate the shells moving. The shells move between the three base positions in a clockwise fashion. You will animate the two in-between frames between each base position. The rate of movement is 1.00 seconds to make a complete cycle -- there are thus 9 frames rendered each 1.00 seconds of movement. Each frame should persist for approximately 0.11 seconds before the next frame is drawn.

    The duration of movement is to be between 1.0 and 2.67 seconds. Use a random number generator to determine the multiple of 0.33 seconds used, with a new random number being sampled from it for each animation.

    As the shells are moved, they location of the pea must be tracked by your program.

##  Extras:
    To encourage you to learn more about graphics, you may achieve up to 2 extra credit points on this assignment for enhancements that go beyond the base requirements. One highly desired extra feature is a scoring feature, with the score displayed on-screen. If scoring is implemented. add 10 points for each time a user guesses correctly on the first try after an animation is completed, add 5 points if on the second try, and add 0 if it is on the 3rd try.

    Another good extra is to have a see through shell tied to some key ... this would allow the user to see the pea move with the shell.

    All extra credit must be recorded in the comment header.



##  Individual Requirements:

    - Program
        - Viewing
            - Shells and Pea are in Perspective
            - UI elements are in Orthographic
            - 
            
        - Shells 
            - Shape: Cube
            - Size: 75px ^ 3
            - Orientation: Axis-aligned
            - Appearance: Wireframe, Pastel Sky Blue
            - Locations:
                - 1: (243.25, 175, -100)
                - 2: (156.75, 175, -100)
                - 3: (200.00, 250, -100)
                
        - Pea
            - Shape: Cube
            - Size: 25px ^3
            - Orientation: Axis-aligned
            - Appearance: Wireframe, Pastel Lime Green
            - Location: Bottom Center of some Shell
            - Hidden when under a shell, but drawn when shell is hidden

        - 