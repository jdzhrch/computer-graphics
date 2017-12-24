#pragma once
#include <glut.h>    
#include "sph_math.h"	
struct sph_particle
{
public:
	fVector3		pos;		//位置
	float			density;	//密度
	float			pressure;	//压力
	fVector3		accel;		//加速度

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
