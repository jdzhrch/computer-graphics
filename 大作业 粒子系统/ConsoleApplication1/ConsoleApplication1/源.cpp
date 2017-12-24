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
//金字塔初始旋转角度
GLfloat rtri = 0;
void init(void)
{
	//设置背景色——用于填充背景
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//设置多边形填充模式为smooth 方式
	glShadeModel(GL_SMOOTH);
	//打开深度测试开关——用于检测物体间的z 深度差异
	glEnable(GL_DEPTH_TEST);
	//线的抗锯齿开关
	glEnable(GL_LINE_SMOOTH);
	//启用抗锯齿效果
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	//指定混合函数
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//启用色彩混合状态
	glEnable(GL_BLEND);
}

//opengl 用户自定义绘图函数
void display(void)
{
	
}

//用户自定义窗口调整大小事件的处理函数
//在这个函数中要求设置视口、投影、透视变换的相关参数
void reshape(int w, int h)
{
	//设置视口参数为整个窗口范围内
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	//设置投影参数:投影矩阵初始为单位阵
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//设置透视参数: 眼睛或摄像机的视角参数为60 度，视景体的宽度和高度比，视距（焦距）
	//(near)和视径(far)参数
	//near = 1, far = 100, Z 轴负向顺着视线方向指向屏幕内
	//X 轴正向向右，Y 轴正向向上，坐标原点在屏幕中心处
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
	//设置摄像机的位置及姿态参数:
	//摄像机位置(cX, cY, cZ)
	//视点所观察中心位置Ow(oX, oY, oZ)
	//摄像机位姿参数——摄像机顶部矢量
	gluLookAt(0, 0, 5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//设置矩阵模式为模型-视图变换模式，以便于后面的自定义显示函数继续本模式

	if (w <= h)
		glOrtho(-15, 15, -15 * (GLfloat)h / (GLfloat)w, 15 * (GLfloat)h / (GLfloat)w, -15, 15);
	else
		glOrtho(-15 * (GLfloat)h / (GLfloat)w, 15 * (GLfloat)h / (GLfloat)w, -15, 15, -15, 15);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//用户自定义键盘事件处理函数
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'x':
	case 27: //ESC 键盘
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
	//用命令行参数初始化OpenGL
	glutInit(&argc, argv);
	//使用RGB 色彩、双缓存和深度模式
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	//初始化绘图窗口大小参数
	glutInitWindowSize(1024, 768);
	//窗口左上角坐标参数
	glutInitWindowPosition(0, 0);
	//创建窗口
	glutCreateWindow("OpenGL HelloWorld");
	//用户自定义初始化绘图环境函数
	init();
	//用户指定的绘图函数，display 名可变
	glutDisplayFunc(display);
	//窗口调整大小事件的处理函数
	glutReshapeFunc(reshape);
	//窗口键盘处理事件的处理函数
	glutKeyboardFunc(keyboard);
	glutMotionFunc(mouseMotion);
	//设置窗口空闲时调用的函数
	glutIdleFunc(idle);
	//进入glut 函数库的主循环
	glutMainLoop();
	return 0;
}
ParticleSystem particleSystem;

//视区      
float whRatio;
int wHeight = 0;
int wWidth = 0;

//视点      
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
	glMatrixMode(GL_PROJECTION);//设置矩阵模式为投影           
	glLoadIdentity();   //初始化矩阵为单位矩阵              
	whRatio = (GLfloat)width / (GLfloat)height;  //设置显示比例         
	glOrtho(-30, 30, -30, 30, -100, 100); //正投影      
	glMatrixMode(GL_MODELVIEW);  //设置矩阵模式为模型        
}

void reshape(int width, int height)
{
	if (height == 0) { //如果高度为0     
		height = 1;   //让高度为1（避免出现分母为0的现象）          
	}
	wHeight = height;
	wWidth = width;
	updateView(wHeight, wWidth); //更新视角          
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色和深度缓存     
	glClearColor(0, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();   //初始化矩阵为单位矩阵          
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 1, 0);
	drawScene();//绘制场景       
	glutSwapBuffers();//交换缓冲区      
}