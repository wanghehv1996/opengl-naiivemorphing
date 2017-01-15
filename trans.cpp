#include "trans.h"
#include <math.h>
//#include<map>
extern float PI;
extern float transPi;


map<int, int> matchVec(vector<TriPoint>& tpvec1, vector<TriPoint>& tpvec2, pair<Point, Point> pair1, pair<Point, Point> pair2);

void GetVec(map<string, Object>& objmap, vector<TriPoint>& tpvec, vector<TriPoint>& spherevec, vector<TriPoint>&transvec, int & pace){
		//divide obj get their center set transvec
		vector <int>objvec;
		tpvec.clear();
		transvec.clear();

		for (map<string, Object>::iterator it = objmap.begin(); it != objmap.end(); ++it)
		{
			//cout << "getvec:obj name :" << it->first << endl;
			Object temp = it->second;
			vector<FacePoint>::iterator iter = temp.facepnts.begin();
			if (tpvec.size()!=0)
				objvec.push_back(tpvec.size());
			while (iter != temp.facepnts.end()){
				//cout << iter->pid <<"in"<< temp.points.size()<<"/"<<temp.norvecs.size() << endl;
				TriPoint tp;
				
				tp.x = temp.points[iter->pid - 1].x;
				tp.y = temp.points[iter->pid - 1].y;
				tp.z = temp.points[iter->pid - 1].z;
				if (iter->pid >= temp.norvecs.size()){
					tp.nx = 0;
					tp.ny = 0;
					tp.nz = 0;
				}
				else{
					tp.nx = temp.norvecs[iter->vnid - 1].x;
					tp.ny = temp.norvecs[iter->vnid - 1].y;
					tp.nz = temp.norvecs[iter->vnid - 1].z;
				}
				tpvec.push_back(tp);
				
				//ofs << "tpvec:\t" << iter->pid << "\t" << temp.points[iter->pid - 1].x << "\t" << temp.points[iter->pid - 1].y << "\t" << temp.points[iter->pid - 1].z  <<endl;
				iter++;
			}
			//cout << "getvec:obj " << it->first <<" finish"<< endl;
		}

		int facenum = tpvec.size() / 3;
		cout << "facenum" << facenum << endl;
		int latitude = sqrt(facenum);
		int longitude = 0;
		if (latitude % 2 == 0){
			longitude = latitude / 2;
			if (longitude == 0)
				longitude = 1;
			latitude = facenum / longitude / 2;
		}
		else{
			if (latitude == 0)
				longitude = 1;
			else
				longitude = facenum / latitude / 2;
		}
		//cout << "begin get sphere" << endl;
		GetSphere(latitude, longitude, facenum, tpvec, spherevec, transvec, pace);
}

