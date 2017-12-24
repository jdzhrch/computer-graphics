#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(int particleNum)
{
	Particle * particle = nullptr;
	for(int i = 0; i < particleNum ;i++){
		particle = new Particle;
		freeParticles.push(particle);
	}
}

Particle * ParticleSystem::get_particle()
{
	Particle *particle = freeParticles.top();
	freeParticles.pop();//是否销毁指针
	busyParticles.push_back(particle);
	return particle;
}

void ParticleSystem::free_particle(int i)
{
	Particle * particle = busyParticles.at(i);
	freeParticles.push(particle);
	busyParticles.erase(busyParticles.begin() + i);//是否销毁指针
}

void ParticleSystem::update() {
	for (int i = 0; i < busyParticles.size(); i++) {
		busyParticles.at(i)->update();
	}
}

void ParticleSystem::emit() {
	Particle *particle = get_particle();
	particle->draw();
}
