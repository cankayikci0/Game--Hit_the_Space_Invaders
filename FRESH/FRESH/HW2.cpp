/*********
   CTIS164 - Template Source Program
----------
STUDENT : CAN KAYIKCI
SECTION : 1
HOMEWORK: 2
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:        1-Shows the score while playing
                            2-Added scoreboard. Keeps the scores up to 12 games. Used bubblesort to display scores
                            in an ascending order.
                            3-The 'score' increases differently depending on the impact.
                            For example, if you hit the target slightly(as hitting its ears), you earn less points.
                            However, if you hit its center or next to the center, you earn more points.
                            4-)User cannot move or fire when game is paused
                            
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
#define PAUSE 4

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

int scores[12]; // ARRAY TO KEEP THE DATA OF SCORES

fire_t fire;
int xStart = -300;
enemy_t enemy[5] = { {false} };
int mode = START;
/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false, tab = false;
int number = 0, playerNum = 0;
//TIME COUNTER -- 20 SECOND
double timeCounter = 20;
int  winWidth, winHeight; // current Window width and height
// NECESSARY INITIALIZATIONS
int xShape = 0, xEnemy = -250, xEnemy2 = 250, hit = 0, yE = 0, num, textY = 5, tester = 0;
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

//FIRST TARGET'S DRAWING
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
//SECOND TARGET'S DRAWING
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
//START SCREEN'S TARGET'S DRAWING
void drawTheEnemySTART()
{
    //line
    glColor3ub(50, 50, 50);
    glLineWidth(5);
    glBegin(GL_LINES);
    glVertex2f(xStart - 60, yE + 180);
    glVertex2f(xStart - 30, yE + 180);
    glEnd();

    glRectf(xStart - 60, yE + 250, xStart - 300, yE + 110);
    glColor3f(1, 1, 1);
    print(xStart - 270, yE + 220, "CAN KAYIKCI", GLUT_BITMAP_TIMES_ROMAN_24);
    print(xStart - 270, yE + 190, "22103969", GLUT_BITMAP_TIMES_ROMAN_24);
    print(xStart - 270, yE + 160, "HOMEWORK-II", GLUT_BITMAP_TIMES_ROMAN_24);

    glColor3ub(0, 0, 204);
    glRectf(xStart - 15, yE + 200, xStart + 15, yE + 160);
    glRectf(xStart - 15, yE + 195, xStart - 25, yE + 160);
    glRectf(xStart + 15, yE + 195, xStart + 25, yE + 160);
    glRectf(xStart - 25, yE + 190, xStart - 32, yE + 165);
    glRectf(xStart + 25, yE + 190, xStart + 32, yE + 165);
    //TAIL                    
    glRectf(xStart - 25, yE + 160, xStart - 35, 140);
    glRectf(xStart + 25, yE + 160, xStart + 35, 140);
    glRectf(xStart - 5, yE + 160, xStart - 15, yE + 140);
    glRectf(xStart + 5, yE + 160, xStart + 15, yE + 140);
    //EYES
    glColor3ub(100, 100, 100);
    glRectf(xStart - 25, yE + 180, xStart - 8, yE + 173);
    glRectf(xStart + 25, yE + 180, xStart+ 8, yE + 173);

}
//RESET TARGET'S X COORDINATES
void resetTheTarget()
{
    //WHEN IT IS GREATER THAN WINDOW WIDTH OR HIT
    for (int i = 0; i < 5; i++) {
        if (enemy[i].pos.x > 800)
            enemy[i].pos.x = -600;
        if (enemy[i].pos.x > 600 && enemy[i].hit) {
            enemy[i].pos.x = -600;
            enemy[i].hit = false;
        }
    }
    //OR WHEN THE GAME RESTARTS
    if (mode == END)
    {
        for (int i = 0; i < 5; i++)
            enemy[i].pos.x = -250 * i;
    }
}
//STAR EFFECT TO FEEL LIKE CHARACTERS ARE MOVING THROUGH SPACE
void drawStar()
{
    int x = rand() % 1200 - 600;
    int y = rand() % 600 - 300;
    glColor4f(1, 1, 1, 0.7);
    circle(x, y, 2);
}
//SPACESHIP'S DRAWING
void drawTheShape()
{
    //BODY
    glColor3f(1, 1, 1);
    glRectf(xShape - 20, -220, xShape + 20, -180);
    glRectf(xShape - 10, -180, xShape + 10, -160);
    glRectf(xShape - 2.5, -160, xShape + 2.5, -140);
    glColor3f(1, 1, 0);
    glRectf(xShape - 2.5, -140, xShape + 2.5, -130);
    //WINGS
    //LEFT WING
    glColor3f(1, 0, 0);
    glRectf(xShape + 20, -185, xShape + 30, -215);
    glRectf(xShape + 30, -195, xShape + 40, -225);
    glRectf(xShape + 40, -205, xShape + 50, -235);
    glRectf(xShape + 50, -215, xShape + 60, -245);
    glRectf(xShape + 60, -225, xShape + 70, -265);
    glRectf(xShape + 60, -185, xShape + 70, -165);
    glColor3f(1, 1, 1);
    glRectf(xShape + 60, -225, xShape + 70, -185);
    //RIGHT WING
    glColor3f(1, 0, 0);
    glRectf(xShape - 20, -185, xShape - 30, -215);
    glRectf(xShape - 30, -195, xShape - 40, -225);
    glRectf(xShape - 40, -205, xShape - 50, -235);
    glRectf(xShape - 50, -215, xShape - 60, -245);
    glRectf(xShape - 60, -225, xShape - 70, -265);
    glRectf(xShape - 60, -185, xShape - 70, -165);
    glColor3f(1, 1, 1);
    glRectf(xShape - 60, -225, xShape - 70, -185);
    //BOTTOM
    glColor3f(1, 0, 1);
    glRectf(xShape - 20, -220, xShape + 20, -230);
    glRectf(xShape - 10, -230, xShape + 10, -240);
    glRectf(xShape - 5, -240, xShape + 5, -250);
    //FIRE
    if (blink)
    {
        glColor3f(255, 240, 0);
    }
    else
        glColor3ub(206, 32, 41);
    circle(xShape - 65, -272, 5);
    circle(xShape + 65, -272, 5);
}
//SPACESHIP'S BULLET
void drawFire()
{
    glColor3ub(240, 240, 0);
    glRectf(fire.pos.x + 5, fire.pos.y + 30, fire.pos.x, fire.pos.y);
}
//SHOW SCOREBOARD WHEN F3 IS PRESSED
void displayscoreBoard()
{
   glColor3ub(70, 70, 70);
   glRectf(-500, 0, -200, -200);
   glColor3f(0, 0, 0);
   vprint(-400, -20, GLUT_BITMAP_8_BY_13, "SCOREBOARD");
   for (int i = 0; i < playerNum; i++)
       vprint(-450, i*-15-32, GLUT_BITMAP_8_BY_13, "%d-) %d", i+1,scores[i]);

}
//BUBBLE SORT THE SCORES TO SHOW IN AN ASCENDING ORDER
void bubbleSort(int scores[])
{
    int sorted, pass = 1, k, temp;
    do {
        sorted = 1;
        for(k = 0; k < playerNum-pass;k++)
            if (scores[k] < scores[k + 1])
            {
                temp = scores[k];
                scores[k] = scores[k+1];
                scores[k + 1] = temp;
                sorted = 0;
            }
        pass++;
    } while (!sorted);
}
//BACKGROUND CONTAINER
void displayBackground()
{
    glColor3ub(30, 30, 30);
    glRectf(-600, -300, 600, 300);
    glColor3f(1, 1, 1);
    //REMAINING TIME
    if (mode == RUN || mode == PAUSE) {
        vprint(300, 270, GLUT_BITMAP_8_BY_13, " REMAINING TIME: %.2f", timeCounter);
    }
    // vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
    if(mode == RUN || mode == PAUSE)
        for (int i = 0; i < 50; i++)
            drawStar();
    //SHOWS THE SCORE
    if(mode == RUN || mode == PAUSE)
        vprint(450, 70, GLUT_BITMAP_8_BY_13, "YOUR SCORE : %d", hit);
    if(mode == END)
        vprint(400, 70, GLUT_BITMAP_8_BY_13, "YOUR SCORE is : %d", hit);
    //SHOWS THE SCOREBOARD
    if (tab || mode == END) {
        bubbleSort(scores);
        displayscoreBoard();
    }

    //INFORMATION
    if (mode == RUN || mode == PAUSE) {
        glColor3f(0, 0, 0);
        glRectf(-600, -277, 600, -300);
        glColor3f(1, 1, 0);
        vprint(-500, -290, GLUT_BITMAP_8_BY_13, "F1 = PAUSE/RESTART  F3 = SCOREBOARD   SPACE = SHOOT  RIGHT/LEFT ARROW = MOVE             CAN KAYIKCI 22103969");
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
    //DRAWS THE ENEMIES
    if (mode == RUN || mode == PAUSE) {
        for (int i = 0; i < 5; i++) {
            if (!enemy[i].hit) {
                if (i % 2 == 1)
                    drawTheEnemy2(enemy[i]);
                else
                    drawTheEnemy(enemy[i]);
            }
        }
    }
    //DRAWS THE BULLET
    if (fire.active)
        drawFire();
    resetTheTarget();
    //INFORMS THE USER WHEN IT IS NECESSARY
    if (mode == START) {
        glColor3ub(100, 100, 100);
        glRectf(600, 300, -600, -300);
        glColor3f(0, 179, 179);
        print(-80, textY-250, "Press F1 to Start", GLUT_BITMAP_HELVETICA_18);
        glColor3ub(26, 0, 26);
        print(-140, 0, "HIT THE SPACE INVADERS", GLUT_BITMAP_TIMES_ROMAN_24);
        drawTheEnemySTART();
    }
    if (mode == END) {
        print(-80, textY - 250, "Press F1 to Start", GLUT_BITMAP_HELVETICA_18);
        print(-100, 50, "GAME ENDED", GLUT_BITMAP_TIMES_ROMAN_24);
    }
   
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
    //FIRES THE BULLET
    if (key == ' ' && fire.active == false && mode == RUN) {
        fire.pos.y = -140;
        fire.pos.x = xShape-2.5;
        fire.active = true;
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
    case GLUT_KEY_F3: tab = true;
    }
    //MOVE THE SPACESHIP
    if (mode == RUN) {
        if (key == GLUT_KEY_RIGHT && xShape < 530)
            xShape += 10;
        if (key == GLUT_KEY_LEFT && xShape > -530)
            xShape += -10;
    }
    //F1 KEY ARRANGEMENTS
    if (key == GLUT_KEY_F1) {
        switch (mode) {
        case START:
            mode = RUN;
            activeTimer = !activeTimer;
            for (int i = 0; i < 5; i++)
                enemy[i].pos.x = -250 * i;
            break;
        case END:
            activeTimer = true;
            mode = RUN;
            hit = 0;
            break;
        case RUN:
            activeTimer = false;
            mode = PAUSE;
            break;
        case PAUSE:
            mode = RUN;
            activeTimer = true;
            
        }
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
    case GLUT_KEY_F3: tab = false;
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
//CHECKS WHETHER THE TARGET IS HIT
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
        //REDUCES TIME COUNTER
        if (timeCounter >= 0)
            timeCounter -= 0.01;
        else {
            //WHEN TIME IS OVER
            mode = END;
            activeTimer = !activeTimer;
            timeCounter = 20;
            scores[playerNum] = hit;
            playerNum++;
        }
        for (int i = 0; i < 5; i++) {
            enemy[i].pos.x += 4;
        }
    }
    blink = !blink;
    if (fire.active && mode == RUN) 
        fire.pos.y += 10;
    if (fire.pos.y > 300)
        fire.active = false;
    //testCollision for every target
    for (int i = 0; i < 5; i++) {
        if (testCollision(fire, enemy[i]) == 1) {
            hit++;
            enemy[i].hit = true;
        }
    }
    //FOR THE MOVING SENTENCE AND TARGET IN THE START SCREEN
    tester++;
    if (tester > 50) {
        textY = -textY;
        tester = 0;
        xStart += 20;
        if (xStart > 600)
            xStart = -600;
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
    glutCreateWindow("Hit the Space Invaders");

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