void GetSphere(int latitude, int longitude, int facenum, vector<TriPoint>& tpvec, vector<TriPoint>& spherevec, vector<TriPoint>&transvec, int & pace){
	float center[3] = { 0, 0, 0 };
	center[0] = tpvec[0].x;
	center[1] = tpvec[0].y;
	center[2] = tpvec[0].z;
	float radius = 10;

	float latiAng = 180.0 / latitude;
	float longAng = 360.0 / longitude;
	int pos = 0;
	cout << "lati long" << latiAng << ' '<< longAng << endl;
	

	transvec.clear();
	spherevec.clear();
	TriPoint tp;
	bool notpvec = false;
	//cout << "begin get sphere loop" << endl;
		for (float i = 0; i < 360; i += longAng){
			for (float j = -90; j < 90; j += latiAng){
				//cout << "loop: i = " << i << "j = " << j << endl;
				//if (pos >= tpvec.size())
					//return;
				

				tp.x = cos(j*transPi)*cos(i*transPi)*radius + center[0];
				tp.y = cos(j*transPi)*sin(i*transPi)*radius + center[1];
				tp.z = sin(j*transPi)*radius + center[2];
				//ofs << "circlepos " << tp.x << "\t" << tp.y << "\t" << tp.z;
				tp.nx = tp.x - center[0];
				tp.ny = tp.y - center[1];
				tp.nz = tp.z - center[2];
				spherevec.push_back(tp);
				if (notpvec){
					tpvec.push_back(tpvec[0]);
					tp = (tp - tpvec[pos]);
				}
				else
					tp = (tp - tpvec[pos]);// pace;
				transvec.push_back(tp);
				//ofs << "  mid  "<< tp.nx << "\t" << tp.ny << "\t" << tp.nz << " tpvec " << tpvec[pos].nx << "\t" << tpvec[pos].ny << "\t" << tpvec[pos].nz << endl;

				pos++;
				//if (pos >= tpvec.size())
					//return;

				float jp = j + latiAng;
				float ip = i + longAng;

				tp.x = cos(j*transPi)*cos(ip*transPi)*radius + center[0];
				tp.y = cos(j*transPi)*sin(ip*transPi)*radius + center[1];
				tp.z = sin(j*transPi)*radius + center[2];
				//ofs << "circlepos " << tp.x << "\t" << tp.y << "\t" << tp.z;
				tp.nx = tp.x - center[0];
				tp.ny = tp.y - center[1];
				tp.nz = tp.z - center[2];
				spherevec.push_back(tp);
				if (notpvec){
					tpvec.push_back(tpvec[1]);
					tp = (tp - tpvec[pos]);
				}
				else
					tp = (tp - tpvec[pos]);// pace;
				transvec.push_back(tp);
				//ofs << "  mid  " << tp.nx << "\t" << tp.ny << "\t" << tp.nz << " tpvec " << tpvec[pos].nx << "\t" << tpvec[pos].ny << "\t" << tpvec[pos].nz << endl;

				pos++;
				//if (pos >= tpvec.size())
					//return;

				tp.x = cos(jp*transPi)*cos(ip*transPi)*radius + center[0];
				tp.y = cos(jp*transPi)*sin(ip*transPi)*radius + center[1];
				tp.z = sin(jp*transPi)*radius + center[2];
				//ofs << "circlepos " << tp.x << "\t" << tp.y << "\t" << tp.z;
				tp.nx = tp.x - center[0];
				tp.ny = tp.y - center[1];
				tp.nz = tp.z - center[2];
				spherevec.push_back(tp);
				if (notpvec){
					tpvec.push_back(tpvec[2]);
					tp = (tp - tpvec[pos]);
				}
				else
					tp = (tp - tpvec[pos]);// pace;
				transvec.push_back(tp);
				//ofs << "  mid  " << tp.nx << "\t" << tp.ny << "\t" << tp.nz << " tpvec " << tpvec[pos].nx << "\t" << tpvec[pos].ny << "\t" << tpvec[pos].nz << endl;

				pos++;
				
				if (pos >= tpvec.size())
					//return;
					notpvec = true;

				//second tri
				tp.x = cos(j*transPi)*cos(i*transPi)*radius + center[0];
				tp.y = cos(j*transPi)*sin(i*transPi)*radius + center[1];
				tp.z = sin(j*transPi)*radius + center[2];
				//ofs << "circlepos " << tp.x << "\t" << tp.y << "\t" << tp.z;
				tp.nx = tp.x - center[0];
				tp.ny = tp.y - center[1];
				tp.nz = tp.z - center[2];
				spherevec.push_back(tp);
				if (notpvec){
					tpvec.push_back(tpvec[0]);
					tp = (tp - tpvec[pos]);
				}else
					tp = (tp - tpvec[pos]);// pace;
				transvec.push_back(tp);
				//ofs << "  mid  " << tp.nx << "\t" << tp.ny << "\t" << tp.nz << " tpvec " << tpvec[pos].nx << "\t" << tpvec[pos].ny << "\t" << tpvec[pos].nz << endl;

				pos++;
				//if (pos >= tpvec.size())
					//return;

				tp.x = cos(jp*transPi)*cos(ip*transPi)*radius + center[0];
				tp.y = cos(jp*transPi)*sin(ip*transPi)*radius + center[1];
				tp.z = sin(jp*transPi)*radius + center[2];
				//ofs << "circlepos " << tp.x << "\t" << tp.y << "\t" << tp.z;
				tp.nx = tp.x - center[0];
				tp.ny = tp.y - center[1];
				tp.nz = tp.z - center[2];
				spherevec.push_back(tp);
				if (notpvec){
					tpvec.push_back(tpvec[1]);
					tp = (tp - tpvec[pos]);
				}
				else
					tp = (tp - tpvec[pos]);// pace;
				transvec.push_back(tp);
				//ofs << "  mid  " << tp.nx << "\t" << tp.ny << "\t" << tp.nz << " tpvec " << tpvec[pos].nx << "\t" << tpvec[pos].ny << "\t" << tpvec[pos].nz << endl;

				pos++;
				//if (pos >= tpvec.size())
					//return;

				tp.x = cos(jp*transPi)*cos(i*transPi)*radius + center[0];
				tp.y = cos(jp*transPi)*sin(i*transPi)*radius + center[1];
				tp.z = sin(jp*transPi)*radius + center[2];
				//ofs << "circlepos " << tp.x << "\t" << tp.y << "\t" << tp.z;
				tp.nx = tp.x - center[0];
				tp.ny = tp.y - center[1];
				tp.nz = tp.z - center[2];
				spherevec.push_back(tp);
				if (notpvec){
					tpvec.push_back(tpvec[2]);
					tp = (tp - tpvec[pos]);
				}
				else
					tp = (tp - tpvec[pos]);// pace;
				transvec.push_back(tp);
				//ofs << "  mid  " << tp.nx << "\t" << tp.ny << "\t" << tp.nz << " tpvec " << tpvec[pos].nx << "\t" << tpvec[pos].ny << "\t" << tpvec[pos].nz << endl;

				pos++;
			}
		}
		///cout << "after loop " << pos<<"  "<<tpvec.size()<<endl;
		while (pos < tpvec.size()){
			tp.x = radius + center[0];
			tp.y = center[1];
			tp.z = center[2];
			//ofs << "circlepos " << tp.x << "\t" << tp.y << "\t" << tp.z;
			tp.nx = tp.x - center[0];
			tp.ny = tp.y - center[1];
			tp.nz = tp.z - center[2];
			spherevec.push_back(tp);
			tp = (tp - tpvec[pos]);// pace;
			transvec.push_back(tp);
			pos++;
		}
	//ofs.close();
}




