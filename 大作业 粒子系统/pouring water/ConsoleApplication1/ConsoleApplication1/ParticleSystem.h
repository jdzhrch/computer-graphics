#pragma once
#include<stack>
#include<vector>
#include "Particle.h"

class ParticleSystem {
	float x1, y1, x2, y2, z1, z2; //������λ��  
	Mesh *mesh;
	bool isModel;
	int speed;//��������  
	particle **p;//��������  
	particle* (*f)(); //��ʼ�����ӵĺ���ָ��  
public:
	void emit(particle* (init)());
	void update();
	ParticleSystem(int _speed, float _x1,float _x2, float _y1, float _y2, float _z1, float _z2,bool _isModel);
};