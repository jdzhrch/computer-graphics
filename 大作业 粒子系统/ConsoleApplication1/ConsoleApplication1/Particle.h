#pragma once
#include "basicElement.h"
class Particle
{
public:
	Vector speed;
	Vector force;
	Vertex pos;
	float mass;
	float size;
	float age;
	float lifetime;
	Particle(){}
	void update();
	void draw();
};

