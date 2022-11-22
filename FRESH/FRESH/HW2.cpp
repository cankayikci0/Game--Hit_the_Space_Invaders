/*********
   CTIS164 - Template Source Program
----------
STUDENT :
SECTION :
HOMEWORK: 2
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define START 1
#define RUN 2             //MODE CONSTANTS
#define END 3

#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD  10 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532
//STRUCTURES
typedef struct {
    float x, y;
}pos_t;
typedef struct {
    bool hit;
    pos_t pos;
}enemy_t;
typedef struct {
    pos_t pos;
    float angle;
    bool active;
}fire_t;

fire_t fire;

enemy_t enemy[5] = { {false} };
int mode = START;
/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int number = 0;
double timeCounter = 20;
int  winWidth, winHeight; // current Window width and height
int xShape = 0, xEnemy = -250, xEnemy2 = 250, numofFire = 0, a = 0, hit = 0, yE = 0, num;
bool blink = false;
bool activeTimer = false;
//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

//vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void drawTheEnemy2(enemy_t enemy)
{
    glColor3ub(240, 30, 0);
    glRectf(enemy.pos.x - 15, yE+200, enemy.pos.x + 15, yE+155);
    glRectf(enemy.pos.x - 15, yE+195, enemy.pos.x - 25, yE+155);
    glRectf(enemy.pos.x + 15, yE+195, enemy.pos.x + 25, yE+155);
    glRectf(enemy.pos.x - 25, yE+190, enemy.pos.x - 32, yE+155);
    glRectf(enemy.pos.x + 25, yE+190, enemy.pos.x + 32, yE+155);
    //EYES
    glColor3ub(30, 30, 30);
    glRectf(enemy.pos.x - 25, yE+180, enemy.pos.x - 8, yE+173);
    glRectf(enemy.pos.x + 25, yE+180, enemy.pos.x + 8, yE+173);
    //NOSE
    glRectf(enemy.pos.x-5, yE+170, enemy.pos.x+5, yE+160);
    //TAIL
    glColor3ub(240, 30, 0);
    glRectf(enemy.pos.x - 18, yE+155, enemy.pos.x - 25, yE+140);
    glRectf(enemy.pos.x + 18, yE+155, enemy.pos.x + 25, yE+140);
    glRectf(enemy.pos.x - 10, yE+155, enemy.pos.x + 10, yE+150);
    glRectf(enemy.pos.x - 18, yE+140, enemy.pos.x + 18, yE+133);
}

void drawTheEnemy(enemy_t enemy)
{
    glColor3ub(0, 0, 204);
    glRectf(enemy.pos.x - 15, yE+200, enemy.pos.x + 15, yE+160);
    glRectf(enemy.pos.x - 15, yE+195, enemy.pos.x - 25, yE+160);
    glRectf(enemy.pos.x + 15, yE+195, enemy.pos.x + 25, yE+160);
    glRectf(enemy.pos.x - 25, yE+190, enemy.pos.x - 32, yE+165);
    glRectf(enemy.pos.x + 25, yE+190, enemy.pos.x + 32, yE+165);
    //TAIL                    
    glRectf(enemy.pos.x - 25, yE+160, enemy.pos.x - 35, 140);
    glRectf(enemy.pos.x + 25, yE+160, enemy.pos.x + 35, 140);
    glRectf(enemy.pos.x - 5, yE+160, enemy.pos.x - 15, yE+140);
    glRectf(enemy.pos.x + 5, yE+160, enemy.pos.x + 15, yE+140);
    //EYES
    glColor3ub(30, 30, 30);
    glRectf(enemy.pos.x - 25, yE+180, enemy.pos.x - 8, yE+173);
    glRectf(enemy.pos.x + 25, yE+180, enemy.pos.x + 8, yE+173);

}

void resetTheTarget()
{
    for (int i = 0; i < 5; i++) {
        if (enemy[i].pos.x > 800)
            enemy[i].pos.x = -600;
        if (enemy[i].pos.x > 600 && enemy[i].hit) {
            enemy[i].pos.x = -600;
            enemy[i].hit = false;
        }

    }
    
}

void hitted(enemy_t enemy[], int hitEnemy)
{
    if (enemy[hitEnemy].pos.x != enemy[hitEnemy - 1].pos.x || enemy[hitEnemy].pos.x != enemy[hitEnemy + 1].pos.x)
        enemy[hitEnemy].pos.x = -650;
    else
        enemy[hitEnemy].pos.x = -750;
}

void drawStar()
{
    int x = rand() % 2400 - 1200;
    int y = rand() % 600 - 300;
    glColor4f(1, 1, 1, 0.5);
    circle(x, y, 2);
}

void drawTheShape()
{
    //BODY
    glColor3f(1, 1, 1);
    glRectf(xShape - 20, -240, xShape + 20, -200);
    glRectf(xShape - 10, -200, xShape + 10, -180);
    glRectf(xShape - 2.5, -180, xShape + 2.5, -160);
    glColor3f(1, 1, 0);
    glRectf(xShape - 2.5, -160, xShape + 2.5, -150);
    //WINGS
    //LEFT WING
    glColor3f(1, 0, 0);
    glRectf(xShape + 20, -205, xShape + 30, -235);
    glRectf(xShape + 30, -215, xShape + 40, -245);
    glRectf(xShape + 40, -225, xShape + 50, -255);
    glRectf(xShape + 50, -235, xShape + 60, -265);
    glRectf(xShape + 60, -245, xShape + 70, -285);
    glRectf(xShape + 60, -205, xShape + 70, -185);
    glColor3f(1, 1, 1);
    glRectf(xShape + 60, -245, xShape + 70, -205);
    //RIGHT WING
    glColor3f(1, 0, 0);
    glRectf(xShape - 20, -205, xShape - 30, -235);
    glRectf(xShape - 30, -215, xShape - 40, -245);
    glRectf(xShape - 40, -225, xShape - 50, -255);
    glRectf(xShape - 50, -235, xShape - 60, -265);
    glRectf(xShape - 60, -245, xShape - 70, -285);
    glRectf(xShape - 60, -205, xShape - 70, -185);
    glColor3f(1, 1, 1);
    glRectf(xShape - 60, -245, xShape - 70, -205);
    //BOTTOM
    glColor3f(1, 0, 1);
    glRectf(xShape - 20, -240, xShape + 20, -250);
    glRectf(xShape - 10, -250, xShape + 10, -260);
    glRectf(xShape - 5, -260, xShape + 5, -270);
    //FIRE
    if (blink)
    {
        glColor3f(1, 1, 0);
    }
    else
        glColor3f(1, 0, 0);
    circle(xShape - 65, -292, 5);
    circle(xShape + 65, -292, 5);
}

void drawFire()
{
    glColor3f(1, 0, 1);
    circle(fire.pos.x, fire.pos.y, 15);
}

void displayBackground()
{
    glColor3ub(30, 30, 30);
    glRectf(-600, -300, 600, 300);
    glColor3f(1, 1, 1);
    vprint(500, 270, GLUT_BITMAP_8_BY_13, "%.2f", timeCounter);
    // vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
    for (int i = 0; i < 50; i++)
        drawStar();
    vprint(500, 70, GLUT_BITMAP_8_BY_13, "HIT : %d", hit);
    if (mode == START) {
        glColor3f(1, 0, 1);
        print(200, 270, "Press Spacebar to Start", GLUT_BITMAP_TIMES_ROMAN_24);
    }
}

//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    displayBackground();
    drawTheShape();
    for (int i = 0; i < 5; i++) {
        if (!enemy[i].hit) {
            drawTheEnemy2(enemy[i]);
        }
    }
    //drawTheEnemy2();
    if (fire.active)
        drawFire();
    resetTheTarget();
    //drawTheEnemy2();
    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);
    if (key == ' ') {
        mode = RUN;
        activeTimer = !activeTimer;
        for (int i = 0; i < 5; i++)
            enemy[i].pos.x = -250 * i;

    }


    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    }
    if (key == GLUT_KEY_RIGHT && xShape < 530)
        xShape += 10;
    if (key == GLUT_KEY_LEFT && xShape > -530)
        xShape += -10;
    if (key == GLUT_KEY_F1 && fire.active == false) {
        fire.pos.y = -200;
        fire.pos.x = xShape;
        fire.active = true;
    }



    // to refresh the window it calls display() function
    glutPostRedisplay();
}



//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }
    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

int testCollision(fire_t fire, enemy_t enemy)
{
    if (fire.pos.y > 120 && fire.pos.y < 200 && fire.pos.x > enemy.pos.x-30 && fire.pos.x < enemy.pos.x+30)
        return 1;
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.
    if (activeTimer) {
        timeCounter -= 0.01;
        for (int i = 0; i < 5; i++) {
            enemy[i].pos.x += 4;
        }
    }
    blink = !blink;
    if (fire.active) 
        fire.pos.y += 10;
    if (fire.pos.y > 300)
        fire.active = false;
    for (int i = 0; i < 5; i++) {
        if (testCollision(fire, enemy[i]) == 1) {
            hit++;
            //hitted(enemy, i);
            enemy[i].hit = true;
        }
        
    }
        
    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    srand(time(NULL));
}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Hit the Invaders");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}