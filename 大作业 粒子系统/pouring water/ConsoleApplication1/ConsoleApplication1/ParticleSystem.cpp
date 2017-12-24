#include "ParticleSystem.h"
static const char* objfilename = "Cube.obj";
void ParticleSystem::emit(particle* (init)()) {
	for (int i = 0; i < speed; i++) {
		f = init;
		p[i] = init();
		int place = rand() % speed;
		p[i]->pos.x = 1.0f*place / speed*(x2 - x1) + x1;
		p[i]->pos.y = 1.0f*place / speed*(y2 - y1) + y1;
		p[i]->pos.z = 1.0f*place / speed*(z2 - z1) + z1;
	}
}

void ParticleSystem::update() {
	for (int i = 0; i < speed; i++) {
		//如果加载模型，则调用相应draw函数
		if(isModel){
			p[i]->draw(mesh);
		}else {
			p[i]->draw();
		}
		if (p[i]->life < 0) {
			delete p[i];
			p[i] = f();
			int place = rand() % speed;
			p[i]->pos.x = 1.0f*place / speed*(x2 - x1) + x1;
			p[i]->pos.y = 1.0f*place / speed*(y2 - y1) + y1;
			p[i]->pos.z = 1.0f*place / speed*(z2 - z1) + y1;
		}
	}
}

ParticleSystem::ParticleSystem(int _speed, float _x1,float _x2, float _y1, float _y2, float _z1, float _z2,bool _isModel) {
	speed = _speed;
	x1 = _x1;
	x2 = _x2;
	y1 = _y1;
	y2 = _y2;
	z1 = _z1;
	z2 = _z2;
	isModel = _isModel;
	mesh = new Mesh;
	mesh->parse(objfilename);
	p = new particle*[speed];
}