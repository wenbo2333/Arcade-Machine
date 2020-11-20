#include "circle.h"
#include "rect.h"
#include "graphics.h"
#include <iostream>
#include <vector>
#include <memory>
#include <time.h>
#include <ctime>
#include <string>
using namespace std;

GLdouble width, height;
int wd, score, speed;
int pad1x = 0,pad1y = 250, pad2x = 500, pad2y = 250, velocityX = 3, velocityY = 3, point1 = 0, point2 = 0, ballStart = 1, computerDifficulty = 1;
int gravity = 2;
const color skyBlue(77/255.0, 213/255.0, 240/255.0);
const color grassGreen(26/255.0, 176/255.0, 56/255.0);
const color white(1, 1, 1);
const color brickRed(201/255.0, 20/255.0, 20/255.0);
const color darkBlue(1/255.0, 110/255.0, 214/255.0);
const color purple(119/255.0, 11/255.0, 224/255.0);
const color black(0, 0, 0);
const color magenta(1, 0, 1);
const color orange(1, 163/255.0, 22/255.0);
const color cyan (0, 1, 1);

vector<unique_ptr<Shape>> clouds;
Rect grass;
vector<Rect> buildings1;
vector<Rect> buildings2;
vector<Rect> buildings3;
Rect user;

enum state{menu, flappybird, pong, flappyEnding};
state gameState = menu;

string label, label1, label2;
string lable1p, lable2p, winlablep, winlable2p;
string menuLabel, menuLabel1, menuLabel2;

Rect ball;
Rect paddle1;
Rect paddle2;

void initClouds() {
    // Note: the Rect objects that make up the flat bottom of the clouds
    // won't appear until you implement the Rect::draw method.
    clouds.clear();
    dimensions cloudBottom(30, 30);
    // First cloud
    clouds.push_back(make_unique<Circle>(white, 300, 100, 20));
    clouds.push_back(make_unique<Circle>(white, 330, 100, 20));
    clouds.push_back(make_unique<Circle>(white, 320, 90, 20));
    clouds.push_back(make_unique<Rect>(white, 315, 105, cloudBottom));
    // Second cloud
    clouds.push_back(make_unique<Circle>(white, 100, 80, 20));
    clouds.push_back(make_unique<Circle>(white, 130, 80, 20));
    clouds.push_back(make_unique<Circle>(white, 120, 70, 20));
    clouds.push_back(make_unique<Rect>(white, 115, 85, cloudBottom));
    // Third cloud
    clouds.push_back(make_unique<Circle>(white, 450, 50, 20));
    clouds.push_back(make_unique<Circle>(white, 480, 50, 20));
    clouds.push_back(make_unique<Circle>(white, 470, 40, 20));
    clouds.push_back(make_unique<Rect>(white, 465, 55, cloudBottom));

}

void initGrass() {
    grass.setCenter(250, 450);
    grass.setSize(width, height/3);
    grass.setColor(grassGreen);
}

void initBuildings() {
    // First vector is for closest buildings
    int totalBuildingWidth = 0;
    dimensions buildingSize;
    dimensions buildingSize1;

    // Populate third set of buildings
    totalBuildingWidth = 0;
    while (totalBuildingWidth < width + 2350) {
        // Building height between 200-300
        buildingSize.height = 210 + rand() % (( 290 + 1 ) - 210);
        buildingSize1.height = 210 + rand() % (( 290 + 1 ) - 210);
        buildingSize1.width = 50;
        buildingSize.width = 50;
        buildings3.push_back(Rect(purple,
                                  totalBuildingWidth+(buildingSize.width/2)+5,
                                  height-((buildingSize.height/2)),
                                  buildingSize));
        buildings2.push_back(Rect(brickRed,
                                  totalBuildingWidth+(buildingSize1.width/2)+5,
                                  0,
                                  buildingSize1));
        totalBuildingWidth += buildingSize.width + 5;
        totalBuildingWidth += 5; // Placeholder line to avoid infinite loop
    }
}

