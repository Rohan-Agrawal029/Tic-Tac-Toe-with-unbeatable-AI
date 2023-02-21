#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>

#define CONFETTI_COUNT 1000
#define CONFETTI_SIZE 0.4

float conf_x[CONFETTI_COUNT], conf_y[CONFETTI_COUNT], conf_z[CONFETTI_COUNT];
float conf_r[CONFETTI_COUNT], conf_g[CONFETTI_COUNT], conf_b[CONFETTI_COUNT];
float conf_rx[CONFETTI_COUNT], conf_ry[CONFETTI_COUNT], conf_rz[CONFETTI_COUNT];
float conf_a[CONFETTI_COUNT];
float top = 1.0;
int counter = 0, start_confetti = 0;

float title_xpos = -30.0, title_zpos = 0.0;
float markWindow_mark1_zpos = 0.0, markWindow_mark2_zpos = 0.0, markWindow_mark3_zpos = 0.0, markWindow_mark4_zpos = 0.0;
float place_mark_zpos = 1.0;
float ypos1 = 2.5, ypos2 = 2.5, zpos = 0.0;
float angle = 0.0;
float speed = 1.0;
int error_flag = 0, mid_flag = 0, state = 1, mark_zstate = 1, title_zstate = 1;
int placed_once, mark_placed = 0;
int current = -1, comp_move = -1;
int p_move_log[5];
int c = 0;
int player = 1, choose_sp;
int p1_mark_choice = 0, p2_mark_choice = 0, comp_mark_choice = 0;
int markWindow, mainWindow, gameWindow, markErrorWindow, gameEndWindow;
int width = 600, height = 600;
int slices = 20, stacks = 20;

int ttt[9], over = 0, winner;
int win_cells[3];

void getValues()
{
    for(int i = 0; i < CONFETTI_COUNT; i++)
    {
        counter += 1;
        conf_x[i] = rand() % 11;
        if(i % 2 == 1)
            conf_x[i] *= -1;
        conf_y[i] = 10 * top;
        conf_z[i] = rand() % -2;
        conf_r[i] = (float)rand() / (float)RAND_MAX;
        conf_g[i] = (float)rand() / (float)RAND_MAX;
        conf_b[i] = (float)rand() / (float)RAND_MAX;
        conf_rx[i] = (float)rand() / (float)RAND_MAX;
        conf_ry[i] = (float)rand() / (float)RAND_MAX;
        conf_rz[i] = (float)rand() / (float)RAND_MAX;
        conf_a[i] = rand() % 360;
        if(counter == 5)
        {
            top += 0.1;
            counter = 0;
        }
    }
}

void clear_board()
{
    ttt[0]='1';ttt[1]='2';ttt[2]='3';ttt[3]='4';ttt[4]='5';
    ttt[5]='6';ttt[6]='7';ttt[7]='8';ttt[8]='9';
    win_cells[0] = -1, win_cells[1] = -1, win_cells[2] = -1;
    c = 0; mid_flag = 0; current = -1;
    over = 0; p1_mark_choice = 0; p2_mark_choice = 0; comp_mark_choice = 0;
    player = 1; choose_sp = -1; comp_move = -1; mark_placed = 0; placed_once = 0;
    top = 1.0; counter = 0; start_confetti = 0;
    getValues();
    for(int i = 0; i < 5; i++)
        p_move_log[i] = 0;
}

void draw_confetti(float x, float y, float z, float r, float g, float b, float a, float rx, float ry, float rz)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(a, rx, ry, rz);
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
        glVertex3f(-CONFETTI_SIZE, CONFETTI_SIZE, 0);
        glVertex3f(-CONFETTI_SIZE, -CONFETTI_SIZE, 0);
        glVertex3f(CONFETTI_SIZE, -CONFETTI_SIZE, 0);
        glVertex3f(CONFETTI_SIZE, CONFETTI_SIZE, 0);
    glEnd();
    glPopMatrix();
}

int win()
{
	if(ttt[0]==ttt[3]&&ttt[3]==ttt[6])
    {
        win_cells[0] = 0, win_cells[1] = 3, win_cells[2] = 6;
        return 1;
    }
	else if(ttt[1]==ttt[4]&&ttt[4]==ttt[7])
    {
        win_cells[0] = 1, win_cells[1] = 4, win_cells[2] = 7;
        return 1;
    }
	else if(ttt[2]==ttt[5]&&ttt[5]==ttt[8])
    {
        win_cells[0] = 2, win_cells[1] = 5, win_cells[2] = 8;
        return 1;
    }
	else if(ttt[0]==ttt[1]&&ttt[1]==ttt[2])
    {
        win_cells[0] = 0, win_cells[1] = 1, win_cells[2] = 2;
        return 1;
    }
	else if(ttt[3]==ttt[4]&&ttt[4]==ttt[5])
    {
        win_cells[0] = 3, win_cells[1] = 4, win_cells[2] = 5;
        return 1;
    }
	else if(ttt[6]==ttt[7]&&ttt[7]==ttt[8])
    {
        win_cells[0] = 6, win_cells[1] = 7, win_cells[2] = 8;
        return 1;
    }
	else if(ttt[0]==ttt[4]&&ttt[4]==ttt[8])
    {
        win_cells[0] = 0, win_cells[1] = 4, win_cells[2] = 8;
        return 1;
    }
	else if(ttt[2]==ttt[4]&&ttt[4]==ttt[6])
    {
        win_cells[0] = 2, win_cells[1] = 4, win_cells[2] = 6;
        return 1;
    }
	else if(ttt[0]!='1'&&ttt[1]!='2'&&ttt[2]!='3'&&ttt[3]!='4'&&ttt[4]!='5'&&ttt[5]!='6'&&ttt[6]!='7'&&ttt[7]!='8'&&ttt[8]!='9')
		return 0;
	else
		return -1;
}

