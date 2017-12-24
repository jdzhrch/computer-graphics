#ifndef XXXX_H  
#define XXXX_H 
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <math.h>
#include <assert.h>

// CPP Runtime library
#include <string>
#include <vector>
#include <list>
#include <map>
#include <limits>
#include <stack>
#include <set>
#include <algorithm>

#include <glut.h> 
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
struct float_3
{
	float x, y, z;
};


#endif