#include "Model.h"


struct CPoint {
	double x;
	double y;
	void SetPoint(double a, double b) {
		x = a; y = b;
	}
};
double PI;
double tFovy;                   // ��gluPerspective��ʹ��(��OnMouseWheel��ʹ��)
CPoint prePt, nowPt;                                        // ��OnMouseMove��ʹ��
double tEyeX, tEyeY, tEyeZ;                                    // ��gluLookAt��ʹ�õ�3������
double tCenterX, tCenterY, tCenterZ;
double tUpX, tUpY, tUpZ;
double tVerticalAng, tHorizonAng, tRadius, tAngInc; // ˮƽ���򡢴�ֱ����Ľǡ��뾶
//��꽻���йص�
int mousetate = 0; //��굱ǰ��״̬
GLfloat Oldx = 0.0; // ���֮ǰ��λ��
GLfloat Oldy = 0.0;
//��ʵ�ֽǶȴ�С��صĲ�����ֻ��Ҫ�����Ϳ������
float oldxRotate = 0.0f;
float oldyRotate = 0.0f;
float xRotate = 0.0f;
float yRotate = 0.0f;
float ty = 0.0f;
float scale = 0.2;

Model model;


// ������ı��С��ʱ��
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




//  ���̽��� 1. �л��ļ� 2.�л���ʾ
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
	glLoadIdentity();                                            // �˴����䲻����glLoadIdentity()
	gluPerspective(tFovy, 1, 0.1, 1000.0);                        // ע��zNear,zFar��ȡֵ
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutPostRedisplay();
}


// ��꽻��
void myMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mousetate = 1;
		Oldx = x;
		Oldy = y;

		
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		mousetate = 0;
	//�����¼�
	if (state == GLUT_UP && button == 3) {
			scale -= 0.1;
	}
	if (state == GLUT_UP && button == 4) {
			scale += 0.1;
	}
	glutPostRedisplay();
}

// ����˶�ʱ
void onMouseMove(int x, int y) {
	if (mousetate) {
		//x��Ӧy����Ϊ��Ӧ���Ƿ�����
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

	//Ҫ���֮ǰ����Ȼ���
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(tEyeX, tEyeY, tEyeZ, tCenterX, tCenterY, tCenterZ, tUpX, tUpY, tUpZ);
	//glRotatef(xRotate, 1.0f, 0.0f, 0.0f); // ��������ת�ĺ��� ��һ�������ǽǶȴ�С������Ĳ�������ת�ķ�����
	//glRotatef(yRotate, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.0f);
	glScalef(0.1 * scale, 0.1 * scale, 0.1 * scale);
	//glColor3f(0, 0, 0);
	model.draw();
	glPopMatrix();
	glFlush();

	glutSwapBuffers(); //����Opengl������ʵ��˫���漼����һ����Ҫ����
}

int main(int argc, char** argv)
{
	tAngInc = PI / 90;                      // ÿ�δ�������¼��任�ĽǶ�ֵ
	tFovy = 45.0;
	prePt.SetPoint(-1, -1);                 // ��ʼ��prePt & nowPt(��OnMouseMove��ʹ��)
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
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // GLUT_Double ��ʾʹ��˫������ǵ�����
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 500);
	glutCreateWindow("Mesh Viewer");
	//һ��ʼĬ�϶�ȡ�ļ�1
	model.readfile("model_sink/file.obj");
	model.parse(); 
	glutMouseFunc(myMouse);
	glutMotionFunc(onMouseMove); // ����ƶ���ʱ��ĺ���
	glutReshapeFunc(&myReshape);
	glutDisplayFunc(&myDisplay);
	glutSpecialFunc(&mySpecial);

	glutMainLoop();
	return 0;
}