char winnable()
{
    char a, b, c, d, e, f, g, h, i;
    a=ttt[0];b=ttt[1];c=ttt[2];d=ttt[3];e=ttt[4];
    f=ttt[5];g=ttt[6];h=ttt[7];i=ttt[8];
    if(a=='C' && b=='C' && c=='3')
        return '3';
    else if(a=='C' && b=='2' && c=='C')
        return '2';
    else if(a=='1' && b=='C' && c=='C')
        return '1';
    else if(d=='C' && e=='C' && f=='6')
        return '6';
    else if(d=='C' && e=='5' && f=='C')
        return '5';
    else if(d=='4' && e=='C' && f=='C')
        return '4';
    else if(g=='C' && h=='C' && i=='9')
        return '9';
    else if(g=='C' && h=='8' && i=='C')
        return '8';
    else if(g=='7' && h=='C' && i=='C')
        return '7';
    else if(a=='C' && d=='C' && g=='7')
        return '7';
    else if(a=='C' && d=='4' && g=='C')
        return '4';
    else if(a=='1' && d=='C' && g=='C')
        return '1';
    else if(b=='C' && e=='C' && h=='8')
        return '8';
    else if(b=='C' && e=='5' && h=='C')
        return '5';
    else if(b=='2' && e=='C' && h=='C')
        return '2';
    else if(c=='C' && f=='C' && i=='9')
        return '9';
    else if(c=='C' && f=='6' && i=='C')
        return '6';
    else if(c=='3' && f=='C' && i=='C')
        return '3';
    else if(a=='C' && e=='C' && i=='9')
        return '9';
    else if(a=='C' && e=='5' && i=='C')
        return '5';
    else if(a=='1' && e=='C' && i=='C')
        return '1';
    else if(c=='C' && e=='C' && g=='7')
        return '7';
    else if(c=='C' && e=='5' && g=='C')
        return '5';
    else if(c=='3' && e=='C' && g=='C')
        return '3';
    return '0';
}

char loseable()
{
    char a, b, c, d, e, f, g, h, i;
    a=ttt[0];b=ttt[1];c=ttt[2];d=ttt[3];e=ttt[4];
    f=ttt[5];g=ttt[6];h=ttt[7];i=ttt[8];
    if(a=='A' && b=='A' && c=='3')
        return '3';
    else if(a=='A' && b=='2' && c=='A')
        return '2';
    else if(a=='1' && b=='A' && c=='A')
        return '1';
    else if(d=='A' && e=='A' && f=='6')
        return '6';
    else if(d=='A' && e=='5' && f=='A')
        return '5';
    else if(d=='4' && e=='A' && f=='A')
        return '4';
    else if(g=='A' && h=='A' && i=='9')
        return '9';
    else if(g=='A' && h=='8' && i=='A')
        return '8';
    else if(g=='7' && h=='A' && i=='A')
        return '7';
    else if(a=='A' && d=='A' && g=='7')
        return '7';
    else if(a=='A' && d=='4' && g=='A')
        return '4';
    else if(a=='1' && d=='A' && g=='A')
        return '1';
    else if(b=='A' && e=='A' && h=='8')
        return '8';
    else if(b=='A' && e=='5' && h=='A')
        return '5';
    else if(b=='2' && e=='A' && h=='A')
        return '2';
    else if(c=='A' && f=='A' && i=='9')
        return '9';
    else if(c=='A' && f=='6' && i=='A')
        return '6';
    else if(c=='3' && f=='A' && i=='A')
        return '3';
    else if(a=='A' && e=='A' && i=='9')
        return '9';
    else if(a=='A' && e=='5' && i=='A')
        return '5';
    else if(a=='1' && e=='A' && i=='A')
        return '1';
    else if(c=='A' && e=='A' && g=='7')
        return '7';
    else if(c=='A' && e=='5' && g=='A')
        return '5';
    else if(c=='3' && e=='A' && g=='A')
        return '3';
    return '0';
}

void get_computer_move()
{
    char chance;
    chance=winnable();
    if(chance!='0')
    {
        for(int i=0;i<9;i++)
            if(ttt[i]==chance)
            {
                comp_move = i;
                return;
            }
    }
    chance=loseable();
    if(chance!='0')
    {
        for(int i=0;i<9;i++)
            if(ttt[i]==chance)
            {
                comp_move = i;
                return;
            }
    }
    if(c == 1)
    {
        if(ttt[4] == '5')
        {
            comp_move = 4;
            mid_flag = 1;
            for(int i = 0; i < 9; i++)
                if(ttt[i] == 'A')
                    p_move_log[0] = i + 1;
        }
        else
            comp_move = 0;
        return;
    }
    if(c == 2 && mid_flag == 0)
    {
        comp_move = 2;
        return;
    }
    else if(c == 2 && mid_flag == 1)
    {
        for(int i = 0; i < 9; i++)
            if(ttt[i] == 'A' && (i+1) != p_move_log[0])
                p_move_log[1] = (i+1);
        if(p_move_log[0] == 1 && p_move_log[1] == 9)
            comp_move = 1;
        else if(p_move_log[0] == 1 && p_move_log[1] != 9)
            comp_move = 8;
        else if(p_move_log[0] == 9 && p_move_log[1] == 1)
            comp_move = 1;
        else if(p_move_log[0] == 9 && p_move_log[1] != 1)
            comp_move = 0;
        else if(p_move_log[0] == 7 && p_move_log[1] == 3)
            comp_move = 1;
        else if(p_move_log[0] == 7 && p_move_log[1] != 3)
            comp_move = 2;
        else if(p_move_log[0] == 3 && p_move_log[1] == 7)
            comp_move = 1;
        else if(p_move_log[0] == 3 && p_move_log[1] != 7)
            comp_move = 6;
        else if(p_move_log[0] == 2)
            comp_move = 2;
        else if(p_move_log[0] == 4)
            comp_move = 0;
        else if(p_move_log[0] == 8)
            comp_move = 6;
        else if(p_move_log[0] == 6)
            comp_move = 8;
        return;
    }
    for(int i=0;i<9;i++)
    if(ttt[i]!='A' && ttt[i]!='C')
    {
        comp_move = i;
        return;
    }
}

