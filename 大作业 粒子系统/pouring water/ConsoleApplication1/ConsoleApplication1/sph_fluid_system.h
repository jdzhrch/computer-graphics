#pragma once
#include "basicElement.h"
#include "sph_ParticlePool.h"
#include "sph_grid_container.h"
#include "sph_neighbor_table.h"

class FluidSystem
{
public:
	virtual void init(unsigned short maxPointCounts,
		const float_3* wallBox_min, const float_3* wallBox_max,
		const float_3* initFluidBox_min, const float_3* initFluidBox_max,
		const float_3* gravity)
	{
		_init(maxPointCounts,
			fBox3(fVector3(wallBox_min), fVector3(wallBox_max)),
			fBox3(fVector3(initFluidBox_min), fVector3(initFluidBox_max)),
			fVector3(gravity));
	}

	/** Get size of point in bytes */
	virtual unsigned int getPointStride(void) const { return sizeof(sph_particle); }
	/** Get point counts */
	virtual unsigned int getPointCounts(void) const { return m_pointBuffer.size(); }
	/** Get Fluid sph_particle Buffer */
	virtual const float_3* getPointBuf(void) const { return (const float_3*)m_pointBuffer.getParticle(0); }
	/** �߼�֡ */
	virtual void tick(void);

private:
	/** ��ʼ��ϵͳ
	*/
	void _init(unsigned short maxPointCounts, const fBox3& wallBox, const fBox3& initFluidBox, const fVector3& gravity);
	/** �����ܶȡ�ѹ�����Լ����ڹ�ϵ */
	void _computePressure(void);
	/** ������ٶ� */
	void _computeForce(void);
	/** �ƶ�����*/
	void _advance(void);
	/** ������ʼҺ���*/
	void _addFluidVolume(const fBox3& fluidBox, float spacing);

private:
	PointPool m_pointBuffer;
	GridContainer m_gridContainer;
	NeighborTable m_neighborTable;

	// SPH Kernel
	float m_kernelPoly6;
	float m_kernelSpiky;
	float m_kernelViscosity;

	//Other Parameters
	float m_unitScale;
	float m_viscosity;
	float m_restDensity;
	float m_pointMass;
	float m_smoothRadius;
	float m_gasConstantK;
	float m_boundartStiffness;
	float m_boundaryDampening;
	float m_speedLimiting;
	fVector3 m_gravityDir;

	fBox3 m_sphWallBox;
public:
	FluidSystem();
	~FluidSystem();
};

