#include "graphics.h"
#include <iostream>
#include "rect.h"
#include <vector>
#include <ctime>

using namespace std;

GLdouble width, height;
int wd;
const color white(1,1,1);
const color black(0,0,0);
vector<Rect> snake;
vector<Rect> lives;
vector<Rect> apples;
vector<Rect> border;
int snakeSpeed = 20;
Rect box;

enum direction {
    UP, DOWN,LEFT,RIGHT
};
short snakeDirection = RIGHT;

void spawnNewApple() {
    srand(time(NULL));
    if(!apples.empty()){
        apples.pop_back();
    }
    Rect newApple;
    dimensions boxDim(20,20);

    int x,y;
    x = rand() % 500 + 50; // BOXES INSIDE GRID RANGE IN 550x550
    y = rand() % 500 + 50;
    x = ceil(x / 20) * 20 + 10; // MULTIPLES OF 20 plus 10.
    y = ceil(y / 20) * 20 + 10;
    newApple.setCenter(x,y); //center of screen
    newApple.setSize(boxDim);
    newApple.setColor(white);
    apples.push_back(newApple);

}

void drawLabel(char *label, int x, int y){
    char *c;
    glPushMatrix();
    glTranslatef(x, y+8,0);
    glColor3f(1,1,1);
    glScalef(0.11f,-0.10f,0);

    for (c=label; *c != ' '; c++)
    {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN , *c);
    }
    glPopMatrix();
}

bool isGameOver(){
    return lives.size() == 1;
}

void initBox(){

    dimensions boxDim(520,520);
    box.setSize(boxDim);
    box.setCenter(300,300);
    box.setColor(white);
}

void initBorder(){

    // Draw top horizontal border.
    for(int x = 30; x < width-30; x+=20){
        Rect unit;
        dimensions boxDim(20,20);
        unit.setCenter(x,30);
        unit.setSize(boxDim);
        unit.setColor(black);
        border.push_back(unit);

    }
    // Draw bottom horizontal border.
    for(int x = 30; x < width-30; x+=20){
        Rect unit;
        dimensions boxDim(20,20);
        unit.setCenter(x,571);
        unit.setSize(boxDim);
        unit.setColor(black);
        border.push_back(unit);
    }
    // Draw left vertical border.
    for(int y = 30; y < height-30; y+=20){
        Rect unit;
        dimensions boxDim(20,20);
        unit.setCenter(29,y);
        unit.setSize(boxDim);
        unit.setColor(black);
        border.push_back(unit);
    }
    // Draw right vertical border.
    for(int y = 30; y < height-30; y+=20){
        Rect unit;
        dimensions boxDim(20,20);
        unit.setCenter(570,y);
        unit.setSize(boxDim);
        unit.setColor(black);
        border.push_back(unit);
    }

}

void initLives(){
    for(int i = 0; i < 3; i++){
        Rect live;
        dimensions boxDim(20,20);
        live.setSize(boxDim);
        live.setColor(white);
        lives.push_back(live);
    }
    lives.at(0).setCenter(110,590);
    lives.at(1).setCenter(140,590);
    lives.at(2).setCenter(170,590);

}

void initSnake(){
    for(int i = 0; i < 4; i++){
        Rect unit;
        dimensions boxDim(20,20);
        unit.setSize(boxDim);
        unit.setColor(white);
        snake.push_back(unit);
    }
    snake.at(0).setCenter(290,270);
    snake.at(1).setCenter(270,270);
    snake.at(2).setCenter(250,270);
    snake.at(3).setCenter(230,270);

}