void draw_mark_1(float x, float y, float z, float eye_y, float mouth_y)
{
    GLfloat ctrlpoints1[4][3] = {
        {-0.6, 0.5, -5.0}, {-0.1, 0.22, -5.0},
        {0.1, 0.22, -5.0}, {0.6, 0.5, -5.0}};

    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(1.0, 1.0, 0.0);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glutSolidSphere(1.8, slices, stacks);

    glTranslatef(-0.5, eye_y, 1.6);
    glColor3f(0.0, 0.0, 0.0);
    glutSolidSphere(0.2, slices, stacks);
    glTranslatef(1.0, 0.0, 0.0);
    glutSolidSphere(0.2, slices, stacks);

    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints1[0][0]);
    glTranslatef(-0.5, mouth_y, 5.2);
    glLineWidth(4.0);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
        for(int i = 0; i < 100; i++)
            glEvalCoord1d(((float) i) / 100.0);
    glEnd();
    glPopMatrix();
}

void draw_mark_2(float x, float y, float z, float eye_y, float mouth_y)
{
    GLfloat ctrlpoints2[4][3] = {
        {-0.6, 0.22, -5.0}, {-0.1, 0.5, -5.0},
        {0.1, 0.5, -5.0}, {0.6, 0.22, -5.0}};

    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(0.0, 1.0, 1.0);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glutSolidSphere(1.8, slices, stacks);

    glTranslatef(-0.5, eye_y, 1.6);
    glColor3f(0.0, 0.0, 0.0);
    glutSolidSphere(0.2, slices, stacks);
    glTranslatef(1.0, 0.0, 0.0);
    glutSolidSphere(0.2, slices, stacks);

    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints2[0][0]);
    glTranslatef(-0.5, mouth_y, 5.2);
    glLineWidth(4.0);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
        for(int i = 0; i < 100; i++)
            glEvalCoord1d(((float) i) / 100.0);
    glEnd();
    glPopMatrix();
}

void draw_mark_3(float x, float y, float z, float eye_y)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(1.0, 0.0, 1.0);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glutSolidSphere(1.8, slices, stacks);

    glTranslatef(-0.5, eye_y, 1.6);
    glColor3f(0.0, 0.0, 0.0);
    glutSolidSphere(0.2, slices, stacks);
    glTranslatef(1.0, 0.0, 0.0);
    glutSolidSphere(0.2, slices, stacks);
    glPopMatrix();
}

void draw_mark_4(float x, float y, float z, float eye_y, float mouth_y)
{
    GLfloat ctrlpoints3[4][3] = {
        {-0.6, 0.5, -5.0}, {-0.2, 0.5, -5.0},
        {0.2, 0.5, -5.0}, {0.6, 0.5, -5.0}};

    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(0.0, 1.0, 0.0);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glutSolidSphere(1.8, slices, stacks);

    glTranslatef(-0.5, eye_y, 1.6);
    glColor3f(0.0, 0.0, 0.0);
    glutSolidSphere(0.2, slices, stacks);
    glTranslatef(1.0, 0.0, 0.0);
    glutSolidSphere(0.2, slices, stacks);

    glTranslatef(-0.5, mouth_y, 5.2);
    glLineWidth(4.0);
//    glBegin(GL_LINES);
//        glVertex3f(-0.6, 0.5, -5.0);
//        glVertex3f(0.6, 0.5, -5.0);
//    glEnd();
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints3[0][0]);
    glBegin(GL_LINE_STRIP);
        for(int i = 0; i < 100; i++)
            glEvalCoord1d(((float) i) / 100.0);
    glEnd();
    glPopMatrix();
}

