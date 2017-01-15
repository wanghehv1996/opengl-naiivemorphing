#ifndef _READ_H_
#define _READ_H_

#include "model.h"
#include <iostream>
#include "tgaload.h"

GLuint readTexture(string pathname);
GLuint readTexturetga(string pathname);
void readObj(string pathname, map<string, Object> &objMap, map<string, Material> &mtlMap, int texid = 0);
void readMtl(string pathname, map<string, Material> &mtlMap, int texid = 0);

#endif