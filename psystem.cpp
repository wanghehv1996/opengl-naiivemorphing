#include "psystem.h"
#include <random>
#include <time.h>
#include <vector>
#include <iostream>

ParticleSystem::ParticleSystem(vector<TriPoint>genVec, int count){
	this->genmode = 2;
	this->genVec.clear();
	int jmpint = genVec.size() / ( count / 20);
	cout << genVec.size() <<' '<< jmpint << endl;
	int actcount = 0;
	vector<TriPoint>::iterator iter = genVec.begin();
	srand(unsigned(time(0)));
	this->count = count;
	for (int i = 0; i < genVec.size()&&actcount<count; i += jmpint){
		this->genVec.push_back(genVec[i]);
		cout << "gen i =" << i << endl;
		for (int j = 0; j < 20 && actcount<count; j++){
			actcount++;
			this->particles.push_back(initParticle(genVec[i]));
		}
	}
}

ParticleSystem::ParticleSystem(Point genPtr, int count){
	this->genmode = 1;
	srand(unsigned(time(0)));
	this->count = count;
	for (int i = 0; i < count; i++){
		particles.push_back(initParticle(genPtr));
	}
}


vector<Particle> ParticleSystem::getParticles(){
	return this->particles;
}

void ParticleSystem::move(){
	for (int i = 0; i < count; i++){
		particles[i].move();
	}
}

void ParticleSystem::moveWithWind(Point wind){
	for (int i = 0; i < count; i++){
		particles[i].moveWithWind(wind);
	}
}

void ParticleSystem::moveWithAcc(Point acc){
	for (int i = 0; i < count; i++){
		particles[i].moveWithAcc(acc);
	}
}

void ParticleSystem::moveCircle(Point center, float dz, float radius){
	for (int i = 0; i < count; i++){

		float angle = (particles[i].x == 0) ? PI / 2 : atan(particles[i].z / particles[i].x);
		if (particles[i].z < 0)
			angle += PI;
		angle += PI / 180;
		Point p;
		p.y = particles[i].y + dz;
		//if (particles[i].x*particles[i].x + particles[i].z*particles[i].z <= radius*radius/1.5){
			radius = sqrt( particles[i].x*particles[i].x + particles[i].z*particles[i].z)+radius/300;
		//}
		
		p.x = radius*cos(angle)*(1 + rand() / (double)RAND_MAX / 100 - 0.005);
		p.z = radius*sin(angle)*(1 + rand() / (double)RAND_MAX / 100 - 0.005);

		particles[i].moveTo(p);
	}
}


void ParticleSystem::moveCircle0(Point center, float dz, float radius){
	for (int i = 0; i < count; i++){
		
		float angle = (particles[i].x == 0) ? PI / 2 : atan(particles[i].z / particles[i].x);
		if (particles[i].z < 0)
			angle += PI;
		angle += PI / 180;
		Point p;
		p.y = particles[i].y+ rand() / (double)RAND_MAX*dz*2 - dz;
		//if (particles[i].x*particles[i].x + particles[i].z*particles[i].z <= radius*radius/1.5){
		//radius = sqrt(particles[i].x*particles[i].x + particles[i].z*particles[i].z) + radius / 500;
		//}

		p.x = radius*cos(angle)*(1 + rand() / (double)RAND_MAX / 50 - 0.01);
		p.z = radius*sin(angle)*(1 + rand() / (double)RAND_MAX / 50 - 0.01);

		particles[i].moveTo(p);
		if (particles[i].show>=particles[i].lifetime)
		particles[i].show=0;
	}
}

void ParticleSystem::moveWithPCircle(int axis, float radius){
	for (int i = 0; i < count; i++){
		if (particles[i].show == 0){
			particles[i].ax = rand() / (double)RAND_MAX*PI * 2;
			particles[i].ay = rand() / (double)RAND_MAX*PI * 2;
			particles[i].az = rand() / (double)RAND_MAX*PI * 2;
		}
		particles[i].moveWithPCircle(axis, radius);
	}
}


void ParticleSystem::refreshDieP(int index){
	if (index < 0 || index >= particles.size())
		return;
	if (this->genmode == 1){
		particles[index] = initParticle(genPtr);
	}
	if (this->genmode == 2){
		int triind = rand()%genVec.size();
		particles[index] = initParticle(genVec[triind]);
	}
}