void display_main_title()
{
    glLineWidth(7.0);
    glColor3f(1.0, 0.023, 0.051);
    glBegin(GL_LINES);
        //drawing T (Tic tac toe)
        glVertex3f(-11.5, 7.0, -10.0);
        glVertex3f(-9.5, 7.0, -10.0);

        glVertex3f(-10.5, 7.0, -10.0);
        glVertex3f(-10.5, 4.0, -10.0);

        //drawing I (tIc tac toe)
        glVertex3f(-9, 7.0, -10.0);
        glVertex3f(-7.5, 7.0, -10.0);

        glVertex3f(-8.2, 7.0, -10.0);
        glVertex3f(-8.2, 4.0, -10.0);

        glVertex3f(-9, 4.0, -10.0);
        glVertex3f(-7.5, 4.0, -10.0);

        //drawing C (tiC tac toe)
        glVertex3f(-6.75, 7.0, -10.0);
        glVertex3f(-5.5, 7.0, -10.0);

        glVertex3f(-6.75, 7.0, -10.0);
        glVertex3f(-6.75, 4.0, -10.0);

        glVertex3f(-6.75, 4.0, -10.0);
        glVertex3f(-5.5, 4.0, -10.0);

        //drawing T (tic Tac toe)
        glVertex3f(-3.25, 7.0, -10.0);
        glVertex3f(-1.25, 7.0, -10.0);

        glVertex3f(-2.25, 7.0, -10.0);
        glVertex3f(-2.25, 4.0, -10.0);

        //drawing A (tic tAc toe)
        glVertex3f(-1.0, 4.0, -10.0);
        glVertex3f(0.0, 7.0, -10.0);

        glVertex3f(0.0, 7.0, -10.0);
        glVertex3f(1.0, 4.0, -10.0);

        glVertex3f(-0.5, 5.5, -10.0);
        glVertex3f(0.5, 5.5, -10.0);

        //drawing C (tic taC toe)
        glVertex3f(1.75, 7.0, -10.0);
        glVertex3f(3.0, 7.0, -10.0);

        glVertex3f(1.75, 7.0, -10.0);
        glVertex3f(1.75, 4.0, -10.0);

        glVertex3f(1.75, 4.0, -10.0);
        glVertex3f(3.0, 4.0, -10.0);

        //drawing T (tic tac Toe)
        glVertex3f(5.25, 7.0, -10.0);
        glVertex3f(7.25, 7.0, -10.0);

        glVertex3f(6.25, 7.0, -10.0);
        glVertex3f(6.25, 4.0, -10.0);

        //drawing O (tic tac tOe)
        glVertex3f(7.75, 7.0, -10.0);
        glVertex3f(9.0, 7.0, -10.0);

        glVertex3f(9.0, 7.0, -10.0);
        glVertex3f(9.0, 4.0, -10.0);

        glVertex3f(7.75, 4.0, -10.0);
        glVertex3f(9.0, 4.0, -10.0);

        glVertex3f(7.75, 7.0, -10.0);
        glVertex3f(7.75, 4.0, -10.0);

        //drawing E (tic tac toE)
        glVertex3f(10.0, 7.0, -10.0);
        glVertex3f(11.25, 7.0, -10.0);

        glVertex3f(10.0, 7.0, -10.0);
        glVertex3f(10.0, 4.0, -10.0);

        glVertex3f(10.0, 4.0, -10.0);
        glVertex3f(11.25, 4.0, -10.0);

        glVertex3f(10.0, 5.5, -10.0);
        glVertex3f(10.75, 5.5, -10.0);
    glEnd();
}

void draw_cuboid(float z)
{
    glBegin(GL_QUADS);
        //front
        glColor3f(0.56, 0.305, 0.541);
        glVertex3f(-5.0, 1.0, z);
        glVertex3f(-5.0, -1.0, z);
        glVertex3f(5.0, -1.0, z);
        glVertex3f(5.0, 1.0, z);
        //back
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(-5.0, 1.0, -z);
        glVertex3f(5.0, 1.0, -z);
        glVertex3f(5.0, -1.0, -z);
        glVertex3f(-5.0, -1.0, -z);
        //left
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(-5.0, 1.0, -z);
        glVertex3f(-5.0, -1.0, -z);
        glVertex3f(-5.0, -1.0, z);
        glVertex3f(-5.0, 1.0, z);
        //right
        glColor3f(1.0, 1.0, 0.0);
        glVertex3f(5.0, 1.0, z);
        glVertex3f(5.0, -1.0, z);
        glVertex3f(5.0, -1.0, -z);
        glVertex3f(5.0, 1.0 , -z);
        //top
        glColor3f(0.8, 0.58, 0.8);
        glVertex3f(-5.0, 1.0, -z);
        glVertex3f(-5.0, 1.0, z);
        glVertex3f(5.0, 1.0, z);
        glVertex3f(5.0, 1.0, -z);
        //bottom
        glColor3f(5.0, 0.0, 1.0);
        glVertex3f(-5.0, -1.0, -z);
        glVertex3f(-5.0, -1.0, z);
        glVertex3f(5.0, -1.0, z);
        glVertex3f(5.0, -1.0, -z);
    glEnd();
}

void reshape(int w, int h)
{
    if(w > width || h > height)
    {
        w = width;
        h = height;
        glutReshapeWindow(w, h);
    }

    if(h == 0)
        h = 1;
    float r = 1.0 * w / h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluOrtho2D(-10, 10, -10, 10);
    gluPerspective(60, r, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void init()
{
    glClearColor(0.553, 0.989, 0.576, 1.0);
    glEnable(GL_MAP1_VERTEX_3);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
}

void processNormalKeys(unsigned char key, int x, int y)
{
    glutPostRedisplay();
    if(key == 27)
        exit(0);
    else if(key == 43)
        speed += 0.5;
    else if(key == 45)
        speed -= 0.5;
}

void display_text(char *s, float w)
{
    glLineWidth(w);
    for(char *i = s; *i != '\0'; i++)
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *i);
}

void processMouse_main(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        player = 1;
        if(error_flag == 1)
        {
            error_flag = 0;
            glutSetWindow(markErrorWindow);
            glutHideWindow();
            glutSetWindow(mainWindow);
        }
        if((x > 155 && x < 445) && (y > 330 && y < 385))
        {   //Single Player button
            choose_sp = 1;
            glutSetWindow(markWindow);
            glutShowWindow();
        }
        else if((x > 155 && x < 445) && (y > 415 && y < 475))
        {   //Multi Player button
            choose_sp = 0;
            glutSetWindow(markWindow);
            glutShowWindow();
        }
        else if((x > 153 && x < 447) && (y > 505 && y < 562))
            exit(0);
        //'+' button
        else if((x > 15 && x < 45) && (y > 560 && y < 580))
            speed += 0.5;
        //'-' button
        else if((x > 55 && x < 77) && (y > 560 && y < 580))
            speed -= 0.5;
    }
}

