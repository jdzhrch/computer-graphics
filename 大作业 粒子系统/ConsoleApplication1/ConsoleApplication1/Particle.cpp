#include "Particle.h"
#define WATER_COLOR { 11, 172, 207, 0.5 }
static float dt = 0.01;//Á½Ö¡¼ä¸ô

void Particle::update(){
	Vector accel(force.x/mass, force.y / mass, force.z / mass);
	//update speed
	speed.x += accel.x * dt;
	speed.y += accel.y * dt;
	speed.z += accel.z * dt;
	//update position
	pos.x += speed.x * dt;
	pos.y += speed.y * dt;
	pos.z += speed.z * dt;
}

void Particle::draw(){
	update();
	glEnableClientState(GL_COLOR_ARRAY);
	GLfloat color[4] = WATER_COLOR;
	glColorPointer(4, GL_FLOAT, 0, color);
	glBegin(GL_POINTS);
	glVertex3f(pos.x,pos.y,pos.z);
	glEnd();
}
