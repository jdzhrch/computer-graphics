#pragma once
#include "sph_math.h"
struct Particle2
{
public:
	fVector3		pos;		//位置
	float			density;	//密度
	float			pressure;	//压力
	fVector3		accel;		//加速度

	fVector3		velocity;
	fVector3		velocity_eval;

	int				next;

	Particle2() {
		pos.set(0, 0, 0);
		next = 0;
		velocity.set(0, 0, 0);
		velocity_eval.set(0, 0, 0);
		pressure = 0;
		density = 0;
		accel.set(0, 0, 0);
	}
};

/** point buffer
*/
class PointBuffer
{
public:
	void reset(unsigned int capcity);
	unsigned int size(void) const { return mFluidCounts; }
	Particle2* get(unsigned int index) { return mFluidBuf + index; }
	const Particle2* get(unsigned int index) const { return mFluidBuf + index; }
	Particle2* AddPointReuse(void);

private:
	Particle2* mFluidBuf;
	unsigned int mFluidCounts;
	unsigned int mBufCapcity;

public:
	PointBuffer(){}
	virtual ~PointBuffer(){}
};