/* model->model
 * 
 *
 *
 */
void GetVec2(map<string, Object>& objmap, vector<TriPoint>& tpvec){
	vector <int>objvec;
	tpvec.clear();

	for (map<string, Object>::iterator it = objmap.begin(); it != objmap.end(); ++it)
	{
		//cout << "getvec:obj name :" << it->first << endl;
		Object temp = it->second;
		vector<FacePoint>::iterator iter = temp.facepnts.begin();
		if (tpvec.size() != 0)
			objvec.push_back(tpvec.size());
		while (iter != temp.facepnts.end()){
			//cout << iter->pid <<"in"<< temp.points.size()<<"/"<<temp.norvecs.size() << endl;
			TriPoint tp;

			tp.x = temp.points[iter->pid - 1].x;
			tp.y = temp.points[iter->pid - 1].y;
			tp.z = temp.points[iter->pid - 1].z;
			if (iter->pid >= temp.norvecs.size()){
				tp.nx = 0;
				tp.ny = 0;
				tp.nz = 0;
			}else{
				tp.nx = temp.norvecs[iter->vnid - 1].x;
				tp.ny = temp.norvecs[iter->vnid - 1].y;
				tp.nz = temp.norvecs[iter->vnid - 1].z;
			}
			tpvec.push_back(tp);

			//ofs << "tpvec:\t" << iter->pid << "\t" << temp.points[iter->pid - 1].x << "\t" << temp.points[iter->pid - 1].y << "\t" << temp.points[iter->pid - 1].z  <<endl;
			iter++;
		}
		//cout << "getvec:obj " << it->first <<" finish"<< endl;
	}
}

