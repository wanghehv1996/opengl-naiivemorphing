#include "model.h"
TriPoint operator-(TriPoint &a1, TriPoint &a2)
{
	TriPoint tp;
	tp.x = a1.x - a2.x;
	tp.y = a1.y - a2.y;
	tp.z = a1.z - a2.z;
	tp.nx = a1.nx - a2.nx;
	tp.ny = a1.ny - a2.ny;
	tp.nz = a1.nz - a2.nz;
	return tp;
}

TriPoint operator+(TriPoint &a1, TriPoint &a2)
{
	TriPoint tp;
	tp.x = a1.x + a2.x;
	tp.y = a1.y + a2.y;
	tp.z = a1.z + a2.z;
	tp.nx = a1.nx + a2.nx;
	tp.ny = a1.ny + a2.ny;
	tp.nz = a1.nz + a2.nz;
	return tp;
}

TriPoint operator/(TriPoint &a1, int &a2)
{
	TriPoint tp;
	tp.x = a1.x / a2;
	tp.y = a1.y / a2;
	tp.z = a1.z / a2;
	if (a2 < 0){
		tp.nx = -a1.nx;
		tp.ny = -a1.ny;
		tp.nz = -a1.nz;
	}
	return tp;
}
