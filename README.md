# Arcade Machine
This program is an arcade machine that allows the user to select from three different games to play, Flappy Bird, Pong, or Space Race.
The high scores from Flappy Bird and Space Race are automatically saved and displayed in the menu. Program was created by Oliver Hamburger on a Windows 10 machine.
NOTE: Classes used were from previous projects, redundant to test when already proven functional.

## Needed to run program
1. Mingw (if on windows)
2. OpenGL and Glut (https://github.com/UVMCS120F2020/Graphics-Starter)

## How to run program
1. Open project in Clion
2. If on Windows, add the freeglut.dll to the cmake-duild-debug folder or else it will not run.
3. Run the project, It should by default select graphics.cpp to run, if not build graphics.cpp then run it.

## Keyboard/Mouse inputs
1. 'm' to go to the menu
2. 'f' to play flappy bird
    a. to play flappy bird, use the up-arrow to jump through the pipes
3. 'p' to play pong
    a. to play pong, us the up-arrow and down-arrow to move the paddle up and down
4. 's' to play space race
    a. to play space race, use all four arrow keys to navigate the spaceship to the other end of the screen
    
## Consepts
Module 4 using graphics to play and draw the games.
Module 2 using is-a relationships for the classes of the shapes, and polymorphism to create and draw a vector of pointers of shapes.

## Citations
M4OEP-Graphics2-ohamburg
M4OEP-Graphics1-ohamburg
Graphics-Starter
Runner-ohamburg