void GetTrans2(vector<TriPoint>& tpvec1, vector<TriPoint>& tpvec2, vector<TriPoint>& transvec,int pace){
	if (pace <= 10)
		cout << "ERROR : pace is too small !!" << endl;
	transvec.clear();
	vector<TriPoint>::iterator iter1 = tpvec1.begin();
	vector<TriPoint>::iterator iter2 = tpvec2.begin();

	for (; iter1 != tpvec1.end() && iter2 != tpvec2.end(); iter1++, iter2++)	{
			//cout << iter->pid <<"in"<< temp.points.size()<<"/"<<temp.norvecs.size() << endl;
			TriPoint tp;
			tp.x = (iter2->x - iter1->x) / pace;
			tp.y = (iter2->y - iter1->y) / pace;
			tp.z = (iter2->z - iter1->z) / pace;
			tp.nx = (iter2->nx - iter1->nx) / pace;
			tp.ny = (iter2->ny - iter1->ny) / pace;
			tp.nz = (iter2->nz - iter1->nz) / pace;
			transvec.push_back(tp);

			//ofs << "tpvec:\t" << iter->pid << "\t" << temp.points[iter->pid - 1].x << "\t" << temp.points[iter->pid - 1].y << "\t" << temp.points[iter->pid - 1].z  <<endl;
	}

	//vec1 is too short
	if (iter1 == tpvec1.end()){
		while (iter2 != tpvec2.end()){
			tpvec1.push_back(*tpvec1.begin());
			TriPoint tp;
			tp.x = (iter2->x - tpvec1.begin()->x) / pace;
			tp.y = (iter2->y - tpvec1.begin()->y) / pace;
			tp.z = (iter2->z - tpvec1.begin()->z) / pace;
			tp.nx = (iter2->nx - tpvec1.begin()->nx) / pace;
			tp.ny = (iter2->ny - tpvec1.begin()->ny) / pace;
			tp.nz = (iter2->nz - tpvec1.begin()->nz) / pace;
			transvec.push_back(tp);
			iter2++;
		}
	}
	//iter2 is too short
	else if (iter2 == tpvec2.end()){
		while (iter1 != tpvec1.end()){
			tpvec2.push_back(*tpvec2.begin());
			TriPoint tp;
			tp.x = (tpvec2.begin()->x - iter1->x) / pace;
			tp.y = (tpvec2.begin()->y - iter1->y) / pace;
			tp.z = (tpvec2.begin()->z - iter1->z) / pace;
			tp.nx = (tpvec2.begin()->nx - iter1->nx) / pace;
			tp.ny = (tpvec2.begin()->ny - iter1->ny) / pace;
			tp.nz = (tpvec2.begin()->nz - iter1->nz) / pace;
			transvec.push_back(tp);
			iter1++;
		}
	}

}

std::pair<Point,Point> getInsidePoint(vector<TriPoint>tpvec){
	if (tpvec.empty()){
		cout << "ERROR : getinsidepoint tpvec is empty" << endl;
		return make_pair(Point{ 0, 0, 0 }, Point{ 0, 0, 0 });
	}
	int size = tpvec.size();
	float minx = tpvec[0].x;
	float maxx = tpvec[0].x;
	float miny = tpvec[0].y;
	float maxy = tpvec[0].y;
	float minz = tpvec[0].z;
	float maxz = tpvec[0].z;
	for (int i = 1; i < size; i++){
		if (minx>tpvec[i].x)
			minx = tpvec[i].x;
		if (maxx < tpvec[i].x)
			maxx = tpvec[i].x;
		if (miny>tpvec[i].y)
			miny = tpvec[i].y;
		if (maxy < tpvec[i].y)
			maxy = tpvec[i].y;
		if (minz>tpvec[i].z)
			minz = tpvec[i].z;
		if (maxz < tpvec[i].z)
			maxz = tpvec[i].z;
	}
	Point p1 =  Point{ (minx + maxx) / 2, (miny + maxy) / 2, (minz + maxz) / 2 };
	Point p2 = Point{ (maxx - minx) , (maxy - miny) , (maxz - minz) };
	return make_pair(p1, p2);
}

