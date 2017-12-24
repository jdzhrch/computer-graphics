#include "basicElement.h"
#include "sph_ParticlePool.h"

PointPool::PointPool(unsigned int capacity) {
	pp_capacity = capacity;
	pp_particleBuffer = (sph_particle*)malloc(pp_capacity * sizeof(sph_particle));
	pp_curNum = 0;
}

sph_particle* PointPool::AddPointReuse(void)
{
	if (pp_curNum * 2>pp_capacity)
	{
		//get a particle randomly
		unsigned int index = rand() % pp_curNum;
		return pp_particleBuffer + index;
	} 
	//a new point
	sph_particle* point = pp_particleBuffer + pp_curNum;
	sph_particle p;
	*point = p;
	pp_curNum++;
	return point;
}

