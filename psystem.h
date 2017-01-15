#ifndef _PSYSTEM_H_
#define _PSYSTEM_H_

#include "particle.h"
#include <vector>
#include "model.h"

class ParticleSystem{
public:
	ParticleSystem(vector<TriPoint>genVec,int count);
	ParticleSystem(Point genPtr, int count);
	ParticleSystem(){ count = 0; };
	vector<Particle> getParticles();
	void move();
	void moveWithWind(Point wind);
	void moveWithAcc(Point acc);
	void moveCircle(Point center, float dz, float radius);
	void moveCircle0(Point center, float dz, float radius);
	void moveWithPCircle(int axis, float radius);
	void refreshDieP(int index);
private:
	int genmode;//1 for one point, 2 for tripoints
	int count;
	Point genPtr;
	vector<TriPoint>genVec;
	vector<Particle> particles;
};


#endif