void GetTrans3(vector<TriPoint>& tpvec1, vector<TriPoint>& tpvec2, vector<TriPoint>& transvec, int pace){
	if (pace <= 10)
		cout << "ERROR : pace is too small !!" << endl;
	float scale1, scale2;
	transvec.clear();
	int longer = 0;
	if (tpvec1.size() > tpvec2.size())
		longer = 1;
	else if (tpvec1.size() < tpvec2.size())
		longer = -1;
	cout << "OLD: tp1 " << tpvec1.size() << " tp2 " << tpvec2.size() << endl;
	transvec.clear();
	pair<Point, Point> pair1 = getInsidePoint(tpvec1);//first for center, second for scale
	pair<Point, Point> pair2 = getInsidePoint(tpvec2);
	map<int, int>matchmap;
	if (longer >= 0)
		matchmap = matchVec(tpvec1, tpvec2, pair1, pair2);
	else
		matchmap = matchVec(tpvec2, tpvec1, pair2, pair1);

	vector<int>newvec;
	int longersize = 0;
	if (longer >= 0)
		longersize = tpvec1.size();
	else
		longersize = tpvec2.size();
	cout << "NEW: tp1 " << tpvec1.size() << " tp2 " << tpvec2.size() << endl;
	cout << tpvec1.size() % 3 << " " << tpvec2.size() % 3 << endl;
	cout << matchmap.size() << endl;
	for (int i = 0; i < longersize / 3; i++){
		newvec.push_back(matchmap.find(i)->second);
		//cout << i << ' ' << matchmap.find(i)->second<<endl;
	}
	cout <<"newvecsize"<< newvec.size() << endl;

	if (longer >= 0){
		vector<TriPoint>newtrivec;
		for (int i = 0; i < newvec.size(); i++){
			newtrivec.push_back(tpvec2[3 * newvec[i] ]);
			newtrivec.push_back(tpvec2[3 * newvec[i] + 1]);
			newtrivec.push_back(tpvec2[3 * newvec[i] + 2]);
		}
		tpvec2 = newtrivec;
	}
	else{
		vector<TriPoint>newtrivec;
		for (int i = 0; i < newvec.size(); i++){
			newtrivec.push_back(tpvec1[3 * newvec[i] ]);
			newtrivec.push_back(tpvec1[3 * newvec[i] + 1]);
			newtrivec.push_back(tpvec1[3 * newvec[i] + 2]);
		}
		tpvec1 = newtrivec;
	}

	vector<TriPoint>::iterator iter1 = tpvec1.begin();
	vector<TriPoint>::iterator iter2 = tpvec2.begin();
	for (;iter1!=tpvec1.end()&&iter2!=tpvec2.end(); iter1++,iter2++){
		TriPoint tp;
		tp.x = (iter2->x - iter1->x) / pace;
		tp.y = (iter2->y - iter1->y) / pace;
		tp.z = (iter2->z - iter1->z) / pace;
		tp.nx = (iter2->nx - iter1->nx) / pace;
		tp.ny = (iter2->ny - iter1->ny) / pace;
		tp.nz = (iter2->nz - iter1->nz) / pace;
		transvec.push_back(tp);
	}

}

