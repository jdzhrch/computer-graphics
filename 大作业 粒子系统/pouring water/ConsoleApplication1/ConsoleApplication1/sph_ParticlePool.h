#pragma once
#include "sph_particle.h"

class PointPool
{
public:
	unsigned int size(void) const { return pp_curNum; }
	sph_particle* getParticle(unsigned int index) { return pp_particleBuffer + index; }
	const sph_particle* getParticle(unsigned int index) const { return pp_particleBuffer + index; }
	sph_particle* AddPointReuse(void);
	sph_particle* AddPoint(fVector3 pos);

private:
	sph_particle* pp_particleBuffer;
	unsigned int pp_curNum;
	unsigned int pp_capacity;

public:
	PointPool(){}
	PointPool(unsigned int capacity);
	virtual ~PointPool(){}
};
