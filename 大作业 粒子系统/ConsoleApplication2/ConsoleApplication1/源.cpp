#include "Model.h"


struct CPoint {
	double x;
	double y;
	void SetPoint(double a, double b) {
		x = a; y = b;
	}
};
double PI;
double tFovy;                   // 在gluPerspective中使用(在OnMouseWheel中使用)
CPoint prePt, nowPt;                                        // 在OnMouseMove中使用
double tEyeX, tEyeY, tEyeZ;                                    // 在gluLookAt中使用的3个向量
double tCenterX, tCenterY, tCenterZ;
double tUpX, tUpY, tUpZ;
double tVerticalAng, tHorizonAng, tRadius, tAngInc; // 水平方向、垂直方向的角、半径
//鼠标交互有关的
int mousetate = 0; //鼠标当前的状态
GLfloat Oldx = 0.0; // 点击之前的位置
GLfloat Oldy = 0.0;
//与实现角度大小相关的参数，只需要两个就可以完成
float oldxRotate = 0.0f;
float oldyRotate = 0.0f;
float xRotate = 0.0f;
float yRotate = 0.0f;
float ty = 0.0f;
float scale = 0.2;

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




//  键盘交互 1. 切换文件 2.切换显示
void mySpecial(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		ty += 0.01;
		tFovy += 1.0;
		break;
	case GLUT_KEY_DOWN:
		ty -= 0.01;
		tFovy -= 1.0;
		break;
	default:
		break;
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();                                            // 此处尤其不可少glLoadIdentity()
	gluPerspective(tFovy, 1, 0.1, 1000.0);                        // 注意zNear,zFar的取值
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutPostRedisplay();
}


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

		nowPt.x = x;
		nowPt.y = y;
		if (prePt.x != -1 && prePt.y != -1 && nowPt.x != -1 && nowPt.y != -1) {
			double tDx = nowPt.x - prePt.x;
			double tDy = nowPt.y - prePt.y;
			double tDis = sqrt(tDx*tDx + tDy*tDy);
			if (tDx > 0.) {
				tHorizonAng += tAngInc * tDx / tDis;
				if (tHorizonAng < 0.) { tHorizonAng += 2 * PI; }
				if (tHorizonAng > 2 * PI) { tHorizonAng -= 2 * PI; }
			}
			else if (tDx < 0.) {
				tHorizonAng += tAngInc * tDx / tDis;
				if (tHorizonAng < 0.) { tHorizonAng += 2 * PI; }
				if (tHorizonAng > 2 * PI) { tHorizonAng -= 2 * PI; }
			}
			if (tDy > 0.) {
				tVerticalAng = tVerticalAng + tAngInc * tDy / tDis;
				if (tVerticalAng > PI / 2) { tVerticalAng = PI / 2; }
			}
			else if (tDy < 0.) {
				tVerticalAng = tVerticalAng + tAngInc * tDy / tDis;
				if (tVerticalAng < -PI / 2) { tVerticalAng = -PI / 2; }
			}

			tEyeX = tRadius * cos(tVerticalAng) * cos(tHorizonAng);
			tEyeY = tRadius * sin(tVerticalAng);
			tEyeZ = tRadius * cos(tVerticalAng) * sin(tHorizonAng);
		}
		prePt.x = nowPt.x;
		prePt.y = nowPt.y;
	}
}

void myDisplay()
{

	//要清除之前的深度缓存
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(tEyeX, tEyeY, tEyeZ, tCenterX, tCenterY, tCenterZ, tUpX, tUpY, tUpZ);
	//glRotatef(xRotate, 1.0f, 0.0f, 0.0f); // 让物体旋转的函数 第一个参数是角度大小，后面的参数是旋转的法向量
	//glRotatef(yRotate, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.0f);
	glScalef(0.1 * scale, 0.1 * scale, 0.1 * scale);
	//glColor3f(0, 0, 0);
	model.draw();
	glPopMatrix();
	glFlush();

	glutSwapBuffers(); //这是Opengl中用于实现双缓存技术的一个重要函数
}

int main(int argc, char** argv)
{
	tAngInc = PI / 90;                      // 每次触发鼠标事件变换的角度值
	tFovy = 45.0;
	prePt.SetPoint(-1, -1);                 // 初始化prePt & nowPt(在OnMouseMove中使用)
	nowPt.SetPoint(-1, -1);
	tVerticalAng = 0.;
	tHorizonAng = PI / 2;
	tRadius = 400.0;
	tEyeX = tRadius * cos(tVerticalAng) * cos(tHorizonAng);
	tEyeY = tRadius * sin(tVerticalAng);
	tEyeZ = tRadius * cos(tVerticalAng) * sin(tHorizonAng);
	tCenterX = 0.;
	tCenterY = 0.;
	tCenterZ = 0.;
	tUpX = 0.;
	tUpY = 1.0;
	tUpZ = 0.;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // GLUT_Double 表示使用双缓存而非单缓存
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 500);
	glutCreateWindow("Mesh Viewer");
	//一开始默认读取文件1
	model.readfile("model_sink/file.obj");
	model.parse(); 
	glutMouseFunc(myMouse);
	glutMotionFunc(onMouseMove); // 鼠标移动的时候的函数
	glutReshapeFunc(&myReshape);
	glutDisplayFunc(&myDisplay);
	glutSpecialFunc(&mySpecial);

	glutMainLoop();
	return 0;
}