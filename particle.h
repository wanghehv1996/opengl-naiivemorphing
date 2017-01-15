#ifndef _PARTICLE_H_
#define _PARTICLE_H_
#pragma once
#include <vector>
#include "model.h"

typedef struct
{
	float r;
	float g;
	float b;
	float alpha;
}Color;

class Particle{
public:
	float x, y, z;
	Color color;
	float vx, vy, vz;
	float ax, ay, az;
	int lifetime;
	float size;
	int show;
	void move();
	void moveWithWind(Point wind);
	void moveWithAcc(Point acc);
	void moveTo(Point dst);
	void moveWithPCircle(int axis, float radius);
};

Particle initParticle(Point p);
Particle initParticle(TriPoint p);

#endif