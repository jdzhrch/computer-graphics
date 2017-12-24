#pragma once  
#define GLUT_DISABLE_ATEXIT_HACK  
#include <glut.h>    
#include "ObjLoader.h"


struct particle {
	Vertex pos;
	Vector speed;
	Vector acc;
	float life;
	unsigned char color[4];
	void draw();
	void draw(Mesh *mesh);
	particle(){}
	particle(Vector _speed,Vector _acc,float _life, unsigned char* _color);
};

