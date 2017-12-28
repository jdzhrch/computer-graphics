#define _CRT_SECURE_NO_WARNINGS      
#define GLUT_DISABLE_ATEXIT_HACK 

#include <stdlib.h>       
#include "sph_fluid_system.h"
#include "Model.h"

//鼠标交互有关的
int mousestate = 0; //鼠标当前的状态
GLfloat Oldx = 0.0; // 点击之前的位置
GLfloat Oldy = 0.0;
//与实现角度大小相关的参数
float xRotate = 0.0f;
float yRotate = 0.0f;
float scale = 0.38;
//与实现位置移动有关的参数
float room_x = 0.0;
float room_y = 0.0;
float water_x = 0.0;
float water_y = 0.0;
//灯亮暗程度
float light_intensity = 1;

Model model;
Model model_particle;

//Mesh *mesh;
const char * modelObjFileName = "model/room/House.obj";
const char * particleObjFileName = "Cube.obj";
bool ifUseModel = false;				//是否使用模型作为粒子

const float water_color[4] = { 89.0 / 255, 195.0 / 255, 226.0 / 255,0.1 };
FluidSystem*					g_pSPHSystem = 0;

//视点      
float center[] = { 30, 0, 0 };
float eye[] = { 0, 0, 0 };

void resetSPHSystem(void)
{
	float_3 wall_min = { -25, 00, -25 };
	float_3 wall_max = { 25, 50, 25 };
	float_3 fluid_min = { -15, 15, -15 };
	float_3 fluid_max = { 15, 35, 15 };
	float_3 gravity = { 0.0, -9.8f, 0 };
	g_pSPHSystem->init(512 * 2, &wall_min, &wall_max, &fluid_min, &fluid_max, &gravity);
}

void initGL()
{
	model.readfile(modelObjFileName);
	model.parse();
	model_particle.readfile(particleObjFileName);
	model_particle.parse();
	resetSPHSystem();
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 0, 1);
}

void reshape(GLint w, GLint h)
{
	glViewport(0, 0, (GLsizei)(w), (GLsizei)(h));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w > h)
		glOrtho(-30 * (GLdouble)(w) / h, 30 * (GLdouble)(w) / h, -30 * 1.0, 30 * 1.0, -30 * 100.0, 30 * 100.0);
	else
		glOrtho(-30 * 1.0, 30 * 1.0, -30 * (GLdouble)(h) / w, 30 * (GLdouble)(h) / w, -30 * 100.0, 30 * 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mousestate = 1;
		Oldx = x;
		Oldy = y;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		mousestate = 0;
	glutPostRedisplay();
}

void mouseMove(int x, int y) {
	if (mousestate) {
		//x对应y是因为对应的是法向量
		yRotate += x - Oldx;
		glutPostRedisplay();
		Oldx = x;
		xRotate += y - Oldy;
		glutPostRedisplay();
		Oldy = y;
	}
}

void keyboard(unsigned char key, int x, int y){
	switch (key) {
	case 27:// ESC
		exit(0);
		break;
	//控制房间上下左右
	case 119://w
		room_y += 1;
		cout << room_x << " room_x" << endl;
		break;
	case 115://s
		room_y -= 1;
		cout << room_x << " room_x" << endl;
		break;
	case 97://a
		room_x -= 1;
		break;
	case 100://d
		room_x += 1;
		break;
	default:
		break;
	}
}

