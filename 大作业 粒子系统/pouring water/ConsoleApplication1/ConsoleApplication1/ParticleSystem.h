#pragma once
#include<stack>
#include<vector>
#include "Particle.h"

class ParticleSystem {
	float x1, y1, x2, y2, z1, z2; //发射器位置  
	Mesh *mesh;
	bool isModel;
	int speed;//发射速率  
	particle **p;//发射粒子  
	particle* (*f)(); //初始化粒子的函数指针  
public:
	void emit(particle* (init)());
	void update();
	ParticleSystem(int _speed, float _x1,float _x2, float _y1, float _y2, float _z1, float _z2,bool _isModel);
};