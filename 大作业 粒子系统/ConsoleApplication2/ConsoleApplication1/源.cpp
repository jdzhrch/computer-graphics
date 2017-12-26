#include "Model.h"


//鼠标交互有关的
int mousetate = 0; //鼠标当前的状态
GLfloat Oldx = 0.0; // 点击之前的位置
GLfloat Oldy = 0.0;
//与实现角度大小相关的参数，只需要两个就可以完成
float xRotate = 0.0f;
float yRotate = 0.0f;
float ty = 0.0f;
float scale = 1;

Model model;


// 当窗体改变大小的时候
void myReshape(GLint w, GLint h)
{
	glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w > h)
		glOrtho(-3 *static_cast<GLdouble>(w) / h, 3 * static_cast<GLdouble>(w) / h, -3 * 1.0, 3 * 1.0, -3 * 100.0, 3 * 100.0);
	else
		glOrtho(-3 * 1.0, 3 * 1.0, -3 * static_cast<GLdouble>(h) / w, 3 * static_cast<GLdouble>(h) / w, -3 * 100.0, 3 * 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}



/*
//  键盘交互 1. 切换文件 2.切换显示
void mySpecial(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_F1:
		cout << "读取文件:" << file_1 << " 中......" << endl;
		readfile(file_1);
		scale = 1.0;
		currentfile = 1;
		initGL();
		break;
	case GLUT_KEY_F2:
		cout << "读取文件:" << file_2 << " 中......" << endl;
		readfile(file_2);
		scale = 1.2;
		currentfile = 2;
		initGL();
		break;
	case GLUT_KEY_F3:
		cout << "读取文件:" << file_3 << " 中......" << endl;
		readfile(file_3);
		scale = 0.01;
		currentfile = 3;
		initGL();
		break;
	case GLUT_KEY_F4:
		if (showFace == true) {
			showFace = false;
			showWire = true;
			cout << "切换显示模式为：WireFrame" << endl;
		}
		else if (showWire == true)
		{
			showWire = false;
			showFlatlines = true;
			cout << "切换显示模式为：Flatlines" << endl;
		}
		else if (showFlatlines == true) {
			showFlatlines = false;
			showFace = true;
			cout << "切换显示模式为：Flat" << endl;
		}
		break;
	case GLUT_KEY_UP:
		ty += 0.01;
		break;
	case GLUT_KEY_DOWN:
		ty -= 0.01;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}*/

// 鼠标交互
void myMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mousetate = 1;
		Oldx = x;
		Oldy = y;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		mousetate = 0;
	//滚轮事件
	if (state == GLUT_UP && button == 3) {
			scale -= 0.1;
	}
	if (state == GLUT_UP && button == 4) {
			scale += 0.1;
	}
	glutPostRedisplay();
}

// 鼠标运动时
void onMouseMove(int x, int y) {
	if (mousetate) {
		//x对应y是因为对应的是法向量
		yRotate += x - Oldx;
		glutPostRedisplay();
		Oldx = x;
		xRotate += y - Oldy;
		glutPostRedisplay();
		Oldy = y;
	}
}

void myDisplay()
{
	//要清除之前的深度缓存
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//与显示相关的函数
	glRotatef(xRotate, 1.0f, 0.0f, 0.0f); // 让物体旋转的函数 第一个参数是角度大小，后面的参数是旋转的法向量
	glRotatef(yRotate, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, ty);
	glScalef(scale, scale, scale); // 缩放

								   //每次display都要使用glcalllist回调函数显示想显示的顶点列表
	model.draw();

	glutSwapBuffers(); //这是Opengl中用于实现双缓存技术的一个重要函数
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // GLUT_Double 表示使用双缓存而非单缓存
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 500);
	glutCreateWindow("Mesh Viewer");
	//一开始默认读取文件1
	model.readfile("cube.obj");
	model.parse(); 
	glutMouseFunc(myMouse);
	glutMotionFunc(onMouseMove); // 鼠标移动的时候的函数
	glutReshapeFunc(&myReshape);
	glutDisplayFunc(&myDisplay);

	glutMainLoop();
	return 0;
}