void special(int key, int x, int y) {
	switch (key) {
		//控制远近
		case 2://F2
			if (scale > 0.2)
				scale -= 0.1;
			break;
		case 3://F3
			scale += 0.1;
			cout << scale << " scale" << endl;
			break;
		//控制灯光亮暗
		case 11://F11
			if (light_intensity > 0)
				light_intensity -= 0.1;
			break;
		case 12://F12
			if (light_intensity < 1)
				light_intensity += 0.1;
			break;
		//控制水上下左右
		case GLUT_KEY_UP:
			water_y += 1;
			break;
		case GLUT_KEY_DOWN:
			water_y -= 1;
			break;
		case GLUT_KEY_LEFT:
			water_x -= 1;
			break;
		case GLUT_KEY_RIGHT:
			water_x += 1;
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

void display()
{
	/*
	用于原本设计的倒水
	fVector3 pos((rand() & 100 - 50) / 100.0 *5, 100+ (rand() & 100 - 50) / 100.0 * 5, (rand() & 100 - 50) / 100.0 * 5);
	g_pSPHSystem->_addParticle(pos);
	*/
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //清理颜色和深度缓存     

	//由于渲染是独立的，重力会随glrotatef旋转，因此需要把它转回原位。此处由于sin采用弧度制，glrotatef采用角度制，被卡了很久。助教说可以利用顶点变量，在变换后会自动变成变换后坐标。
	fVector3 gravity(-9.8 * sin(xRotate / 180 * PI) * sin(yRotate / 180 * PI), -9.8 * cos(xRotate/180 * PI), 9.8 * sin(xRotate / 180 * PI) * cos(yRotate / 180 * PI));
	g_pSPHSystem->_setGravity(gravity);

	//draw room
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(xRotate, 1.0f, 0.0f, 0.0f); // 让物体旋转的函数 第一个参数是角度大小，后面的参数是旋转的法向量
	glRotatef(yRotate, 0.0f, 1.0f, 0.0f);
	glTranslatef(room_x, room_y-70.f, 0.f);
	{//灯光
		GLfloat light_position[] = { 200.0f, 100.0f, 400.0f, 1.0f };
		GLfloat light_ambient[] = { light_intensity, light_intensity, light_intensity, 1.0f };
		GLfloat light_diffuse[] = { light_intensity, light_intensity, light_intensity, 1.0f };
		GLfloat light_specular[] = { light_intensity, light_intensity, light_intensity, 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

		//开启灯光  
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
	}
	glScalef(100 * scale, 100 * scale, 100 * scale);
	model.draw();
	glPopMatrix();
	glFlush();

	//draw particles
	glPushMatrix();
	glLoadIdentity();  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(xRotate, 1.0f, 0.0f, 0.0f); // 让物体旋转的函数 第一个参数是角度大小，后面的参数是旋转的法向量
	glRotatef(yRotate, 0.0f, 1.0f, 0.0f);
	glTranslatef(water_x, water_y, 0.0f);
	glScalef(scale, scale, scale); // 缩放

	g_pSPHSystem->tick();
	const float_3* p = g_pSPHSystem->getPointBuf();
	unsigned int stride = g_pSPHSystem->getPointStride();

	{
		GLfloat mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat mat_diffuse[] = { 89.0 / 255, 195.0 / 255, 226.0 / 255,1 }; 
		GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 
		//GLfloat mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };  
		//GLfloat mat_shininess = 30.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		//glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		//glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	}
	/*
	绘制重力向量 用于检验
	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	glVertex3f(0, 0, 0);
	fVector3 grav = g_pSPHSystem->_getGravity();
	glVertex3f(grav.x*1000, grav.y*1000, grav.z*1000);
	glEnd();
	*/
	if (!ifUseModel) {//点作为粒子
		glEnable(GL_POINT_SMOOTH);//点会画成圆，不开是矩形
		glPointSize(40.0f);
		glBegin(GL_POINTS);
		//glColor4fv(water_color);			//水蓝色	
		//glScalef(10.0, 10.0, 10.0);
		for (unsigned int n = 0; n<g_pSPHSystem->getPointCounts(); n++)
		{
			glVertex3f(p->x, p->y, p->z);
			p = (const float_3*)(((const char*)p) + stride);
		}
		glEnd();
		glDisable(GL_POINT_SMOOTH);
	}
	else {//模型作为粒子
		for (unsigned int n = 0; n<g_pSPHSystem->getPointCounts(); n++)
		{
			glPushMatrix();
			glTranslatef(p->x, p->y, p->z);
			//model_particle.draw();
			glutSolidSphere(4, 5, 5);
			glPopMatrix();
			p = (const float_3*)(((const char*)p) + stride);
		}
	}
	glPopMatrix();
	glFlush();

	glutSwapBuffers();	//这是Opengl中用于实现双缓存技术的一个重要函数
}

void idle(void)
{

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	static FluidSystem s_theSystem;
	g_pSPHSystem = &s_theSystem;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // GLUT_Double 表示使用双缓存而非单缓存
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 500);
	glutCreateWindow("倒水模型");

	initGL();
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);
	glutSpecialFunc(&special);
	glutKeyboardFunc(&keyboard);
	glutReshapeFunc(&reshape);
	glutDisplayFunc(&display);
	glutIdleFunc(&idle);

	glutMainLoop();
	return 0;
}