void processMouse_mark_helper(int choice)
{
    if(player == 1)
        p1_mark_choice = choice;
    else if(choice == p1_mark_choice && player == 2)
    {
        error_flag = 1;
        //glutHideWindow();
        glutSetWindow(markErrorWindow);
        glutShowWindow();
    }
    else
        p2_mark_choice = choice;

    if(choose_sp == 0 && player == 1)
        player = 2;
    else if(error_flag == 0)
    {
        player = 1;
        glutHideWindow();
        glutSetWindow(gameWindow);
        glutShowWindow();
    }
}

void processMouse_mark(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        if(error_flag == 1)
        {
            error_flag = 0;
            glutSetWindow(markErrorWindow);
            glutHideWindow();
            glutSetWindow(markWindow);
        }
        //mark 1
        else if((x > 120 && x < 230) && (y > 175 && y < 280))
        {
            processMouse_mark_helper(1);
            if(choose_sp == 1)
            {
                int temp[3] = {2, 4, 3};
                comp_mark_choice = temp[rand() % 3];
            }
        }
        //mark 2
        else if((x > 365 && x < 480) && (y > 175 && y < 280))
        {
            processMouse_mark_helper(2);
            if(choose_sp == 1)
            {
                int temp[3] = {1, 3, 4};
                comp_mark_choice = temp[rand() % 3];
            }
        }
        //mark 3
        else if((x > 120 && x < 230) && (y > 385 && y < 485))
        {
            processMouse_mark_helper(3);
            if(choose_sp == 1)
            {
                int temp[3] = {1, 2, 4};
                comp_mark_choice = temp[rand() % 3];
            }
        }
        //mark 4
        else if((x > 365 && x < 480) && (y > 390 && y < 490))
        {
            processMouse_mark_helper(4);
            if(choose_sp == 1)
            {
                int temp[3] = {1, 2, 3};
                comp_mark_choice = temp[rand() % 3];
            }
        }
        //Back
        else if((x > 490 && x < 578) && (y > 550 && y < 575))
        {
            title_xpos = -30.0;
            markWindow_mark1_zpos = 0.0;
            markWindow_mark2_zpos = 0.0;
            markWindow_mark3_zpos = 0.0;
            markWindow_mark4_zpos = 0.0;
            glutHideWindow();
            glutSetWindow(mainWindow);
        }
        //'+' button
        else if((x > 15 && x < 45) && (y > 560 && y < 580))
            speed += 0.5;
        //'-' button
        else if((x > 55 && x < 77) && (y > 560 && y < 580))
            speed -= 0.5;
    }
}

void processMouse_game_helper(int box)
{
    if(player == 1)
    {
        ttt[box] = 'A';
        if(choose_sp == 0)
            player = 2;
        else
            player = 0;
    }
    else if(player == 2)
    {
        ttt[box] = 'B';
        player = 1;
    }
    current = box;
    place_mark_zpos = -50.0;
}

void processMouse_game(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        //'+' button
        if((x > 15 && x < 45) && (y > 560 && y < 580))
            speed += 0.5;
        //'-' button
        else if((x > 55 && x < 77) && (y > 560 && y < 580))
            speed -= 0.5;
    }
    if(over == 0 && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        //box 1
        if((x > 98 &&  x < 225) && (y > 97 && y < 224) && ttt[0] == '1')
            processMouse_game_helper(0);
        //box 2
        else if((x > 230 &&  x < 370) && (y > 97 && y < 224) && ttt[1] == '2')
            processMouse_game_helper(1);
        //box 3
        else if((x > 375 &&  x < 502) && (y > 97 && y < 224) && ttt[2] == '3')
            processMouse_game_helper(2);
        //box 4
        else if((x > 98 &&  x < 225) && (y > 229 && y < 369) && ttt[3] == '4')
            processMouse_game_helper(3);
        //box 5
        else if((x > 230 &&  x < 370) && (y > 229 && y < 369) && ttt[4] == '5')
            processMouse_game_helper(4);
        //box 6
        else if((x > 375 &&  x < 502) && (y > 229 && y < 369) && ttt[5] == '6')
            processMouse_game_helper(5);
        //box 7
        else if((x > 98 &&  x < 225) && (y > 375 && y < 502) && ttt[6] == '7')
            processMouse_game_helper(6);
        //box 8
        else if((x > 230 &&  x < 370) && (y > 375 && y < 502) && ttt[7] == '8')
            processMouse_game_helper(7);
        //box 9
        else if((x > 375 &&  x < 502) && (y > 375 && y < 502) && ttt[8] == '9')
            processMouse_game_helper(8);
    }
    if(over == 1 && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {   //back
        if((x > 490 && x < 590) && (y > 25 && y < 55))
        {
            clear_board();
            title_xpos = -30.0;
            glutSetWindow(gameEndWindow);
            glutHideWindow();
            glutSetWindow(gameWindow);
            glutHideWindow();
            glutSetWindow(mainWindow);
        }
    }
}

