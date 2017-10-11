#define GLUT_DISABLE_ATEXIT_HACK
#include <stdafx.h>  
#include <windows.h>  
#include <cstdlib>  
#include <glut.h> 
#include "objLoader.h"

static int stripeImageWidth = 32;
static float G = 9.8;
static int ballsize = 100;
static double dt = 0.01;
static int pos_ground = -2;
static float lose_vy = 2;
static float v0 = 10;
static const char* objfilename = "Eight.obj";
static bool ifLoadModel = false;
Mesh mesh;
enum Direction {
	SIT = 0,
	UP = 1,
	DOWN = 2
};
Direction direction;
GLdouble y;
GLdouble vy;

void init(void) {
	y = pos_ground;
	vy = 0; 
	direction = SIT;
	mesh.parse(objfilename);
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	glClearColor(1.0, 1.0, 1.0, 0.3);
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}
void idle(void)
{
	glutPostRedisplay();
}
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(0, y, 0);
	if (ifLoadModel) {
		glRotatef(80.0f, 1.0f, 0.0f, 0.0f);	
		mesh.draw();
	}
	else {
		dt = 0.001;
		glutSolidSphere(2, 50, 50);
	}
	
	if (direction == DOWN) {
		y = y +vy*dt + 0.5*G*dt*dt; 
		vy = vy - G*dt;
		if (y <= pos_ground) {
			vy = 0 - vy;
			direction = UP;
			if(vy - lose_vy > 0){
				vy -= lose_vy;
			}else{
				vy = 0;
				direction = SIT;
			}
		} 
	}else if(direction == UP){ 
		y = y + vy*dt-0.5*G*dt*dt; 
		vy = vy - G*dt; 
		if(vy <= 0){
			vy = 0 - vy;
			direction = DOWN;
		}
	}
	glPopMatrix();
	glFlush();
}
void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-15, 15, -15 * (GLfloat)h / (GLfloat)w, 15 * (GLfloat)h / (GLfloat)w, -15, 15);
	else
		glOrtho(-15 * (GLfloat)h / (GLfloat)w, 15 * (GLfloat)h / (GLfloat)w, -15, 15, -15, 15);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void timer(int val)
{
	//t += 0.00001;
	display();
	glutTimerFunc(0.001, timer, 0);
}
void mouse(int button, int state, int x, int y)
{
	if(state == GLUT_UP){
		vy = v0;
		direction = UP;
	}
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(450, 100);
	glutCreateWindow("ball");
	init();
	glutDisplayFunc(display);
	//glutTimerFunc(0.001, timer, 0);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}
/*#define GLUT_DISABLE_ATEXIT_HACK
#include <stdafx.h>  
#include <windows.h>  
#include <cstdlib>  
#include <glut.h>  
static int ballsize = 100;
void init(void)   
{
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	glClearColor(1.0, 1.0, 1.0, 0.3); 
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSolidSphere(1.0, 1000, 1000);
	glFlush();
}
void reshape(int w, int h)
{
	glViewport((w-ballsize)/2, (h-ballsize)/2, (GLsizei)ballsize, (GLsizei)ballsize);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.5, 1.5 , -1.5, 1.5, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("ball");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
	}*/