void initUser() {
    user.setCenter(100, 200);
    user.setSize(20, 20);
    user.setColor(white);
}

void initPaddle1() {
    //initialize paddle
    paddle1.setCenter(pad1x, pad1y);
    paddle1.setSize(50, 100);
    paddle1.setColor(white);
}

void initPaddle2() {
    //initialize paddle
    paddle2.setCenter(pad2x, pad2y);
    paddle2.setSize(50, 100);
    paddle2.setColor(white);
}

void initBall() {
    //initialize ball
    ball.setCenter(width/2, height/2);
    ball.setSize(20, 20);
    ball.setColor(white);
    if(ballStart % 2 == 0){
        velocityX = 2;
        velocityY = 2;
    }else{
        velocityX = -2;
        velocityY = -2;
    }
    ballStart++;

}

void init() {
    width = 500;
    height = 500;
    speed = -3;
    srand(time(0));
    initClouds();
    initGrass();
    initBuildings();
    point1 = 0;
    point2 = 0;
    winlablep = "";
    winlable2p = "";
    initPaddle1();
    initPaddle2();
    initBall();
}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    gravity = 2;
    // Tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width, height); // DO NOT CHANGE THIS LINE

    // Do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION); // DO NOT CHANGE THIS LINE
    glLoadIdentity(); // DO NOT CHANGE THIS LINE
    glOrtho(0.0, width, height, 0.0, -1.f, 1.f); // DO NOT CHANGE THIS LINE

    // Clear the color buffer with current clearing color
    glClear(GL_COLOR_BUFFER_BIT); // DO NOT CHANGE THIS LINE

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // DO NOT CHANGE THIS LINE

    /*
     * Draw here
     */
    if(gameState == menu){
        menuLabel = "Welcome to the arcade machine!";
        menuLabel1 =  "Press p to play Pong or press f to play Flappy Bird!";
        menuLabel2 = "Press m to return to the menu at any time!";
        glColor3f(1, 0, 0);
        glRasterPos2i(120, 240);
        for (const char &letter : menuLabel) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }

        glColor3f(1, 0, 0);
        glRasterPos2i(30, 250);
        for (const char &letter : menuLabel1) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }

        glColor3f(1, 0, 0);
        glRasterPos2i(75, 260);
        for (const char &letter : menuLabel2) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }

    }

    if(gameState == flappybird){
        for (unique_ptr<Shape> &s : clouds) {
            // #polymorphism
            s->draw();
        }
        grass.draw();

        //draw bottom buildings
        for (int i = 0; i < buildings3.size(); i += 6) {
            if (buildings3[i].isOverlapping(user)) {
                gameState = flappyEnding;
            } else {
                buildings3[i].setColor(purple);
            }
            buildings3[i].draw();

        }


        //draw top buildings
        for (int i = 0; i < buildings2.size(); i += 6) {
            if (buildings2[i].isOverlapping(user)) {
                gameState = flappyEnding;
            } else {
                buildings2[i].setColor(brickRed);
            }
            buildings2[i].draw();

        }
        //draw user
        user.draw();
    }
    if(gameState == flappyEnding){
        for (unique_ptr<Shape> &s : clouds) {
            // #polymorphism
            s->draw();
        }
        grass.draw();
        label = "Game Over";
        label2 = "You Survived " + to_string(score) + " Seconds";
        label1 = "Press f to Play Again";
        speed = -3;
        glColor3f(1, 0, 0);
        glRasterPos2i(width/2 - (4 * label.length()), height/2 + 7);
        for (const char &letter : label) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }

        glRasterPos2i(width/2 - (4 * label2.length()), height/2 + 25);
        for (const char &letter : label2) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }


        glRasterPos2i(width/2 - (4 * label1.length()), height/2 + 43);
        for (const char &letter : label1) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }

    }


    if(gameState == pong){
        paddle1.draw();
        paddle2.draw();
        ball.draw();
        lable1p = "Computer Score: " + to_string(point1);
        glColor3f(1, 0, 0);
        glRasterPos2i(10, 10);
        for (const char &letter : lable1p) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }


        lable2p = "Player Score: " + to_string(point2);
        glColor3f(1, 0, 0);
        glRasterPos2i(width - 160, 10);
        for (const char &letter : lable2p) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }

        if(point1 >= 3){
            winlablep = "Computer Wins!";
            winlable2p = "Press p to play again";
            velocityX = 0;
            velocityY = 0;
        }

        if(point2 >= 3){
            winlablep = "Player Wins!";
            winlable2p = "Press p to play again";
            velocityX = 0;
            velocityY = 0;
        }
        glColor3f(1, 0, 0);
        glRasterPos2i(width/2 - (4 * winlablep.length()), height/2 + 7);
        for (const char &letter : winlablep) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }

        glRasterPos2i(width/2 - (4 * winlable2p.length()), height/2 + 25);
        for (const char &letter : winlable2p) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
        }


        if(ball.getTopY() > paddle1.getCenterY()){
            if(paddle1.getBottomY() <= height) {
                paddle1.move(0, computerDifficulty);
            }
        }

        if(ball.getBottomY() < paddle1.getCenterY()){
            if(paddle1.getTopY() > 0) {
                paddle1.move(0,-computerDifficulty);
            }
        }
    }

    glFlush();  // Render now
}

