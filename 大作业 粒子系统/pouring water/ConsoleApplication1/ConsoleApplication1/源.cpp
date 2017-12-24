#define _CRT_SECURE_NO_WARNINGS      

#include <stdlib.h>       
#include<time.h>    
#include"ParticleSystem.h"    
#include "sph_fluid_system.h"

#define G 9.8
#define WATER_COLOR { 11, 172, 207, 0.5 }
//ParticleSystem *particleSystem;
FluidSystem*					g_pSPHSystem = 0;

//视点      
float center[] = { 5, 5, 5 };
float eye[] = { 0, 0, 0 };

particle* init_particle1()
{
	float size = rand() % 15 * 0.02f;
	unsigned char color[] = WATER_COLOR;
	Vector speed(float(rand() % 10 - 4) / 200, float(rand() % 10 - 4) / 200, float(rand() % 10 - 4) / 200);
	Vector acc(0, -G / 80000, 0);
	particle* p = new particle(speed,acc, rand() % 200, color);
	return p;
}


void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);       
	glLoadIdentity();                    
	glOrtho(-30, 30, -30, 30, -100, 100);
	glMatrixMode(GL_MODELVIEW);    
}

void idle()
{
	glutPostRedisplay();
}

void resetSPHSystem(void)
{
	float_3 wall_min = { -25, 00, -25 };
	float_3 wall_max = { 25, 50, 25 };
	float_3 fluid_min = { -15, 1, -15 };
	float_3 fluid_max = { 15, 7, 15 };
	float_3 gravity = { 0.0, -9.8f, 0 };
	g_pSPHSystem->init(1024, &wall_min, &wall_max, &fluid_min, &fluid_max, &gravity);
}

void init(void)
{
	srand(unsigned(time(nullptr)));
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND); 
	GLfloat light_position[] = { 20.0, 20.0, 20.0, 20.0 };
	glClearColor(1.0, 1.0, 1.0, 0.4);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//particleSystem = new ParticleSystem(300, 0, 0, 0, 0,0,0,false);
	//particleSystem->emit(init_particle1);
	resetSPHSystem();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
	glClearColor(0, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();          
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 1, 0);
	//particleSystem->update();

	g_pSPHSystem->tick();
	const float_3* p = g_pSPHSystem->getPointBuf();
	unsigned int stride = g_pSPHSystem->getPointStride();
	glBegin(GL_POINTS);
	for (unsigned int n = 0; n<g_pSPHSystem->getPointCounts(); n++)
	{
		glVertex3f(p->x, p->y, p->z);


		p = (const float_3*)(((const char*)p) + stride);
	}
	glEnd();
	glutSwapBuffers();  
}

int main(int argc, char *argv[])
{
	static FluidSystem s_theSystem;
	g_pSPHSystem = &s_theSystem;
	glutInit(&argc, argv);   
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);            
	glutInitWindowSize(800, 600);  
	int windowHandle = glutCreateWindow("倒水模型");             
	glutDisplayFunc(display);            
	glutReshapeFunc(reshape);  
	glutIdleFunc(idle);
	init();
	glutMainLoop();      
	return 0;
}