#define _CRT_SECURE_NO_WARNINGS      
#define GLUT_DISABLE_ATEXIT_HACK 

#include <stdlib.h>       
#include"ParticleSystem.h"    
#include "sph_fluid_system.h"

#include <OpenMesh/Core/IO/MeshIO.hh>  
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>  

using namespace std;
typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh;

//��꽻���йص�
int mousetate = 0; //��굱ǰ��״̬
GLfloat Oldx = 0.0; // ���֮ǰ��λ��
GLfloat Oldy = 0.0;
//��ʵ�ֽǶȴ�С��صĲ�����ֻ��Ҫ�����Ϳ������
float xRotate = 0.0f;
float yRotate = 0.0f;
float ty = 0.0f;
float scale = 1;

//�ļ���ȡ�йص�
MyMesh mesh;
const string file_1 = "Bunny.obj";
const string file_2 = "cactus.ply";
const string file_3 = "Armadillo.off";
int currentfile = 1;

GLuint showFaceList, showWireList;
int showstate = 1;
bool showFace = true;
bool showWire = false;
bool showFlatlines = false;


//Mesh *mesh;
const char * objfilename = "Cube.obj";
//const float scale = 1.0;				//ģ�����ű���
bool ifUseModel = false;				//�Ƿ�ʹ��ģ����Ϊ����

#define G 9.8
const float water_color[4] = { 89.0 / 255, 195.0 / 255, 226.0 / 255,0.1 };
FluidSystem*					g_pSPHSystem = 0;

//�ӵ�      
float center[] = { 3, -3, -3 };
float eye[] = { 0, 0, 0 };

void resetSPHSystem(void)
{
	float_3 wall_min = { -25, 00, -25 };
	float_3 wall_max = { 25, 50, 25 };
	float_3 fluid_min = { -15, 15, -15 };
	float_3 fluid_max = { 15, 35, 15 };
	float_3 gravity = { 0.0, -9.8f, 0 };
	g_pSPHSystem->init(1024 * 2, &wall_min, &wall_max, &fluid_min, &fluid_max, &gravity);
}

void initGL()
{
	/*glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(2.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	showFaceList = glGenLists(1);
	showWireList = glGenLists(1);
	int temp = mesh.n_edges();

	// ���� wire 
	glNewList(showWireList, GL_COMPILE);
	glDisable(GL_LIGHTING);
	glLineWidth(1.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_LINES);
	for (MyMesh::HalfedgeIter he_it = mesh.halfedges_begin(); he_it != mesh.halfedges_end(); ++he_it) {
		//�����������ߵ������յ�
		glVertex3fv(mesh.point(mesh.from_vertex_handle(*he_it)).data());
		glVertex3fv(mesh.point(mesh.to_vertex_handle(*he_it)).data());
	}
	glEnd();
	glEnable(GL_LIGHTING);
	glEndList();

	// ����flat
	glNewList(showFaceList, GL_COMPILE);
	for (MyMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it) {
		glBegin(GL_TRIANGLES);
		for (MyMesh::FaceVertexIter fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it) {
			glNormal3fv(mesh.normal(*fv_it).data());
			glVertex3fv(mesh.point(*fv_it).data());
		}
		glEnd();
	}
	glEndList();*/

	resetSPHSystem();
}

