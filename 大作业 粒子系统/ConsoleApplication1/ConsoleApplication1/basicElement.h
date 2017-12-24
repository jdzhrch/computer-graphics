#pragma once
#include <glut.h> 
#include <math.h>
struct Vertex {
	float x;
	float y;
	float z;
	Vertex() {}
	Vertex(float x, float y, float z) :x(x), y(y), z(z) {}
};
struct Vector {
	float x;
	float y;
	float z;
	Vector() {}
	Vector(float x, float y, float z) :x(x), y(y), z(z) {}
	float get_manitude() { return sqrt(x * x + y * y + z * z); }
};