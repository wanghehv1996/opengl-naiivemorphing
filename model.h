#ifndef _MODEL_H_
#define _MODEL_H_
#pragma once
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include <gl/glut.h>

using namespace std;

static float PI = 3.1415926535;
static float transPi = PI / 180;

typedef struct{
	float x;
	float y;
	float z;
}Point;

//class TexCoord {
//public:
typedef struct{
	float x;
	float y;
}TexCoord;

//class FacePoint {
//public:
typedef struct{
	int pid;
	int tcid;
	int vnid;
}FacePoint;

struct TriPoint{
	float x;
	float y;
	float z;
	float nx;
	float ny;
	float nz;
	TriPoint(Point p,Point np):x(p.x),y(p.y),z(p.z),nx(np.x),ny(np.y),nz(np.z){}
	TriPoint(){}
};
TriPoint operator+(TriPoint &a1, TriPoint &a2);
TriPoint operator-(TriPoint &a1, TriPoint &a2);
TriPoint operator / (TriPoint &a1, int &a2);


//class Object {
//public:
typedef struct{
	std::vector<Point> points;
	std::vector<TexCoord> texcoords;
	std::vector<Point> norvecs;
	std::vector<FacePoint> facepnts;
	std::vector<TriPoint>tpvec;
	std::string material;
	int pol;
}Object;

class Material {

public:
	Material(float *a, float *d, float *s, float *e, unsigned id)
	{
		ambient[0] = *a;
		ambient[1] = *(a + 1);
		ambient[2] = *(a + 2);
		ambient[3] = 0;
		diffuse[0] = *d;
		diffuse[1] = *(d + 1);
		diffuse[2] = *(d + 2);
		diffuse[3] = 0;
		specular[0] = *s;
		specular[1] = *(s + 1);
		specular[2] = *(s + 2);
		specular[3] = 0;
		emission[0] = *e;
		emission[1] = *(e + 1);
		emission[2] = *(e + 2);
		emission[3] = 0;
		ambientId = id;
		diffuseId = id;
		specularId = id;
		emissionId = id;
	}
	Material()
	{
		ambient[0] = 0;
		ambient[1] = 0;
		ambient[2] = 0;
		ambient[3] = 0;
		diffuse[0] = 1.0;
		diffuse[1] = 1.0;
		diffuse[2] = 1.0;
		diffuse[3] = 0;
		specular[0] = 1.0;
		specular[1] = 1.0;
		specular[2] = 1.0;
		specular[3] = 0;
		emission[0] = 0;
		emission[1] = 0;
		emission[2] = 0;
		emission[3] = 0;
		ambientId = 0;
		diffuseId = 0;
		specularId = 0;
		emissionId = 0;
	}
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float emission[4];
	unsigned ambientId;
	unsigned diffuseId;
	unsigned specularId;
	unsigned emissionId;
};

#endif