map<int, int> matchVec(vector<TriPoint>& tpvec1, vector<TriPoint>& tpvec2, pair<Point, Point> pair1, pair<Point, Point> pair2){
	vector<TriPoint>::iterator iter1 = tpvec1.begin();
	vector<TriPoint>::iterator iter2 = tpvec2.begin();

	vector<pair<float, float>>angvec1;	//xz for ang1(0 - 360) y for ang2(-90 - 90)
	vector<pair<float, float>>angvec2;

	vector<Point>disvec1;
	vector<Point>disvec2;

	Point center1 = pair1.first;
	for (; iter1 != tpvec1.end() && iter1 + 1 != tpvec1.end() && iter1 + 2 != tpvec1.end(); iter1 += 3){
		//calculate a face center angle
		float dx = iter1->x + (iter1 + 1)->x + (iter1 + 2)->x - 3 * center1.x;
		float dz = iter1->z + (iter1 + 1)->z + (iter1 + 2)->z - 3 * center1.z;
		float dy = iter1->y + (iter1 + 1)->y + (iter1 + 2)->y - 3 * center1.y;

		float sqrtxz = sqrt(dx*dx + dz*dz);
		float angho = asin(dz / sqrtxz);
		if (dx*dz < 0)
			angho += PI / 2;
		float angver = 0;
		if (sqrtxz == 0){
			if (dy>0)
				angver = PI / 2;
			else
				angver = -PI / 2;
		}
		else{
			angver = atan(dy / sqrtxz);
		}
		angvec1.push_back(make_pair(angho, angver));
	}

	Point center2 = pair2.first;
	for (; iter2 != tpvec2.end() && iter2 + 1 != tpvec2.end() && iter2 + 2 != tpvec2.end(); iter2 += 3){
		//calculate a face center angle
		float dx = iter2->x + (iter2 + 1)->x + (iter2 + 2)->x - 3 * center1.x;
		float dz = iter2->z + (iter2 + 1)->z + (iter2 + 2)->z - 3 * center1.z;
		float dy = iter2->y + (iter2 + 1)->y + (iter2 + 2)->y - 3 * center1.y;

		float sqrtxz = sqrt(dx*dx + dz*dz);
		float angho = asin(dz / sqrtxz);
		if (dx*dz < 0)
			angho += PI / 2;
		float angver = 0;
		if (sqrtxz == 0){
			if (dy>0)
				angver = PI / 2;
			else
				angver = -PI / 2;
		}
		else{
			angver = atan(dy / sqrtxz);
		}
		angvec2.push_back(make_pair(angho, angver));
	}


	cout <<"MATCH SIZE"<< tpvec1.size() << ' ' << tpvec2.size() << ' ' << angvec1.size() << ' ' << angvec2.size() << endl;
	map<int, int>matchmap;//index1 , index2
	map<int, int>checkmap;//index2 , cnt
	for (int i = 0; i < angvec1.size(); i++){
		pair<float, float>dang;
		dang.first = 10000;
		dang.second = 10000;
		int matchind = -1;
		int j = 0;
		for (; j < angvec2.size(); j++){
			if (checkmap.find(j) != checkmap.end() && checkmap.find(j)->second>angvec1.size() / angvec2.size())
				continue;
			if (abs(angvec2[j].first - angvec1[i].first) + abs(angvec2[j].second - angvec1[i].second) < (dang.first + dang.second)){
				dang.first = abs(angvec2[j].first - angvec1[i].first);
				dang.second = abs(angvec2[j].second - angvec1[i].second);
				matchind = j;
				//cout << angvec2[j].first - angvec1[i].first << ' ' << angvec2[j].second - angvec1[i].second << ' ' << dang.first << ' '<<dang.second << endl;
				//cout << i << ' ' << j << ' ' << matchind << endl;
			}
		}
		if (matchind>-1){
			matchmap.insert(make_pair(i, matchind));
			map<int, int>::iterator iter = checkmap.find(matchind);
			if (iter == checkmap.end())
				checkmap.insert(make_pair(matchind, 1));
			else
				iter->second++;
		}
		else{
			for (j = 0; j < angvec2.size(); j++){
				if (abs(angvec2[j].first - angvec1[i].first) < dang.first && abs(angvec2[j].second - angvec1[i].second) < dang.second){
					dang.first = abs(angvec2[j].first - angvec1[i].first);
					dang.second = abs(angvec2[j].second - angvec1[i].second);
					matchind = j;
				}
			}
			//cout << "else " << i << j << endl;
			matchmap.insert(make_pair(i, matchind));
			map<int, int>::iterator iter = checkmap.find(matchind);
			if (iter == checkmap.end())
				checkmap.insert(make_pair(matchind, 1));
			else
				iter->second++;
		}
	}
	return matchmap;
}


