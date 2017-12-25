#include <iostream>  
#include <OpenMesh/Core/IO/MeshIO.hh>  
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>  
#include "glut.h"
#include <math.h>
#include <Windows.h>
#include <string>

using namespace std;
typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh;

//鼠标交互有关的
int mousetate = 0; //鼠标当前的状态
GLfloat Oldx = 0.0; // 点击之前的位置
GLfloat Oldy = 0.0;
//与实现角度大小相关的参数，只需要两个就可以完成
float xRotate = 0.0f;
float yRotate = 0.0f;
float ty = 0.0f;
float scale = 1;

//文件读取有关的
MyMesh mesh;
const string file_1 = "Bigmax_White_OBJ.obj";
const string file_2 = "cactus.ply";
const string file_3 = "Armadillo.off";
int currentfile = 1;

GLuint showFaceList, showWireList;
int showstate = 1;
bool showFace = true;
bool showWire = false;
bool showFlatlines = false;

//初始化顶点和面   
void initGL()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(2.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST); //用来开启深度缓冲区的功能，启动后OPengl就可以跟踪Z轴上的像素，那么它只有在前面没有东西的情况下才会绘制这个像素，在绘制3d时，最好启用，视觉效果会比较真实
							 // ------------------- Lighting  
	glEnable(GL_LIGHTING); // 如果enbale那么就使用当前的光照参数去推导顶点的颜色
	glEnable(GL_LIGHT0); //第一个光源，而GL_LIGHT1表示第二个光源
						 // ------------------- Display List  
	showFaceList = glGenLists(1);
	showWireList = glGenLists(1);
	int temp = mesh.n_edges();

	// 绘制 wire 
	glNewList(showWireList, GL_COMPILE);
	glDisable(GL_LIGHTING);
	glLineWidth(1.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_LINES);
	for (MyMesh::HalfedgeIter he_it = mesh.halfedges_begin(); he_it != mesh.halfedges_end(); ++he_it) {
		//链接这个有向边的起点和终点
		glVertex3fv(mesh.point(mesh.from_vertex_handle(*he_it)).data());
		glVertex3fv(mesh.point(mesh.to_vertex_handle(*he_it)).data());
	}
	glEnd();
	glEnable(GL_LIGHTING);
	glEndList();

	// 绘制flat
	glNewList(showFaceList, GL_COMPILE);
	for (MyMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it) {
		glBegin(GL_TRIANGLES);
		for (MyMesh::FaceVertexIter fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it) {
			glNormal3fv(mesh.normal(*fv_it).data());
			glVertex3fv(mesh.point(*fv_it).data());
		}
		glEnd();
	}
	glEndList();
}

// 当窗体改变大小的时候
void myReshape(GLint w, GLint h)
{
	glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w > h)
		glOrtho(-30*static_cast<GLdouble>(w) / h, 30 * static_cast<GLdouble>(w) / h, -30 * 1.0, 30 * 1.0, -30 * 100.0, 30 * 100.0);
	else
		glOrtho(-30 * 1.0, 30 * 1.0, -30 * static_cast<GLdouble>(h) / w, 30 * static_cast<GLdouble>(h) / w, -30 * 100.0, 30 * 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


// 读取文件的函数
void readfile(string file) {
	// 请求顶点法线 vertex normals
	mesh.request_vertex_normals();
	//如果不存在顶点法线，则报错 
	if (!mesh.has_vertex_normals())
	{
		cout << "错误：标准定点属性 “法线”不存在" << endl;
		return;
	}
	// 如果有顶点发现则读取文件 
	OpenMesh::IO::Options opt;
	if (!OpenMesh::IO::read_mesh(mesh, file, opt))
	{
		cout << "无法读取文件:" << file << endl;
		return;
	}
	else cout << "成功读取文件:" << file << endl;
	cout << endl; // 为了ui显示好看一些
				  //如果不存在顶点法线，则计算出
	if (!opt.check(OpenMesh::IO::Options::VertexNormal))
	{
		// 通过面法线计算顶点法线
		mesh.request_face_normals();
		// mesh计算出顶点法线
		mesh.update_normals();
		// 释放面法线
		mesh.release_face_normals();
	}
}

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
	if (showFace)
		glCallList(showFaceList);
	if (showFlatlines) {
		glCallList(showFaceList);
		glCallList(showWireList);
	}
	if (showWire)
		glCallList(showWireList);

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
	readfile(file_1);
	initGL();
	glutMouseFunc(myMouse);
	glutMotionFunc(onMouseMove); // 鼠标移动的时候的函数
	glutSpecialFunc(&mySpecial);
	glutReshapeFunc(&myReshape);
	glutDisplayFunc(&myDisplay);

	glutMainLoop();
	return 0;
}