void increaseSizeOfSnake(){
    Rect unit;
    dimensions boxDim(20,20);
    unit.setSize(boxDim);
    unit.setColor(white);
    snake.push_back(unit);
}
void initApple(){
    spawnNewApple();
}
void drawSnake(){
        // Close window/exit game if snake hits a wall
        // X-axis
        if(snake.front().getLeftX() > width-50.0 || snake.front().getRightX() < 50){
            if(!isGameOver()){
                lives.pop_back();
                snake.front().setCenter(290,270);
            }
            else{
                glutDestroyWindow(wd);
                exit(0);
            }

        }
        // Y-axis
        if(snake.front().getTopY() > height-100 || snake.front().getBottomY() < 50){
            if(!isGameOver()){
                lives.pop_back();
                snake.front().setCenter(290,270);
            }
            else{
                glutDestroyWindow(wd);
                exit(0);
            }
        }
        if(snake.front().isOverlapping(apples.back())){
            spawnNewApple();
            increaseSizeOfSnake();
        }
        if(snakeDirection == RIGHT){
            snake.front().moveX(snakeSpeed);
        }
        else if(snakeDirection == LEFT){
            snake.front().moveX(-snakeSpeed);
        }
        else if(snakeDirection == UP){
            snake.front().moveY(-snakeSpeed);
        }
        else if(snakeDirection == DOWN){
            snake.front().moveY(snakeSpeed);
        }

    for (int i = snake.size()-1; i > 0; i--) {
        // Have body trail the head of the snake.
        snake.at(i).setCenter(snake.at(i-1).getCenter());
    }
}
void init() {
    width = 600;
    height = 650;
    initBox();
    initSnake();
    initLives();
    initApple();
    initBorder();
}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Black and opaque
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    // Tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width, height); // DO NOT CHANGE THIS LINE

    // Do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION); // DO NOT CHANGE THIS LINE
    glLoadIdentity(); // DO NOT CHANGE THIS LINE
    glOrtho(0.0, width, height, 0.0, -1.f, 1.f); // DO NOT CHANGE THIS LINE

    // Clear the color buffer with current clearing color
    glClear(GL_COLOR_BUFFER_BIT); // DO NOT CHANGE THIS LINE


    // DRAW HERE
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // DO NOT CHANGE THIS LINE
    box.draw();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for(Rect &r2 : snake){
        r2.draw();
    }

    for(Rect &r3 : lives){
        r3.draw();
    }
    for(Rect &r4 : apples){
        r4.draw();
    }
    for(Rect &r5: border){
        r5.draw();
    }
    drawSnake();
    drawLabel("Lives: ", 30,590);

    glFlush();  // Render now
}

// http://www.theasciicode.com.ar/ascii-control-characters/escape-ascii-code-27.html
void kbd(unsigned char key, int x, int y)
{
    // escape
    if (key == 27) {
        glutDestroyWindow(wd);
        exit(0);
    }

    glutPostRedisplay();
}

void kbdS(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_DOWN:
            if(snakeDirection!=UP){
                snakeDirection=DOWN;
            }
            break;
        case GLUT_KEY_LEFT:
            if(snakeDirection!=RIGHT){
                snakeDirection=LEFT;
            }
            break;
        case GLUT_KEY_RIGHT:
            if(snakeDirection!=LEFT){
                snakeDirection=RIGHT;
            }
            break;
        case GLUT_KEY_UP:
            if(snakeDirection!=DOWN){
                snakeDirection=UP;
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

void snakeTimer(int dummy) {

    glutPostRedisplay();
    glutTimerFunc(70, snakeTimer, dummy); //increased to 90, otherwise zoom zoom
}



/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {

    init();

    glutInit(&argc, argv);          // Initialize GLUT

    glutInitDisplayMode(GLUT_RGBA);

    glutInitWindowSize((int)width, (int)height);
    glutInitWindowPosition(600, 200); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Snake!");

    // Register callback handler for window re-paint event
    glutDisplayFunc(display);

    // Our own OpenGL initialization
    initGL();

    // register keyboard press event processing function
    // works for numbers, letters, spacebar, etc.
    glutKeyboardFunc(kbd);

    // register special event: function keys, arrows, etc.
    glutSpecialFunc(kbdS);

    // handles mouse movement
    glutPassiveMotionFunc(cursor);

    // handles mouse click
    glutMouseFunc(mouse);

    // handles timer
    glutTimerFunc(0, snakeTimer, 0);

    // Enter the event-processing loop
    glutMainLoop();
    return 0;
}