void GetTrans4(vector<TriPoint>& tpvec1, vector<TriPoint>& tpvec2, vector<TriPoint>& transvec1, vector<TriPoint>& transvec2, int pace){
	pair<Point, Point> pair1 = getInsidePoint(tpvec1);//first for center, second for scale
	pair<Point, Point> pair2 = getInsidePoint(tpvec2);
	transvec1.clear();
	transvec2.clear();
	Point center1 = pair1.first;
	Point center2 = pair2.first;
	float radius = (pair1.second.x + pair1.second.y + pair1.second.z + pair2.second.x + pair2.second.y + pair2.second.z) / 6;
	Point midcenter = Point{ (center1.x + center2.x) / 2, (center1.y + center2.y) / 2, (center1.z + center2.z) / 2 };
	int midpace = pace / 2;

	for (int i = 0; i < tpvec1.size(); i++){
		float disx = tpvec1[i].x - center1.x;
		float disy = tpvec1[i].y - center1.y;
		float disz = tpvec1[i].z - center1.z;
		float nr = sqrt(disx*disx + disy*disy + disz*disz);
		TriPoint tp;
		tp.x = (radius / nr - 1)*disx;
		tp.y = (radius / nr - 1)*disy;
		tp.z = (radius / nr - 1)*disz;
		tp.x += (midcenter.x - center1.x);
		tp.y += (midcenter.y - center1.y);
		tp.z += (midcenter.z - center1.z);
		tp.nx = disx / nr -tpvec1[i].nx;
		tp.ny = disy / nr - tpvec1[i].ny;
		tp.nz = disz / nr - tpvec1[i].nz;
		tp.x /= midpace, tp.y /= midpace, tp.z /= midpace; 		
		tp.nx /= midpace, tp.ny /= midpace, tp.nz /= midpace;
		transvec1.push_back(tp);
	}

	for (int i = 0; i < tpvec2.size(); i++){
		float disx = tpvec2[i].x - center2.x;
		float disy = tpvec2[i].y - center2.y;
		float disz = tpvec2[i].z - center2.z;
		float nr = sqrt(disx*disx + disy*disy + disz*disz);
		TriPoint tp;
		tp.x = (radius / nr - 1)*disx;
		tp.y = (radius / nr - 1)*disy;
		tp.z = (radius / nr - 1)*disz;
		tp.x += (midcenter.x - center2.x);
		tp.y += (midcenter.y - center2.y);
		tp.z += (midcenter.z - center2.z);
		tp.nx = disx / nr - tpvec2[i].nx;
		tp.ny = disy / nr - tpvec2[i].ny;
		tp.nz = disz / nr - tpvec2[i].nz;
		tp.x /= midpace, tp.y /= midpace, tp.z /= midpace;
		tp.nx /= midpace, tp.ny /= midpace, tp.nz /= midpace;
		transvec2.push_back(tp);
	}

}

void GetTrans4Mid2(vector<TriPoint>& tpvec2, vector<TriPoint>& transvec2, int pace){
	int midpace = pace / 2;
	for (int i = 0; i < tpvec2.size(); i++){
		tpvec2[i].x += (transvec2[i].x*midpace);
		tpvec2[i].y += (transvec2[i].y*midpace);
		tpvec2[i].z += (transvec2[i].z*midpace);
		tpvec2[i].nx += (transvec2[i].nx*midpace);
		tpvec2[i].ny += (transvec2[i].ny*midpace);
		tpvec2[i].nz += (transvec2[i].nz*midpace);
	}
}

void GetTrans4Mid1(vector<TriPoint>& tpvec1, vector<TriPoint>& transvec1, int pace){
	int midpace = pace / 2;
	for (int i = 0; i < tpvec1.size(); i++){
		tpvec1[i].x += (transvec1[i].x*midpace);
		tpvec1[i].y += (transvec1[i].y*midpace);
		tpvec1[i].z += (transvec1[i].z*midpace);
		tpvec1[i].nx += (transvec1[i].nx*midpace);
		tpvec1[i].ny += (transvec1[i].ny*midpace);
		tpvec1[i].nz += (transvec1[i].nz*midpace);
	}
}