// http://www.theasciicode.com.ar/ascii-control-characters/escape-ascii-code-27.html
void kbd(unsigned char key, int x, int y) {
    // escape
    if (key == 27) {
        glutDestroyWindow(wd);
        exit(0);
    }
    glutPostRedisplay();
}

void kbdUp(unsigned char key, int x, int y) {

    if (key == 'f') {
        gameState = flappybird;
        glClearColor(skyBlue.red, skyBlue.green, skyBlue.blue, 1.0f);
        initUser();
        score = 0;
        speed = -3;
    }

    if (key == 'p') {
        gameState = pong;
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
        point1 = 0;
        point2 = 0;
        computerDifficulty = 1;
        winlablep = "";
        winlable2p = "";
        initPaddle1();
        initPaddle2();
        initBall();
    }

    if (key == 'm') {
        gameState = menu;
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
    }

    glutPostRedisplay();
}


void kbdS(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_DOWN:
            if(paddle2.getBottomY() <= height) {
                paddle2.move(0, 6);
            }
            break;
        case GLUT_KEY_LEFT:

            break;
        case GLUT_KEY_RIGHT:

            break;
        case GLUT_KEY_UP:
            if(gameState == flappybird){
                user.move(0, -gravity*20);
            }
            if(gameState == pong){
                if(paddle2.getTopY() > 0) {
                    paddle2.move(0,-6);
                }
            }
            break;
    }

    glutPostRedisplay();
}

void cursor(int x, int y) {

    glutPostRedisplay();
}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int state, int x, int y) {

    glutPostRedisplay();
}

void timer(int dummy) {

    glutPostRedisplay();
    glutTimerFunc(30, timer, dummy);
}

void cloudTimer(int dummy) {

    for (unique_ptr<Shape> &s : clouds) {
        // Move all the clouds to the left
        s->moveX(-1);
        // If a shape has moved off the screen
        if (s->getCenterX() < -20) {
            // Set it to the right of the screen so that it passes through again
            s->setCenterX(520);
        }
    }

    glutPostRedisplay();
    glutTimerFunc(50, cloudTimer, dummy);
}