void processMouseMotion_mark(int x, int y)
{
    //mark 1
    if((x > 120 && x < 230) && (y > 175 && y < 280))
    {
        markWindow_mark1_zpos = 2.5;
    }
    //mark 2
    else if((x > 365 && x < 480) && (y > 175 && y < 280))
    {
        markWindow_mark2_zpos = 2.5;
    }
    //mark 3
    else if((x > 120 && x < 230) && (y > 385 && y < 485))
    {
        markWindow_mark3_zpos = 2.5;
    }
    //mark 4
    else if((x > 365 && x < 480) && (y > 390 && y < 490))
    {
        markWindow_mark4_zpos = 2.5;
    }
    else
    {
        if(p1_mark_choice == 1)
        {
            markWindow_mark2_zpos = 0.0;
            markWindow_mark3_zpos = 0.0;
            markWindow_mark4_zpos = 0.0;
        }
        else if(p1_mark_choice == 2)
        {
            markWindow_mark1_zpos = 0.0;
            markWindow_mark3_zpos = 0.0;
            markWindow_mark4_zpos = 0.0;
        }
        else if(p1_mark_choice == 3)
        {
            markWindow_mark1_zpos = 0.0;
            markWindow_mark2_zpos = 0.0;
            markWindow_mark4_zpos = 0.0;
        }
        else if(p1_mark_choice == 4)
        {
            markWindow_mark1_zpos = 0.0;
            markWindow_mark2_zpos = 0.0;
            markWindow_mark3_zpos = 0.0;
        }
        else
        {
            markWindow_mark1_zpos = 0.0;
            markWindow_mark2_zpos = 0.0;
            markWindow_mark3_zpos = 0.0;
            markWindow_mark4_zpos = 0.0;
        }

    }
}

void draw_marks_helper(float x, float y, float z, float eye_y, float mouth_y, int mark)
{
    if(mark == 1)
        draw_mark_1(x, y, z, eye_y, mouth_y);
    else if(mark == 2)
        draw_mark_2(x, y, z, eye_y, mouth_y);
    else if(mark == 3)
        draw_mark_3(x, y, z, eye_y);
    else if(mark == 4)
        draw_mark_4(x, y, z, eye_y, mouth_y);
}

void draw_marks()
{
    int mark;
    float z = 0.0;
    for(int i = 0; i < 9; i++)
    {
        switch(ttt[i])
        {
            case 'A': mark = p1_mark_choice;
            break;
            case 'B': mark = p2_mark_choice;
            break;
            case 'C': mark = comp_mark_choice;
            break;
            default: mark = 0;
            break;
        }

        if(current == i)
            z = place_mark_zpos;

        if(over == 1)
            for(int j = 0; j < 3; j++)
                if(i == win_cells[j])
                    z = zpos;

        switch(i)
        {
            case 0: draw_marks_helper(-5.0, 5.0, z, 0.2, -1.4, mark);
            break;
            case 1: draw_marks_helper(0.0, 5.0, z, 0.2, -1.4, mark);
            break;
            case 2: draw_marks_helper(5.0, 5.0, z, 0.2, -1.4, mark);
            break;
            case 3: draw_marks_helper(-5.0, 0.0, z, 0.4, -1.4, mark);
            break;
            case 4: draw_marks_helper(0.0, 0.0, z, 0.4, -1.4, mark);
            break;
            case 5: draw_marks_helper(5.0, 0.0, z, 0.4, -1.4, mark);
            break;
            case 6: draw_marks_helper(-5.0, -5.0, z, 0.7, -1.3, mark);
            break;
            case 7: draw_marks_helper(0.0, -5.0, z, 0.7, -1.3, mark);
            break;
            case 8: draw_marks_helper(5.0, -5.0, z, 0.7, -1.3, mark);
            break;
        }
        z = 0.0;
    }
}

void mark_display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0, 0.0, -18.0);

    //display title
    glPushMatrix();
    glTranslatef(-8.25, 7.5, 2.0);
    glScalef(0.01, 0.01, 0.01);
    glColor3f(1.0, 0.0, 0.0);
    display_text("CHOOSE YOUR MARK", 4.0);
    glPopMatrix();

    //display Player title
    glPushMatrix();
    glTranslatef(-1.75, 4.8, 5.0);
    glScalef(0.005, 0.005, 0.005);
    glColor3f(1.0, 0.0, 0.0);
    if(player == 1)
        display_text("PLAYER 1", 3.0);
    else
        display_text("PLAYER 2", 3.0);
    glPopMatrix();

    //draw grid lines
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
        glVertex3f(0.0, 5.0, 2.0);
        glVertex3f(0.0, -7.0, 2.0);

        glVertex3f(-7.0, -1.0, 2.0);
        glVertex3f(7.0, -1.0, 2.0);
    glEnd();
    glPopMatrix();

    //draw mark 1
    draw_mark_1(-4.0, 2.5, markWindow_mark1_zpos, 0.3, -1.5);

    //draw mark 2
    draw_mark_2(4.0, 2.5, markWindow_mark2_zpos, 0.3, -1.4);

    //draw mark 3
    draw_mark_3(-4.0, -4.5, markWindow_mark3_zpos, 0.8);

    //draw mark 4
    draw_mark_4(4.0, -4.5, markWindow_mark4_zpos, 0.8, -1.45);

    //Display 'back' text
    glPushMatrix();
    glTranslatef(6.0, -8.5, 2.0);
    glScalef(0.006, 0.006, 0.006);
    glColor3f(1.0, 1.0, 1.0);
    display_text("Back", 3.0);
    glPopMatrix();

    //display '+' and '-'
    glPushMatrix();
    glTranslatef(-8.5, -8.5, 2.0);
    glScalef(0.005, 0.005, 0.005);
    glColor3f(1.0, 1.0, 1.0);
    display_text("+ -", 3.0);
    glPopMatrix();

    glutSwapBuffers();
}

void timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(100/6, timer, 0);

    //make title fly in in main window
    if(title_xpos < 0.0)
        title_xpos += 0.3;

    if(title_xpos >= 0.0)
    {
        switch(title_zstate)
        {
            case 1: if(title_zpos > 2.0)
                        title_zstate = -1;
                    else
                        title_zpos += 0.15;
            break;
            case -1: if(title_zpos < -2.0)
                        title_zstate = 1;
                     else
                        title_zpos -= 0.15;
            break;
        }
    }

    //Rotate the marks
    angle += 3.0 * speed;
    if(angle > 360.0)
        angle -= 360.0;
    else if(angle < -360.0)
        angle += 360.0;

    //make marks go up and down
    switch(state)
    {
        case 1: if(ypos1 > 3.5)
                    state = -1;
                else
                {
                    ypos1 += 0.05;
                    ypos2 -= 0.05;
                }
        break;
        case -1: if(ypos1 < 1.5)
                    state = 1;
                 else
                 {
                     ypos1 -= 0.05;
                     ypos2 += 0.05;
                 }
        break;
    }

    //winning marks celebration
    switch(mark_zstate)
    {
        case 1: if(zpos > 3.0)
                    mark_zstate = -1;
                else
                    zpos += 0.4;
        break;
        case -1: if(zpos < -3.0)
                    mark_zstate = 1;
                 else
                    zpos -= 0.4;
        break;
    }

    //animation when mark is placed
    if(place_mark_zpos < 0.0)
        place_mark_zpos += 2.5;
    if(place_mark_zpos == 0.0)
        mark_placed = 1;

    //confetti rotation
    for(int i = 0; i < CONFETTI_COUNT; i++)
        if(conf_a[i] > 360.0)
            conf_a[i] = 0.0;
        else
            conf_a[i] += 1.5;

    //confetti falling
    if(start_confetti == 1)
        for(int i = 0; i < CONFETTI_COUNT; i++)
            if(conf_y[i] > 8)
                conf_y[i] -= 0.1;
            else if(conf_y[i] > 6)
                conf_y[i] -= 0.12;
            else if(conf_y[i] > 4)
                conf_y[i] -= 0.14;
            else if(conf_y[i] > 2)
                conf_y[i] -= 0.16;
            else if(conf_y[i] > 0)
                conf_y[i] -= 0.18;
            else if(conf_y[i] > -2)
                conf_y[i] -= 0.2;
            else if(conf_y[i] > -4)
                conf_y[i] -= 0.25;
            else if(conf_y[i] > -10)
                conf_y[i] -= 0.3;
            else
                conf_y[i] -= 0.3;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0, 0.0, -18.0);

    //Add ambient light
    GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

    //Add positioned light
    GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat lightPos0[] = {4.0f, 0.0f, 8.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    //Add directed light
    GLfloat lightColor1[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat lightPos1[] = {-1.0f, 0.5f, 0.5f, 0.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

    //display main title
    glPushMatrix();
    glTranslatef(title_xpos, 6.0, title_zpos);
    display_main_title();
    glPopMatrix();

    //draw mark 1
    draw_mark_1(-7.5, ypos1, 0.0, 0.4, -1.4);

    //draw mark 2
    draw_mark_2(-2.5, ypos2, 0.0, 0.4, -1.4);

    //draw mark 3
    draw_mark_3(2.5, ypos1, 0.0, 0.4);

    //draw mark 4
    draw_mark_4(7.5, ypos2, 0.0, 0.4, -1.45);

    //display SP button
    glPushMatrix();
    glTranslatef(0.0, -2.0, -1.1);
    draw_cuboid(1.2);
    glTranslatef(-4.0, -0.1, 2.0);
    glScalef(0.006, 0.006, 0.006);
    glColor3f(1.0, 1.0, 1.0);
    display_text("Single Player", 2.0);
    glPopMatrix();

    //display MP button
    glPushMatrix();
    glTranslatef(0.0, -5.0, -0.2);
    draw_cuboid(0.3);
    glTranslatef(-3.7, 0.2, 2.0);
    glScalef(0.006, 0.006, 0.006);
    glColor3f(1.0, 1.0, 1.0);
    display_text("Multi Player", 2.0);
    glPopMatrix();

    //display EXIT button
    glPushMatrix();
    glTranslatef(0.0, -8.0, 0.0);
    draw_cuboid(0.2);
    glTranslatef(-1.3, 0.5, 2.0);
    glScalef(0.006, 0.006, 0.006);
    glColor3f(1.0, 1.0, 1.0);
    display_text("Exit", 2.0);
    glPopMatrix();

    //display '+' and '-'
    glPushMatrix();
    glTranslatef(-8.5, -8.5, 2.0);
    glScalef(0.005, 0.005, 0.005);
    glColor3f(1.0, 1.0, 1.0);
    display_text("+ -", 3.0);
    glPopMatrix();

    glutSwapBuffers();
}

void game_display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0, 0.0, -18.0);

    int check_win = win();
    if(check_win == 1)
    {
        int p = player;
        over = 1;
        if(choose_sp == 0)
            winner = (player % 2) == 0 ? 1 : 2;
        else
            winner = (player % 2) == 0 ? 1 : 0;
    }
    else if(check_win == 0)
    {
        over = 1;
        winner = -1;
    }

    if(over == 1)
    {
        glutSetWindow(gameEndWindow);
        glutShowWindow();
        glutSetWindow(gameWindow);
        if(winner == 1 || winner == 2)
        {
            start_confetti = 1;
            for(int i = 0; i < CONFETTI_COUNT; i++)
                draw_confetti(conf_x[i], conf_y[i], conf_z[i], conf_r[i], conf_g[i], conf_b[i], conf_a[i], conf_rx[i], conf_ry[i], conf_rz[i]);
        }
    }

    if(player == 0 && over == 0)
    {
         c++;
         get_computer_move();
         placed_once = 0;
    }

    if(mark_placed == 1)
    {
        ttt[comp_move] = 'C';
        current = comp_move;
        mark_placed = 0;
        if(placed_once == 0)
        {
            place_mark_zpos = -50.0;
            placed_once = 1;
        }
    }

    //display turn title
    if(over == 0)
    {
        if(choose_sp == 1)
            player = 1;
        glPushMatrix();
        glTranslatef(-5.25, 7.5, 2.0);
        glScalef(0.007, 0.007, 0.007);
        glColor3f(1.0, 0.0, 0.0);
        if(player == 1)
            display_text("PLAYER 1's TURN", 3.0);
        else
            display_text("PLAYER 2's TURN", 3.0);
        glPopMatrix();
    }

    //draw grid lines
    glPushMatrix();
    glLineWidth(3.0);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
        glVertex3f(-2.5, 7.0, 0.0);
        glVertex3f(-2.5, -7.0, 0.0);

        glVertex3f(2.5, 7.0, 0.0);
        glVertex3f(2.5, -7.0, 0.0);

        glVertex3f(-7.0, 2.5, 0.0);
        glVertex3f(7.0, 2.5, 0.0);

        glVertex3f(-7.0, -2.5, 0.0);
        glVertex3f(7.0, -2.5, 0.0);
    glEnd();
    glPopMatrix();

    draw_marks();

    if(over == 1)
    {
        glPushMatrix();
        glTranslatef(6.0, 7.5, 2.0);
        glScalef(0.006, 0.006, 0.006);
        glColor3f(1.0, 0.0, 0.0);
        display_text("Back", 3.0);
        glPopMatrix();
    }

    //display '+' and '-'
    glPushMatrix();
    glTranslatef(-8.5, -8.5, 2.0);
    glScalef(0.005, 0.005, 0.005);
    glColor3f(1.0, 1.0, 1.0);
    display_text("+ -", 3.0);
    glPopMatrix();

    glutSwapBuffers();
}

