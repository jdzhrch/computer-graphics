#pragma once
#include<stack>
#include<vector>
#include "Particle.h"
//used to store the particles
struct ParticleSystem
{
private:
	std::stack<Particle *> freeParticles;//LIFO for memory performance
	std::vector<Particle *> busyParticles;
	ParticleSystem(){}
	ParticleSystem(int particleNum);
	Particle * get_particle();
public:
	void update();
	void emit();
	void free_particle(int i);
};

