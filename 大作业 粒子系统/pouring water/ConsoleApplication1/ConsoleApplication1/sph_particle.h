#pragma once
#include <glut.h>    
#include "sph_math.h"	
struct sph_particle
{
public:
	fVector3		pos;		//λ��
	float			density;	//�ܶ�
	float			pressure;	//ѹ��
	fVector3		accel;		//���ٶ�

	fVector3		velocity;
	fVector3		velocity_eval;

	int				next;

	float life;
	unsigned char color[4];

	sph_particle() {
		pos.set(0, 0, 0);
		next = 0;
		velocity.set(0, 0, 0);
		velocity_eval.set(0, 0, 0);
		pressure = 0;
		density = 0;
		accel.set(0, 0, 0);
	}
};
