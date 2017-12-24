#pragma once
#include<vector>
#include <glut.h> 
#include "basicElement.h"
using namespace std;
struct Texture {
	float tx;
	float ty;
	Texture() {}
	Texture(float tx, float ty) :tx(tx), ty(ty) {}
};
struct NormalVector {
	float nx;
	float ny;
	float nz;
	NormalVector() {}
	NormalVector(float nx, float ny, float nz) :nx(nx), ny(ny), nz(nz) {}
};
struct Face {
	int vertexId[3];
	int textureId[3];
	int normalVectorId[3];
	Face() {//用于检验f里面是否有这一类型数据 根据格式不同 由于ID下标从1开始 因此此处初始化为0用于检验
		for (int i = 0; i<3; i++) {
			vertexId[i] = 0;
			textureId[i] = 0;
			normalVectorId[i] = 0;
		}
	}
};
class Mesh {
public:
	vector<Vertex> vertexes;
	vector<Texture>  textures;
	vector<NormalVector> normalVectors;
	vector<Face> faces;

	void parse(const char* filename);
	void Mesh::draw();
};
