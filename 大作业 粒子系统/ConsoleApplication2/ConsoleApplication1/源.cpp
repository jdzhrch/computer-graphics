#include "Model.h"


//��꽻���йص�
int mousetate = 0; //��굱ǰ��״̬
GLfloat Oldx = 0.0; // ���֮ǰ��λ��
GLfloat Oldy = 0.0;
//��ʵ�ֽǶȴ�С��صĲ�����ֻ��Ҫ�����Ϳ������
float xRotate = 0.0f;
float yRotate = 0.0f;
float ty = 0.0f;
float scale = 1;

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



/*
//  ���̽��� 1. �л��ļ� 2.�л���ʾ
void mySpecial(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_F1:
		cout << "��ȡ�ļ�:" << file_1 << " ��......" << endl;
		readfile(file_1);
		scale = 1.0;
		currentfile = 1;
		initGL();
		break;
	case GLUT_KEY_F2:
		cout << "��ȡ�ļ�:" << file_2 << " ��......" << endl;
		readfile(file_2);
		scale = 1.2;
		currentfile = 2;
		initGL();
		break;
	case GLUT_KEY_F3:
		cout << "��ȡ�ļ�:" << file_3 << " ��......" << endl;
		readfile(file_3);
		scale = 0.01;
		currentfile = 3;
		initGL();
		break;
	case GLUT_KEY_F4:
		if (showFace == true) {
			showFace = false;
			showWire = true;
			cout << "�л���ʾģʽΪ��WireFrame" << endl;
		}
		else if (showWire == true)
		{
			showWire = false;
			showFlatlines = true;
			cout << "�л���ʾģʽΪ��Flatlines" << endl;
		}
		else if (showFlatlines == true) {
			showFlatlines = false;
			showFace = true;
			cout << "�л���ʾģʽΪ��Flat" << endl;
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
	}
}

void myDisplay()
{
	//Ҫ���֮ǰ����Ȼ���
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//����ʾ��صĺ���
	glRotatef(xRotate, 1.0f, 0.0f, 0.0f); // ��������ת�ĺ��� ��һ�������ǽǶȴ�С������Ĳ�������ת�ķ�����
	glRotatef(yRotate, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, ty);
	glScalef(scale, scale, scale); // ����

								   //ÿ��display��Ҫʹ��glcalllist�ص�������ʾ����ʾ�Ķ����б�
	model.draw();

	glutSwapBuffers(); //����Opengl������ʵ��˫���漼����һ����Ҫ����
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // GLUT_Double ��ʾʹ��˫������ǵ�����
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 500);
	glutCreateWindow("Mesh Viewer");
	//һ��ʼĬ�϶�ȡ�ļ�1
	model.readfile("cube.obj");
	model.parse(); 
	glutMouseFunc(myMouse);
	glutMotionFunc(onMouseMove); // ����ƶ���ʱ��ĺ���
	glutReshapeFunc(&myReshape);
	glutDisplayFunc(&myDisplay);

	glutMainLoop();
	return 0;
}