// ������ı��С��ʱ��
void myReshape(GLint w, GLint h)
{
	glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w > h)
		glOrtho(-30 * static_cast<GLdouble>(w) / h, 30 * static_cast<GLdouble>(w) / h, -30 * 1.0, 30 * 1.0, -30 * 100.0, 30 * 100.0);
	else
		glOrtho(-30 * 1.0, 30 * 1.0, -30 * static_cast<GLdouble>(h) / w, 30 * static_cast<GLdouble>(h) / w, -30 * 100.0, 30 * 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*
// ��ȡ�ļ��ĺ���
void readfile(string file) {
	// ���󶥵㷨�� vertex normals
	mesh.request_vertex_normals();
	//��������ڶ��㷨�ߣ��򱨴� 
	if (!mesh.has_vertex_normals())
	{
		cout << "���󣺱�׼�������� �����ߡ�������" << endl;
		return;
	}
	// ����ж��㷢�����ȡ�ļ� 
	OpenMesh::IO::Options opt;
	if (!OpenMesh::IO::read_mesh(mesh, file, opt))
	{
		cout << "�޷���ȡ�ļ�:" << file << endl;
		return;
	}
	else cout << "�ɹ���ȡ�ļ�:" << file << endl;
	cout << endl; // Ϊ��ui��ʾ�ÿ�һЩ
				  //��������ڶ��㷨�ߣ�������
	if (!opt.check(OpenMesh::IO::Options::VertexNormal))
	{
		// ͨ���淨�߼��㶥�㷨��
		mesh.request_face_normals();
		// mesh��������㷨��
		mesh.update_normals();
		// �ͷ��淨��
		mesh.release_face_normals();
	}
}

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
		if (currentfile == 3)
			scale -= 0.002;
		else
			scale -= 0.1;
	}
	if (state == GLUT_UP && button == 4) {
		if (currentfile == 3)
			scale += 0.002;
		else
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

	glPushMatrix();
	glLoadIdentity();

	glClearColor(0.3, 0.7, 0.5, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //������ɫ����Ȼ���       
	glMatrixMode(GL_PROJECTION);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(xRotate, 1.0f, 0.0f, 0.0f); // ��������ת�ĺ��� ��һ�������ǽǶȴ�С������Ĳ�������ת�ķ�����
	glRotatef(yRotate, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, ty);
	glScalef(scale, scale, scale); // ����
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 1, 0);

	g_pSPHSystem->tick();
	const float_3* p = g_pSPHSystem->getPointBuf();
	unsigned int stride = g_pSPHSystem->getPointStride();

	{
		GLfloat light_position[] = { 20.0f, 100.0f, 100.0f, 1.0f }; //��Դ��λ������������ϵԲ�ģ����������ʽ  
		GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //RGBAģʽ�Ļ����⣬Ϊ0  
		GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //RGBAģʽ��������⣬ȫ�׹�  
		GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };  //RGBAģʽ�µľ���� ��ȫ�׹�  
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

		//�����ƹ�  
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
	}

	{
		GLfloat mat_ambient[] = { 0.0f, 0.0f, 1.0f, 1.0f };  //������ʵĻ�������ɫ��ƭ��ɫ  
		GLfloat mat_diffuse[] = { 89.0 / 255, 195.0 / 255, 226.0 / 255,1 };  //������ʵ����������ɫ��ƫ��ɫ  
		GLfloat mat_specular[] = { 0.0f, 1.0f, 0.0f, 1.0f };   //������ʵľ��淴�����ɫ����ɫ  
		GLfloat mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };   //������ʵķ������ɫ��Ϊ0  
		GLfloat mat_shininess = 30.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	}
	//����Ϊ����
	if (!ifUseModel) {
		glEnable(GL_POINT_SMOOTH);//��ử��Բ�������Ǿ���
		glPointSize(20.0f);
		glBegin(GL_POINTS);
		//glColor4fv(water_color);			//ˮ��ɫ		
		for (unsigned int n = 0; n<g_pSPHSystem->getPointCounts(); n++)
		{
			//glScalef(scale, scale, scale);
			glVertex3f(p->x, p->y, p->z);
			p = (const float_3*)(((const char*)p) + stride);
		}
		glEnd();
		glDisable(GL_POINT_SMOOTH);
	}
	else {//ģ����Ϊ����
		for (unsigned int n = 0; n<g_pSPHSystem->getPointCounts(); n++)
		{
			glPushMatrix();
			//glScalef(scale, scale, scale);
			glTranslatef(p->x, p->y, p->z);
			//mesh->draw();
			glutSolidSphere(0.5, 4, 4);
			glPopMatrix();
			p = (const float_3*)(((const char*)p) + stride);
		}
	}
	glutSwapBuffers();
	glPopMatrix();
	glFlush();/*
	//Ҫ���֮ǰ����Ȼ���
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//����ʾ��صĺ���
	glRotatef(xRotate, 1.0f, 0.0f, 0.0f); // ��������ת�ĺ��� ��һ�������ǽǶȴ�С������Ĳ�������ת�ķ�����
	glRotatef(yRotate, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, ty);
	glScalef(scale, scale, scale); // ����

								   //ÿ��display��Ҫʹ��glcalllist�ص�������ʾ����ʾ�Ķ����б�
	if (showFace)
		glCallList(showFaceList);
	if (showFlatlines) {
		glCallList(showFaceList);
		glCallList(showWireList);
	}
	if (showWire)
		glCallList(showWireList);
		*/
	//glutSwapBuffers(); //����Opengl������ʵ��˫���漼����һ����Ҫ����
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
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // GLUT_Double ��ʾʹ��˫������ǵ�����
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 500);
	glutCreateWindow("��ˮģ��");
	//һ��ʼĬ�϶�ȡ�ļ�1
	//readfile(file_1);
	initGL();
	glutMouseFunc(myMouse);
	glutMotionFunc(onMouseMove); // ����ƶ���ʱ��ĺ���
	//glutSpecialFunc(&mySpecial);
	glutReshapeFunc(&myReshape);
	glutDisplayFunc(&myDisplay);
	glutIdleFunc(&idle);

	glutMainLoop();
	return 0;
}