void mark_error_display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0, 0.0, -18.0);

    glPushMatrix();
    glTranslatef(-19.0, 1.0, 5.0);
    glScalef(0.02, 0.02, 0.02);
    glColor3f(1.0, 1.0, 1.0);
    display_text("Both Players can't", 2.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-22.0, -5.0, 0.0);
    glScalef(0.03, 0.03, 0.03);
    glColor3f(1.0, 1.0, 1.0);
    display_text("have same mark", 2.0);
    glPopMatrix();

    glutSwapBuffers();
}

void game_end_display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0, 0.0, -18.0);

    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
        glPushMatrix();
        if(winner == 1 && choose_sp == 0)
        {
            glTranslatef(-43.0, -3.0, 2.0);
            glScalef(0.06, 0.06, 0.06);
            display_text("PLAYER 1 WINS!!", 3.0);
        }
        else if(winner == 2 && choose_sp == 0)
        {
            glTranslatef(-43.0, -3.0, 2.0);
            glScalef(0.06, 0.06, 0.06);
            display_text("PLAYER 2 WINS!!", 3.0);
        }
        else if(winner == 1 && choose_sp == 1)
        {
            glTranslatef(-30.0, -3.0, 2.0);
            glScalef(0.06, 0.06, 0.06);
            display_text("YOU WIN!!", 3.0);
        }
        else if(winner == 0 && choose_sp == 1)
        {
            glTranslatef(-30.0, -3.0, 2.0);
            glScalef(0.06, 0.06, 0.06);
            display_text("YOU LOSE!!", 3.0);
        }
        else if(winner == -1)
        {
            glTranslatef(-35.0, -3.0, 2.0);
            glScalef(0.06, 0.06, 0.06);
            display_text("GAME DRAW!!", 3.0);
        }
        glPopMatrix();
    glPopMatrix();

    glutSwapBuffers();
}

int main(int argc, char**argv)
{
    srand(time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowPosition(200, 50);
    glutInitWindowSize(width, height);

    mainWindow = glutCreateWindow("Tic Tac Toe");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    glutKeyboardFunc(processNormalKeys);
    glutMouseFunc(processMouse_main);
    init();

    markWindow = glutCreateSubWindow(mainWindow, 0, 0, width, height);
    glutDisplayFunc(mark_display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(processNormalKeys);
    glutMouseFunc(processMouse_mark);
    glutPassiveMotionFunc(processMouseMotion_mark);
    init();
    glutHideWindow();

    markErrorWindow = glutCreateSubWindow(markWindow, (width / 4), (height / 2.4), (width / 2), (height / 6));
    glutDisplayFunc(mark_error_display);
    glutReshapeFunc(reshape);
    init();
    glClearColor(0.0, 0.0, 1.0, 1.0);
    glutHideWindow();

    gameWindow = glutCreateSubWindow(mainWindow, 0, 0, width, height);
    glutDisplayFunc(game_display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(processNormalKeys);
    glutMouseFunc(processMouse_game);
    init();
    clear_board();
    glutHideWindow();

    gameEndWindow = glutCreateSubWindow(gameWindow, (width / 4), (height / 2.18), (width / 2), (height / 12));
    glutDisplayFunc(game_end_display);
    glutReshapeFunc(reshape);
    init();
    glClearColor(0.0, 0.0, 1.0, 1.0);
    glutHideWindow();

    glutSetWindow(mainWindow);

    glutMainLoop();
}
