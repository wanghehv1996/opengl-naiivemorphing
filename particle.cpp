#include "particle.h"

Particle initParticle(Point genPtr){
	Particle particle;
	particle.color.r = rand() / double(RAND_MAX);
	particle.color.g = rand() / double(RAND_MAX);
	particle.color.b = rand() / double(RAND_MAX);
	particle.color.alpha = 1;
	particle.x = genPtr.x;
	particle.y = genPtr.y;
	particle.z = genPtr.z;

	particle.vx = rand() / double(RAND_MAX) / 10 - 0.05;
	particle.vy = rand() / double(RAND_MAX) / 10 - 0.05;
	particle.vz = rand() / double(RAND_MAX) / 10 - 0.05;
	particle.ax = 0;
	particle.ay = -0.001;
	particle.az = 0;
	particle.lifetime = 150;
	particle.size = 0.01*(rand()%3+1);
	particle.show = 0 - rand() % 150;
	return particle;
}

Particle initParticle(TriPoint genPtr){
	Particle particle;
	particle.color.r = rand() / double(RAND_MAX);
	particle.color.g = rand() / double(RAND_MAX);
	particle.color.b = rand() / double(RAND_MAX);
	particle.color.alpha = 1;
	particle.x = genPtr.x;
	particle.y = genPtr.y;
	particle.z = genPtr.z;
	float scale = sqrt(genPtr.nx*genPtr.nx + genPtr.ny*genPtr.ny + genPtr.nz*genPtr.nz);
	particle.vx = genPtr.nx / scale*0.1*(rand() / double(RAND_MAX));// / 2 - 0.25);
	particle.vy = genPtr.ny / scale*0.1*(rand() / double(RAND_MAX));// / 2 - 0.25);
	particle.vz = genPtr.nz / scale*0.1*(rand() / double(RAND_MAX));// / 2 - 0.25);
	particle.ax = 0;
	particle.ay = -0.001;
	particle.az = 0;
	particle.lifetime = 200;
	particle.size = 0.01*(rand() % 3 + 1);
	particle.show = 0 - rand() % 200;
	return particle;
}

void Particle::move(){
	show++;
	if (show <= 0) return;
	if (show > lifetime) return;
	this->x += this->vx;
	this->y += this->vy;
	if (this->y < -3)
		this->vy = -this->vy;
	this->z += this->vz;
	this->vx += this->ax;
	this->vy += this->ay;
	this->vz += this->az;
	this->color.alpha = 1 - show*1.0 / lifetime;
	if (color.alpha < 0)
		color.alpha = 0;
}

void Particle::moveWithWind(Point wind){
	show++;
	if (show <= 0) return;
	if (show > lifetime) return;
	this->x += this->vx;
	this->y += this->vy;
	this->z += this->vz;
	this->vx += this->ax;
	this->vy += this->ay;
	this->vz += this->az;
	this->ax += wind.x;
	this->ay += wind.y;
	this->az += wind.z;
	this->color.alpha = 1 - show*1.0 / lifetime;
	if (color.alpha < 0)
		color.alpha = 0;
}

void Particle::moveWithAcc(Point acc){
	show++;
	if (show <= 0) return;
	if (show > lifetime) return;
	this->x += this->vx;
	this->y += this->vy;
	this->z += this->vz;
	this->vx += this->ax;
	this->vy += this->ay;
	this->vz += this->az;
	this->ax = acc.x;
	this->ay = acc.y;
	this->az = acc.z;
	this->color.alpha = 1 - show*1.0 / lifetime;
	if (color.alpha < 0)
		color.alpha = 0;
}

void Particle::moveTo(Point dst){
	show++;
	if (show <= 0) return;
	if (show > lifetime) return;
	this->x = dst.x;
	this->y = dst.y;
	this->z = dst.z;

	this->color.alpha = 1 - show*1.0 / lifetime;
	if (color.alpha < 0)
		color.alpha = 0;
}


void Particle::moveWithPCircle(int axis,float radius){
	if (show <= 0) {
		show++;
		return;
	}
	//if (show > lifetime) return;

	float a, b;
	if (axis == 1){
		a = this->ax;
		this->ax += PI / 180;
		b = this->ay;
		this->ay += PI / 180;
		this->y = sin(b)*radius;
		this->z = cos(a)*cos(b)*radius;
		this->x = sin(a)*cos(b)*radius;
	}
	else if (axis == 2){
		a = this->ay;
		this->ay += PI / 180;
		b = this->az;
		this->az += PI / 180;
		this->z= sin(b)*radius;
		this->x = cos(a)*cos(b)*radius;
		this->y = sin(a)*cos(b)*radius;
	}
	else if (axis == 3){
		a = this->az;
		this->az += PI / 180;
		b = this->ax;
		this->ax += PI / 180;
		this->x = sin(b)*radius;
		this->y = cos(a)*cos(b)*radius;
		this->z = sin(a)*cos(b)*radius;
	}

	this->color.alpha = 1 - show*1.0 / lifetime;
	if (color.alpha < 0)
		color.alpha = 0;
}