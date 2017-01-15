#ifndef _TRANS_H_
#define _TRANS_H_

#include "model.h"
#include <iostream>

void GetVec(map<string, Object>& objmap, vector<TriPoint>& tpvec, vector<TriPoint>& spherevec, vector<TriPoint>&transvec, int & pace);
void GetSphere(int latitude, int longitude, int facenum, vector<TriPoint>& tpvec, vector<TriPoint>& spherevec, vector<TriPoint>&transvec, int & pace);
void GetVec2(map<string, Object>& objmap, vector<TriPoint>& tpvec);
void GetTrans2(vector<TriPoint>& tpvec1, vector<TriPoint>& tpvec2, vector<TriPoint>& transvec, int pace);
void GetTrans3(vector<TriPoint>& tpvec1, vector<TriPoint>& tpvec2, vector<TriPoint>& transvec, int pace);
void GetTrans4(vector<TriPoint>& tpvec1, vector<TriPoint>& tpvec2, vector<TriPoint>& transvec1, vector<TriPoint>& transvec2, int pace);
void GetTrans4Mid2(vector<TriPoint>& tpvec2, vector<TriPoint>& transvec2, int pace);
std::pair<Point, Point> getInsidePoint(vector<TriPoint>tpvec);
#endif