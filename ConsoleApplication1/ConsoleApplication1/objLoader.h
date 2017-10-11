#pragma once
#include<vector>
#include <glut.h> 
using namespace std;
struct Vertex {
	float x;
	float y;
	float z;
	Vertex() {}
	Vertex(float x, float y, float z) :x(x), y(y), z(z) {}
};
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
