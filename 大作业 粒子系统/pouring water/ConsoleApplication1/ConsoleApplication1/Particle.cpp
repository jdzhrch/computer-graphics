#include<stdlib.h>  
#include"Particle.h"  
#define dt 0.5
float particleSize = rand() % 15 * 0.02f;
void particle::draw() {
	if (life > 0) {
			glPushMatrix();
			glColor4f(color[0], color[1], color[2],color[3]);
			glTranslatef(pos.x, pos.y, pos.z);
			//glScalef(particleSize, particleSize, particleSize);
			glutSolidSphere(0.5, 4, 4);
			glPopMatrix();
			pos.z += speed.z ;
			pos.y += speed.y ;
			pos.x += speed.x ;
			speed.z += acc.z ;
			speed.y += acc.y ;
			speed.x += acc.x ;
			life -= 0.2f;
			glColor3f(0, 0, 0);
		}
}

void particle::draw(Mesh *mesh) {
	if (life > 0) {
		glPushMatrix();
		glColor4f(color[0], color[1], color[2], color[3]);
		glTranslatef(pos.x, pos.y, pos.z);
		//glScalef(particleSize, particleSize, particleSize);
		mesh->draw();
		glPopMatrix();
		pos.z += speed.z;
		pos.y += speed.y;
		pos.x += speed.x;
		speed.z += acc.z;
		speed.y += acc.y;
		speed.x += acc.x;
		life -= 0.2f;
		glColor3f(0, 0, 0);
	}
}

particle::particle(Vector _speed,Vector _acc,float _life, unsigned char* _color) {
	speed = _speed;
	acc = _acc;
	life = _life;
	color[0] = *_color;
	color[1] = *(_color + 1);
	color[2] = *(_color + 2);
	color[2] = *(_color + 3);
}
