#pragma once
#include <iostream>  
#include <OpenMesh/Core/IO/MeshIO.hh>  
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>  
#include "glut.h"
#include <math.h>
#include <Windows.h>
#include <string>
using namespace std;
typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh;
class Model
{
	//文件读取有关的
	MyMesh mesh;
	int currentfile = 1;

	GLuint showFaceList, showWireList;
	int showstate = 1;
	bool showFace = true;
	bool showWire = false;
	bool showFlatlines = false;
public:
	void draw();
	void readfile(string filename);
	void parse();
	Model();
	~Model();
};