void buildingTimer(int dummy) {

    for (int i = 0; i < buildings3.size(); ++i) {
        // Move all the purple buildings to the left
        buildings3[i].moveX(speed);
        // If a shape has moved off the screen
        if (buildings3[i].getCenterX() < -(buildings3[i].getWidth()/2)) {
            // Set it to the right of the screen so that it passes through again
            int buildingOnLeft = (i == 0) ? buildings3.size()-1 : i - 1;
            buildings3[i].setCenterX(buildings3[buildingOnLeft].getCenterX() + buildings3[buildingOnLeft].getWidth()/2 + buildings3[i].getWidth()/2 + 5);
        }
    }


    for (int i = 0; i < buildings2.size(); ++i) {
        // Move all the purple buildings to the left
        buildings2[i].moveX(speed);
        // If a shape has moved off the screen
        if (buildings2[i].getCenterX() < -(buildings2[i].getWidth()/2)) {
            // Set it to the right of the screen so that it passes through again
            int buildingOnLeft = (i == 0) ? buildings2.size()-1 : i - 1;
            buildings2[i].setCenterX(buildings2[buildingOnLeft].getCenterX() + buildings2[buildingOnLeft].getWidth()/2 + buildings2[i].getWidth()/2 + 5);
        }
    }

    glutPostRedisplay();
    glutTimerFunc(30, buildingTimer, dummy);
}

void scoreTimer(int dummy) {

    if(gameState == flappybird){
        score += 1;
    }
    glutPostRedisplay();
    glutTimerFunc(1000, scoreTimer, dummy);

}

void gameTimer(int dummy) {
    if(gameState == flappybird){
        speed -= 1;
    }
    glutPostRedisplay();
    glutTimerFunc(3000, gameTimer, dummy);
}

void userTimer(int dummy){
    user.move(0,gravity);
    glutPostRedisplay();
    glutTimerFunc(20, userTimer, dummy);
}

void ballTimer(int dummy) {
    ball.move(velocityX, velocityY);
    if(ball.isOverlapping(paddle2)){
        //velocityX += 1;
        velocityX = -velocityX;
    }
    if(ball.isOverlapping(paddle1)){
        //velocityX -= 1;
        velocityX = -velocityX;
    }
    if(ball.getBottomY() >= height){
        velocityY = -velocityY;
    }
    if(ball.getTopY() <= 0){
        velocityY = -velocityY;
    }
    if(ball.getLeftX() < paddle1.getRightX() - 10){
        point2++;
        computerDifficulty++;
        initBall();
        initPaddle1();
        initPaddle2();
        //left boundary
    }
    if(ball.getRightX() > paddle2.getLeftX() + 10){
        point1++;
        initBall();
        initPaddle1();
        initPaddle2();
        //right boundary
    }


    glutPostRedisplay();
    glutTimerFunc(30, ballTimer, dummy);
}


void gameTimerp(int dummy){
    if(velocityX < 0){
        velocityX--;
    }else{
        velocityX++;
    }

    if(velocityY < 0){
        velocityY--;
    }else{
        velocityY++;
    }
    glutPostRedisplay();
    glutTimerFunc(6000, gameTimerp, dummy);
}


/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {

    init();

    glutInit(&argc, argv);          // Initialize GLUT

    glutInitDisplayMode(GLUT_RGBA);

    glutInitWindowSize((int)width, (int)height);
    glutInitWindowPosition(0, 0); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Arcade Machine"/* title */ );

    // Register callback handler for window re-paint event
    glutDisplayFunc(display);

    // Our own OpenGL initialization
    initGL();

    // register keyboard press event processing function
    // works for numbers, letters, spacebar, etc.
    glutKeyboardFunc(kbd);
    glutKeyboardUpFunc(kbdUp);

    // register special event: function keys, arrows, etc.
    glutSpecialFunc(kbdS);

    // handles mouse movement
    glutPassiveMotionFunc(cursor);

    // handles mouse click
    glutMouseFunc(mouse);

    // handles timer
    glutTimerFunc(0, timer, 0);
    glutTimerFunc(0, scoreTimer, 0);
    glutTimerFunc(0, gameTimer, 0);
    glutTimerFunc(0, cloudTimer, 0);
    glutTimerFunc(0, buildingTimer, 0);
    glutTimerFunc(0, userTimer, 0);
    glutTimerFunc(0, ballTimer, 0);
    glutTimerFunc(0, gameTimerp, 0);

    // Enter the event-processing loop
    glutMainLoop();
    return 0;
}