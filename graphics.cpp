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
vector<Rect> grid;
vector<Rect> snake;
vector<Rect> lives;
vector<Rect> apples;
int snakeSpeed = 20;

enum direction {
    UP, DOWN,LEFT,RIGHT
};
short snakeDirection = RIGHT;

void spawnNewApple();

void initGrid(){

    // Draw borders with rectangles.
    for(int x = 70; x < width-50; x+=20){
        for(int y = 50; y < height-50; y+=20){
            Rect unit;
            dimensions boxDim(20,20);
            unit.setCenter(x,y);
            unit.setSize(boxDim);
            unit.setColor(white);
            grid.push_back(unit);
        }
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
    lives.at(0).setCenter(460,560);
    lives.at(1).setCenter(490,560);
    lives.at(2).setCenter(520,560);

}

void initSnake(){
    Rect unit;
    dimensions boxDim(20,20);
    unit.setCenter(310,330); //center of screen
    unit.setSize(boxDim);
    unit.setColor(white);
    snake.push_back(unit);

}
void initApple(){
    spawnNewApple();

}
void spawnNewApple() {
    srand(time(NULL));
    if(!apples.empty()){
        apples.pop_back();
    }
    Rect newApple;
    dimensions boxDim(20,20);
    int x,y;
    x = rand() % 460 + 70; // GRID IS OF SIZE 530x530 (I think, math is hard)
    y = rand() % 460 + 70;
    x = ceil(x / 20) * 20 + 10; // MULTIPLES OF 20 plus 10.
    y = ceil(y / 20) * 20 + 10;

    newApple.setCenter(x,y); //center of screen
    newApple.setSize(boxDim);
    newApple.setColor(white);
    apples.push_back(newApple);

}
void increaseSizeOfSnake(){
    Rect unit;
    double x, y; //position
    dimensions boxDim(20,20);
    x = snake.back().getCenterX() - 20;
    y = snake.back().getCenterY();
    unit.setCenter(x,y); //center of screen
    unit.setSize(boxDim);
    unit.setColor(white);
    snake.push_back(unit);
}
void drawLabel(char *label, int x, int y){
    char *c;
    glPushMatrix();
    glTranslatef(x, y+8,0);
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


void init() {
    width = 600;
    height = 600;
    initGrid();
    initSnake();
    initLives();
    initApple();

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
    for(Rect &r : grid){
        r.draw();
    }
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
    // displaying score and lives label.
    drawLabel("Score: ", 55,560);
    drawLabel("Lives: ", 380,560);

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

    for (auto & i : snake) {
        // Close window/exit game if snake hits a wall
        // X-axis
        if(i.getRightX() > width-70 || i.getLeftX() < 70 ){
            if(!isGameOver()){
                lives.pop_back();
                i.setCenter(310,330);
            }
            else{
                glutDestroyWindow(wd);
                exit(0);
            }

        }
        // Y-axis
        if(i.getTopY() > height-100 || i.getBottomY() < 70){
            if(!isGameOver()){
                lives.pop_back();
                i.setCenter(310,330);
            }
            else{
                glutDestroyWindow(wd);
                exit(0);
            }
        }
        if(i.isOverlapping(apples.back())){
            spawnNewApple();
        }
        if(snakeDirection == RIGHT){
            i.moveX(snakeSpeed);
        }
        else if(snakeDirection == LEFT){
            i.moveX(-snakeSpeed);
        }
        else if(snakeDirection == UP){
            i.moveY(-snakeSpeed);
        }
        else if(snakeDirection == DOWN){
            i.moveY(snakeSpeed);
        }

    }
    glutPostRedisplay();
    glutTimerFunc(100, snakeTimer, dummy); //increased to 90, otherwise zoom zoom
}



/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    
    init();
    
    glutInit(&argc, argv);          // Initialize GLUT
    
    glutInitDisplayMode(GLUT_RGBA);
    
    glutInitWindowSize((int)width, (int)height);
    glutInitWindowPosition(0, 0); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Sanke!" /* title */ );
    
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
