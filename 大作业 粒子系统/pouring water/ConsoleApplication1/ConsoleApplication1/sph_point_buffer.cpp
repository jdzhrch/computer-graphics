#include "basicElement.h"
#include "sph_point_buffer.h"
	void PointBuffer::reset(unsigned int capcity)
	{
		mBufCapcity = capcity;
		if (mFluidBuf != 0)
		{
			free(mFluidBuf);
			mFluidBuf = 0;
		}

		if (mBufCapcity>0)
		{
			mFluidBuf = (Particle2*)malloc(mBufCapcity * sizeof(Particle2));
		}
		mFluidCounts = 0;
	}

	//-----------------------------------------------------------------------------------------------------------------
	Particle2* PointBuffer::AddPointReuse(void)
	{
		if (mFluidCounts * 2>mBufCapcity)
		{
			//get a random point
			unsigned int index = rand() % mFluidCounts;
			return mFluidBuf + index;
		} 
		//a new point
		Particle2* point = mFluidBuf + (mFluidCounts++);
		Particle2 p;
		*point = p;
		return point;
	}
