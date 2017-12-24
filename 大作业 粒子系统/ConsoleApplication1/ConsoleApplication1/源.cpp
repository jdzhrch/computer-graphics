#define GLUT_DISABLE_ATEXIT_HACK
#include <stdafx.h>  
#include <windows.h>  
#include <cstdlib>  
#include <stdio.h>
#include <stdlib.h>
#include <glut.h> 
#include <math.h>
#include "ParticleSystem.h"
#define PI 3.1415926
//��������ʼ��ת�Ƕ�
GLfloat rtri = 0;
void init(void)
{
	//���ñ���ɫ����������䱳��
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//���ö�������ģʽΪsmooth ��ʽ
	glShadeModel(GL_SMOOTH);
	//����Ȳ��Կ��ء������ڼ��������z ��Ȳ���
	glEnable(GL_DEPTH_TEST);
	//�ߵĿ���ݿ���
	glEnable(GL_LINE_SMOOTH);
	//���ÿ����Ч��
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	//ָ����Ϻ���
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//����ɫ�ʻ��״̬
	glEnable(GL_BLEND);
}

//opengl �û��Զ����ͼ����
void display(void)
{
	
}

//�û��Զ��崰�ڵ�����С�¼��Ĵ�����
//�����������Ҫ�������ӿڡ�ͶӰ��͸�ӱ任����ز���
void reshape(int w, int h)
{
	//�����ӿڲ���Ϊ�������ڷ�Χ��
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	//����ͶӰ����:ͶӰ�����ʼΪ��λ��
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//����͸�Ӳ���: �۾�����������ӽǲ���Ϊ60 �ȣ��Ӿ���Ŀ�Ⱥ͸߶ȱȣ��Ӿࣨ���ࣩ
	//(near)���Ӿ�(far)����
	//near = 1, far = 100, Z �Ḻ��˳�����߷���ָ����Ļ��
	//X ���������ң�Y ���������ϣ�����ԭ������Ļ���Ĵ�
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
	//�����������λ�ü���̬����:
	//�����λ��(cX, cY, cZ)
	//�ӵ����۲�����λ��Ow(oX, oY, oZ)
	//�����λ�˲����������������ʸ��
	gluLookAt(0, 0, 5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//���þ���ģʽΪģ��-��ͼ�任ģʽ���Ա��ں�����Զ�����ʾ����������ģʽ

	if (w <= h)
		glOrtho(-15, 15, -15 * (GLfloat)h / (GLfloat)w, 15 * (GLfloat)h / (GLfloat)w, -15, 15);
	else
		glOrtho(-15 * (GLfloat)h / (GLfloat)w, 15 * (GLfloat)h / (GLfloat)w, -15, 15, -15, 15);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//�û��Զ�������¼�������
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'x':
	case 27: //ESC ����
		exit(0);
		break;
	default:
		break;
	}
}
void mouseMotion(int x, int y)
{
	
}
void idle()
{
	glutPostRedisplay();
}
int main(int argc, char** argv)
{
	//�������в�����ʼ��OpenGL
	glutInit(&argc, argv);
	//ʹ��RGB ɫ�ʡ�˫��������ģʽ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	//��ʼ����ͼ���ڴ�С����
	glutInitWindowSize(1024, 768);
	//�������Ͻ��������
	glutInitWindowPosition(0, 0);
	//��������
	glutCreateWindow("OpenGL HelloWorld");
	//�û��Զ����ʼ����ͼ��������
	init();
	//�û�ָ���Ļ�ͼ������display ���ɱ�
	glutDisplayFunc(display);
	//���ڵ�����С�¼��Ĵ�����
	glutReshapeFunc(reshape);
	//���ڼ��̴����¼��Ĵ�����
	glutKeyboardFunc(keyboard);
	glutMotionFunc(mouseMotion);
	//���ô��ڿ���ʱ���õĺ���
	glutIdleFunc(idle);
	//����glut ���������ѭ��
	glutMainLoop();
	return 0;
}
ParticleSystem particleSystem;

//����      
float whRatio;
int wHeight = 0;
int wWidth = 0;

//�ӵ�      
float center[] = { 0, 0, 0 };
float eye[] = { 0, 0, 5 };

Particle * init_particle1()
{
	float size = rand() % 15 * 0.02f;
	unsigned char color[] = { 1.0f,0.0f,0.0f };
	Particle* p = new Particle;
	return p;
}

void drawScene()
{
	particleSystem.update();

}

void updateView(int height, int width)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);//���þ���ģʽΪͶӰ           
	glLoadIdentity();   //��ʼ������Ϊ��λ����              
	whRatio = (GLfloat)width / (GLfloat)height;  //������ʾ����         
	glOrtho(-30, 30, -30, 30, -100, 100); //��ͶӰ      
	glMatrixMode(GL_MODELVIEW);  //���þ���ģʽΪģ��        
}

void reshape(int width, int height)
{
	if (height == 0) { //����߶�Ϊ0     
		height = 1;   //�ø߶�Ϊ1��������ַ�ĸΪ0������          
	}
	wHeight = height;
	wWidth = width;
	updateView(wHeight, wWidth); //�����ӽ�          
}
void init(void)
{
	srand(unsigned(time(nullptr)));
	glClearColor(1.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glColor4f(1.0, 1.0, 1.0, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);
	e1 = new emitter(1000, 0, 0, 0, 0);
	e1->emit(init_particle1);
}

void redraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����ɫ����Ȼ���     
	glClearColor(0, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();   //��ʼ������Ϊ��λ����          
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 1, 0);
	drawScene();//���Ƴ���       
	glutSwapBuffers();//����������      
}