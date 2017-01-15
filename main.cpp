#include <windows.h>
#include <string>
#include <map>
#include <gl/glut.h>
#include "read.h"
#include "trans.h"
#include "psystem.h"

//string obj1 = ".\\cat\\cat.obj";
string obj2 = ".\\dog\\dog.obj";
string obj1 = ".\\f16\\f16.obj";
//string obj1 = ".\\bunny\\bunny.obj";
//string obj1 = ".\\Rabbit\\Rabbit.obj";


int materialMark = 0;
void displayFunc();
int particlemode = 0;

//huangjingyanse 
GLfloat aMaterial1[4] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat aMaterial2[4] = { 0.64, 0.64, 0.64, 1.0 };
GLfloat aMaterial3[4] = { 0.8, 0.8, 0.8, 1.0 };

GLfloat dMaterial1[4] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat dMaterial2[4] = { 0.64, 0.64, 0.64, 1.0 };
GLfloat dMaterial3[4] = { 1.0, 1.0, 1.0, 1.0 };

//gaoguang zhengtiguang
GLfloat sMaterial1[4] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat sMaterial2[4] = { 0, 0, 0, 1.0 };
GLfloat sMaterial3[4] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat shinMaterial1 = 10;
GLfloat shinMaterial2 = 70;
GLfloat shinMaterial3 = 128;

 GLfloat s_eye[] = { 0, 0, 8.0 };
 GLfloat s_at[] = { 0.0, 0.0, 0.0 };
 GLfloat s_angle = -90.0;
 GLfloat s_anglez = 0.0;
 float rad = 0;
 float radz = 0;

map<string, Object> objmap;//read from file and get objects
map<string, Object> objmap1, objmap2;
map<string, Material> matname;//read from file and get materials
map<string, Material>matname1, matname2;
vector<TriPoint>tpvec;//object points without texture
vector<TriPoint>tpvec1,tpvec2;
pair<Point, Point>tpcenpair1,tpcenpair2;
vector<TriPoint>transvec,transvec2;//trans points when deformation
vector<TriPoint>spherevec;//sphere points
vector<TriPoint>tmpvec;//Temporary points when deformation
ParticleSystem psystem = ParticleSystem();
ParticleSystem psystemCircle = ParticleSystem();
ParticleSystem psystemCirclex = ParticleSystem();
ParticleSystem psystemCircley = ParticleSystem();
ParticleSystem psystemCirclez = ParticleSystem();

void moveDisplay(int value);//recursive function for deformation
pair<Point,Point> getInsidePoint(vector<TriPoint>tpvec);
bool ShowPsys(int mode);

GLfloat tranArr[3] = { 0.0, 0.0, 0.0 };
GLfloat rotArr[3] = { 0.0, 0.0, 0.0 };//[ymove,xmove

GLint mx = 0, my = 0;

//for reshape
GLfloat aspect = 1;

//for deformation
bool change = false;
bool pause = true;
int changepace = 0;
int pace = 100;
bool changedir = true;

bool showpsys = false;
bool circlepsys = false;

//for mouse func
bool click = 0;

void PrintInstruction(){
	std::cout << "File Completed!" << endl << endl;

	std::cout << "---------- ObjLoader Instruction ----------" << endl << endl;

	std::cout << "These loader will read the file \".\\tank\\tank.obj\" and display it" << endl
		<< "You can use keyboard and mouse:" << endl << endl
		<< "Keyboad:" << endl
		<< "\t\"w,s,a,d\" used for moving" << endl 
		<< "\t\"m\" used for deformation" << endl
		<< "\t\"r\" used for displaying first model" << endl 
		<< "\t\"b\" used for displaying second model" << endl
		<< "\t\"p\" used for start/stop particle" << endl
		<< "\t\"o\" used for open circle particles"<<endl
		<< "\t\"1,2,3\" used for change the mtl&tex (only for cat.cat2.f16)" << endl
		<< "\t\"4,5,6\" used for change the material" << endl
		<< "\t\"7,8,9\" used for choose different particle special effect" << endl
		<< "Mouse:\n\t mouse used for changing the viewpoint " << endl;
	std::cout << "Have fun . . . " << endl << endl;
	std::cout << "--------------------------------------------" << endl << endl;
}


void idleFunc()
{
	Sleep(10);
	glutPostRedisplay();
}

void reshapeFunc(int width, int height)
{
	if (height == 0)
		height = 1;
	aspect = (float)width / height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(75, aspect, 1, 10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
}

void keyboardFunc(unsigned char key, int x, int y)
{
	cout << key;
	switch (key)
	{
	case 'w':
		tranArr[2] += 0.5f;
		break;
	case 's':
		tranArr[2] -= 0.5f;
		break;
	case 'd':
		tranArr[0] += 0.5f;
		break;
	case 'a':
		tranArr[0] -= 0.5f;
		break;
	case'm':
		if (!pause){
			std::cout << "\tButton m: \tstop deformation" << endl;
			change = true;
			pause = true;
		}
		else{
			std::cout << "\tButton m: \tbegin deformation" << endl;
			change = true;
			pause = false;
			moveDisplay(0);
		}
		break;
	case 'r'://for 
		change = false;
		pause = true;
		changepace = 0;
		changedir = true;
		std::cout << "\tButton r: \tdisplay the original model" << endl;
		readObj(obj1, objmap1, matname1);
		tmpvec = tpvec1;
		glutPostRedisplay();
		break;
	case'b':
		change = false;
		pause = true;
		changepace = 100;
		changedir = false;
		std::cout << "\tButton b: \tdisplay the sphere model" << endl;
		readObj(obj2, objmap2, matname2);
		tmpvec = tpvec2;
		glutPostRedisplay();
		break;
	//case'n':{
	//			string filename;
	//			ifstream ifs;
	//			std::cout << "Input new obj file name(e for stop):" << endl;
	//			std::cin >> filename;
	//			if (filename == "e"){
	//				std::cout << endl << "Your command:" << endl;
	//				break;
	//			}
	//			ifs.open(filename.c_str());
	//			while (!ifs){
	//				std::cout << endl << "Wrong file path! Try again\nInput new obj file name(e for stop):" << endl;
	//				std::cin >> filename;
	//				if (filename == "e")
	//					break;
	//				ifs.open(filename.c_str());
	//			}
	//			if (filename == "e"){
	//				std::cout << endl << "Your command:" << endl;
	//				break;
	//			}
	//			ifs.close();
	//			std::cout << endl;
	//			std::cout << "File Loading..." << endl;
	//			readObj(filename.c_str(), objmap, matname);
	//			//GetVec(objmap, tpvec, spherevec, transvec, pace);
	//			GetVec2(objmap1, tpvec1);
	//			GetTrans2(tpvec1, tpvec2, transvec, pace);
	//			//tmpvec = tpvec;
	//			tmpvec = tpvec1;
	//			change = false;
	//			pause = true;
	//			changepace = 0;
	//			changedir = true;
	//			PrintInstruction();
	//			std::cout << "Your command:" << endl;
	//			break;
	//}
	case'p':{
				cout << "p for particles" << endl;
				if (!showpsys)
					psystem = ParticleSystem(tpvec2, 1000);
				showpsys = !showpsys;
				displayFunc();
				break;
	}
	case'o':{
				cout << "o for circle particles" << endl;
				if (!circlepsys){
					psystemCircle = ParticleSystem(Point{ 0, 0, 0 }, 200);
					psystemCirclex = ParticleSystem(Point{ 0, 0, 0 }, 300);
					psystemCircley = ParticleSystem(Point{ 0, 0, 0 }, 300);
					psystemCirclez = ParticleSystem(Point{ 0, 0, 0 }, 300);
				}
				circlepsys = !circlepsys;
				displayFunc();
				break;
	}
	case '1':
		change = false;
		pause = true;
		changepace = 0;
		changedir = true;
		materialMark = 0;
		std::cout << "\tButton 1: \ttex 1" << endl;
		readObj(obj1, objmap1, matname1,1);
		GetVec2(objmap1, tpvec1);
		tpcenpair1 = getInsidePoint(tpvec1);
		GetTrans4(tpvec1, tpvec2, transvec, transvec2, pace);
		tmpvec = tpvec1;
		displayFunc();
		break;
	case '2':
		change = false;
		pause = true;
		changepace = 0;
		changedir = true;
		materialMark = 0;
		std::cout << "\tButton 2: \ttex 2" << endl;
		readObj(obj1, objmap1, matname1, 2);
		GetVec2(objmap1, tpvec1);
		tpcenpair1 = getInsidePoint(tpvec1);
		GetTrans4(tpvec1, tpvec2, transvec, transvec2, pace);
		tmpvec = tpvec1;
		displayFunc();
		break;
	case '3':
		change = false;
		pause = true;
		changepace = 0;
		changedir = true;
		materialMark = 0;
		std::cout << "\tButton 3: \ttex 0" << endl;
		readObj(obj1, objmap1, matname1, 0);
		GetVec2(objmap1, tpvec1);
		tpcenpair1 = getInsidePoint(tpvec1);
		GetTrans4(tpvec1, tpvec2, transvec, transvec2, pace);
		tmpvec = tpvec1;
		displayFunc();
		break;
	case '4':
		change = false;
		pause = true;
		changepace = 0;
		changedir = true;
		materialMark = 1;
		std::cout << "\tButton 4: \tmaterial 1" << endl;
		tmpvec = tpvec1;
		displayFunc();
		break;
	case'5':
		change = false;
		pause = true;
		changepace = 0;
		changedir= true;
		materialMark = 2;
		std::cout << "\tButton 5: \tmaterial 2" << endl;
		tmpvec = tpvec1;
		displayFunc();
		break;
	case'6':
		change = false;
		pause = true;
		changepace = 0;
		changedir = true;
		materialMark = 3;
		std::cout << "\tButton 6: \tmaterial 3" << endl;
		tmpvec = tpvec1;
		displayFunc();
		break;
	case'7':
		particlemode = 0;
		break;
	case'8':
		particlemode = 1;
		break;
	case'9':
		particlemode = 2;
		break;
	case'0':
		particlemode = 3;
		break;
	}
}

void moveDisplay(int value){
	if (change){
		if (!pause){
			glutPostRedisplay();
			glutTimerFunc(1000, moveDisplay, value);
		}
	}
}

void mouseFunc(int button, int state, int x, int y)
{
	if (button == GLUT_DOWN) {
		mx = x;
		my = y;
		click = true; 
	}
	else
		click = false;
}

void motionFunc(int x, int y)
{
	if (click)	{
		rotArr[0] += (y - my) / 5.0f;
		rotArr[1] += (x - mx) / 5.0f;
		mx = x;
		my = y;
	}
}

void setLight()
{
	static const GLfloat light_position[] = { -50.0f, 50.0f, 50.0f, 0.0f };
	static const GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f }; 
	static const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glEnable(GL_DEPTH_TEST);
}

void setMaterial(Material mat)
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat.specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat.emission);
}

void displayFunc()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glTranslatef(tranArr[0], tranArr[1], tranArr[2]);//向xyz轴平移
	glRotatef(rotArr[0], 1.0f, 0.0f, 0.0f);//沿方向向量(1,0,0)顺时针旋转rotarr度。
	glRotatef(rotArr[1], 0.0f, 1.0f, 0.0f);

	//gluLookAt(s_eye[0], s_eye[1], s_eye[2],		s_at[0], s_at[1], s_at[2],		0.0, 1.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	setLight();

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	//glColor4f(168.0 / 255, 216.0 / 255, 234.0 / 255, 1);
	//glPushMatrix();
	//glTranslatef(5, 5, 5);
	////gluSphere(mySphere, iter->size, 32, 16);
	//glutSolidSphere(2, 8, 8);
	//glPopMatrix();

	if (showpsys){
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		if (ShowPsys(particlemode) == false){
			showpsys = false;
			cout << "finish particle" << endl;
		}
	}
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glDisable(GL_COLOR_MATERIAL);
	if (change == false){
		if (changedir == true){
			for (map<string, Object>::iterator it = objmap1.begin(); it != objmap1.end(); ++it)
			{
				Object temp = it->second;
				bool textureBind = false;
				if (materialMark == 0||materialMark){
					if (matname1[temp.material].ambientId != 0){
						glBindTexture(GL_TEXTURE_2D, matname1[temp.material].ambientId);
						textureBind = true;
					}
					else if (matname1[temp.material].diffuseId != 0){
						glBindTexture(GL_TEXTURE_2D, matname1[temp.material].diffuseId);
						textureBind = true;
					}
				}
				

				if (materialMark == 0 && !matname1.empty()){
					setMaterial(matname1[temp.material]);
					glMateriali(GL_FRONT, GL_SHININESS, 100);//镜面指数（取值范围为[0.0, 128.0],数值越大,高光越小,亮度越高）
				}
				else if (materialMark == 1 || (materialMark == 0 && !matname1.empty())){
					glMaterialfv(GL_FRONT, GL_AMBIENT, aMaterial1);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, dMaterial1);
					glMaterialfv(GL_FRONT, GL_SPECULAR, sMaterial1);
					glMateriali(GL_FRONT, GL_SHININESS, shinMaterial1);
				} else if (materialMark == 2){
					glMaterialfv(GL_FRONT, GL_AMBIENT, aMaterial2);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, dMaterial2);
					glMaterialfv(GL_FRONT, GL_SPECULAR, sMaterial2);
					glMateriali(GL_FRONT, GL_SHININESS, shinMaterial2);
				}
				else if (materialMark == 3){
					glMaterialfv(GL_FRONT, GL_AMBIENT, aMaterial3);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, dMaterial3);
					glMaterialfv(GL_FRONT, GL_SPECULAR, sMaterial3);
					glMateriali(GL_FRONT, GL_SHININESS, shinMaterial3);
				}

				//setLight();
				glEnable(GL_TEXTURE_2D);
				if (!textureBind){
					glDisable(GL_TEXTURE_2D);
					glEnable(GL_COLOR_MATERIAL);
					glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
				}
				if (temp.pol == 3) glBegin(GL_TRIANGLES);
				else glBegin(GL_QUADS);
				vector<FacePoint>::iterator iter = temp.facepnts.begin();
				//cout << "objmap1 objsize" << temp.facepnts.size()<<endl;
				while (iter != temp.facepnts.end())
				{
					if (iter->vnid != 0&&iter->vnid<=temp.norvecs.size())
						glNormal3f(temp.norvecs[iter->vnid - 1].x, temp.norvecs[iter->vnid - 1].y, temp.norvecs[iter->vnid - 1].z);
					//cout << iter->tcid << endl;
					if (iter->tcid != 0&&iter->tcid<=temp.texcoords.size())
						glTexCoord2f(temp.texcoords[iter->tcid - 1].x, temp.texcoords[iter->tcid - 1].y);
					if (iter->pid != 0&&iter->pid<=temp.points.size())
						glVertex3f(temp.points[iter->pid - 1].x, temp.points[iter->pid - 1].y, temp.points[iter->pid - 1].z);
					++iter;
				}
				glEnd();
			}


		}//if change != true change2sphere !=true
		else{
			for (map<string, Object>::iterator it = objmap2.begin(); it != objmap2.end(); ++it)
			{
				Object temp = it->second;
				bool textureBind = false;

				if (!matname2.empty()){
					if (matname2[temp.material].ambientId != 0){
						glBindTexture(GL_TEXTURE_2D, matname2[temp.material].ambientId);
						textureBind = true;
					}
					else if (matname2[temp.material].diffuseId != 0){
						glBindTexture(GL_TEXTURE_2D, matname2[temp.material].diffuseId);
						textureBind = true;
					}
				}

				if (materialMark == 0&&!matname2.empty()){
					setMaterial(matname2[temp.material]);
					glMateriali(GL_FRONT, GL_SHININESS, 100);//镜面指数（取值范围为[0.0, 128.0],数值越大,高光越小,亮度越高）
				}
				else if (materialMark == 1 || (materialMark == 0 && !matname2.empty())){
					glMaterialfv(GL_FRONT, GL_AMBIENT, aMaterial1);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, dMaterial1);
					glMaterialfv(GL_FRONT, GL_SPECULAR, sMaterial1);
					glMateriali(GL_FRONT, GL_SHININESS, shinMaterial1);
				}
				else if (materialMark == 2){
					glMaterialfv(GL_FRONT, GL_AMBIENT, aMaterial2);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, dMaterial2);
					glMaterialfv(GL_FRONT, GL_SPECULAR, sMaterial2);
					glMateriali(GL_FRONT, GL_SHININESS, shinMaterial2);
				}
				else if (materialMark == 3){
					glMaterialfv(GL_FRONT, GL_AMBIENT, aMaterial3);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, dMaterial3);
					glMaterialfv(GL_FRONT, GL_SPECULAR, sMaterial3);
					glMateriali(GL_FRONT, GL_SHININESS, shinMaterial3);
				}


				//setLight();
				glEnable(GL_TEXTURE_2D);
				if (!textureBind){
					glDisable(GL_TEXTURE_2D);
					glEnable(GL_COLOR_MATERIAL);
					glColor4f(0.7f,0.7f,0.7f,1.0f);
				}
				if (temp.pol == 3) glBegin(GL_TRIANGLES);
				else glBegin(GL_QUADS);
				vector<FacePoint>::iterator iter = temp.facepnts.begin();
				while (iter != temp.facepnts.end())
				{
					if (iter->vnid != 0 && iter->vnid<=temp.norvecs.size())
						glNormal3f(temp.norvecs[iter->vnid - 1].x, temp.norvecs[iter->vnid - 1].y, temp.norvecs[iter->vnid - 1].z);
					if (iter->tcid != 0 && iter->tcid<=temp.texcoords.size())
						glTexCoord2f(temp.texcoords[iter->tcid - 1].x, temp.texcoords[iter->tcid - 1].y);
					if (iter->pid != 0 && iter->pid <= temp.points.size())
						glVertex3f(temp.points[iter->pid - 1].x, temp.points[iter->pid - 1].y, temp.points[iter->pid - 1].z);
					++iter;
				}
				glEnd();
			}

		}
	}//if change = true
	else{
		//cout << "change t" << endl;
		int i = 0;
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		glColor3f(0.0f, 0.0f, 1.0f);
		glBegin(GL_TRIANGLES);
		//glBegin(GL_POINTS);
		for (int i = 0; i<tmpvec.size();i++)
		{
			glNormal3f(tmpvec[i].nx, tmpvec[i].ny, tmpvec[i].nz);
			glVertex3f(tmpvec[i].x, tmpvec[i].y, tmpvec[i].z);
			
			if (!pause){
				if (changedir){
					//add
					if (changepace < pace / 2){
						tmpvec[i].x = tmpvec[i].x + transvec[i].x;
						tmpvec[i].y = tmpvec[i].y + transvec[i].y;
						tmpvec[i].z = tmpvec[i].z + transvec[i].z;
						tmpvec[i].nx = tmpvec[i].nx + transvec[i].nx;
						tmpvec[i].ny = tmpvec[i].ny + transvec[i].ny;
						tmpvec[i].nz = tmpvec[i].nz + transvec[i].nz;
					}

					else{
						tmpvec[i].x = tmpvec[i].x - transvec2[i].x;
						tmpvec[i].y = tmpvec[i].y - transvec2[i].y;
						tmpvec[i].z = tmpvec[i].z - transvec2[i].z;
						tmpvec[i].nx = tmpvec[i].nx - transvec2[i].nx;
						tmpvec[i].ny = tmpvec[i].ny - transvec2[i].ny;
						tmpvec[i].nz = tmpvec[i].nz - transvec2[i].nz;
					}
				}
				else {
					
					if (changepace > pace / 2){
						tmpvec[i].x = tmpvec[i].x + transvec2[i].x;
						tmpvec[i].y = tmpvec[i].y + transvec2[i].y;
						tmpvec[i].z = tmpvec[i].z + transvec2[i].z;
						tmpvec[i].nx = tmpvec[i].nx + transvec2[i].nx;
						tmpvec[i].ny = tmpvec[i].ny + transvec2[i].ny;
						tmpvec[i].nz = tmpvec[i].nz + transvec2[i].nz;
					}else{
						tmpvec[i].x = tmpvec[i].x - transvec[i].x;
						tmpvec[i].y = tmpvec[i].y - transvec[i].y;
						tmpvec[i].z = tmpvec[i].z - transvec[i].z;
						tmpvec[i].nx = tmpvec[i].nx - transvec[i].nx;
						tmpvec[i].ny = tmpvec[i].ny - transvec[i].ny;
						tmpvec[i].nz = tmpvec[i].nz - transvec[i].nz;
					}
				}
			}
		}

		//printf("%d %d",changepace,pace);
		if (!pause){
			if (changedir){
				changepace++;
				if (changepace == pace / 2){
					tmpvec = tpvec2;
					GetTrans4Mid2(tmpvec, transvec2, pace);
				}
			}
			else {
				changepace--;
				if (changepace == pace / 2){
					tmpvec = tpvec1;
					GetTrans4Mid2(tmpvec, transvec, pace);
					//cout << "mid status " << endl;
				}
			}
		}

		if (changepace >= pace){
			changedir = false;
			change = false;
			pause = true;
			psystem = ParticleSystem(tpvec2, 1000);
			tmpvec = tpvec2;
			showpsys = true;
		}
		else if (changepace <= 0){
			changedir = true;
			change = false;
			pause = true;
			psystem = ParticleSystem(tpvec1, 1000);
			tmpvec = tpvec1;
			showpsys = true;
		}

		glEnd();
	}
	glPopMatrix();
	glutSwapBuffers();
}



int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(320, 180);
	glutInitWindowSize(720, 360);
	glutCreateWindow("OpenGl");
	glutReshapeFunc(&reshapeFunc);
	glutKeyboardFunc(&keyboardFunc);
	glutMouseFunc(&mouseFunc);
	glutMotionFunc(&motionFunc);
	glutDisplayFunc(&displayFunc);
	glutIdleFunc(&idleFunc);
	std::cout << "---------- ObjLoader Begin ----------" << endl<<endl;



	
	//string filename;
	//ifstream ifs;
	//std::cout << "Input obj file name:" << endl;
	////std::cin >> filename;
	//filename = ".\\tank\\tank.obj";
	//ifs.open(filename.c_str());
	//while (!ifs){
	//	std::cout << "Wrong file path! Try again\nInput obj file name:" << endl;
	//	std::cin >> filename;
	//	ifs.open(filename.c_str());
	//}
	//ifs.close();
	//std::cout << endl;
	//std::cout << "File Loading..." << endl;
	//readObj(filename.c_str(), objmap, matname);

	//new


	readObj(obj2, objmap2, matname2);
	GetVec2(objmap2, tpvec2);
	readObj(obj1, objmap1, matname1);
	GetVec2(objmap1, tpvec1);
	tpcenpair1 = getInsidePoint(tpvec1);
	tpcenpair2 = getInsidePoint(tpvec2);
	//GetTrans2(tpvec1, tpvec2, transvec, pace);
	//GetTrans3(tpvec1, tpvec2, transvec, pace);
	GetTrans4(tpvec1, tpvec2, transvec,transvec2, pace);
	tmpvec = tpvec1;
	//new

	//GetVec(objmap, tpvec, spherevec, transvec, pace);
	//tmpvec = tpvec;

	PrintInstruction();
	std::cout << "Your command:" << endl;

	//glClearColor(0.9, 0.9, 0.9, 1.0);
	glClearColor(0.2, 0.2, 0.2, 1.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glutMainLoop();
	return EXIT_SUCCESS;
}


float particleEmission[4] = { 1.0, 1.0, 1.0, 1 };
Point wind = { 0, 0, 0 };
Point acc = { 0, 0, 0 };
int angle = 0;
int angle0 = 0;
bool ShowPsys(int mode){
	
	if (mode == 0){//free drop mode
		psystem.move();
	}
	else if (mode == 1){//wind mode
		psystem.moveWithWind(wind);
		wind = { rand() / double(RAND_MAX) / 1000 - 0.0005, rand() / double(RAND_MAX) / 1000 - 0.0005, rand() / double(RAND_MAX) / 1000 - 0.0005 };
	}
	else if (mode == 2){//acc mode
		psystem.moveWithAcc(acc);
		angle += 5;
		acc = { sin(angle*transPi)*0.00125, 0, cos(angle*transPi)*0.00125 };
	}
	else if (mode == 3){//circle mode
		Point margin1 = tpcenpair2.second;
		Point center1 = tpcenpair2.first;
		center1.y -= margin1.y;
		psystem.moveCircle(center1, 0.0025, sqrt(margin1.x*margin1.x + margin1.z*margin1.z)*2);
		angle += 5;
		acc = { sin(angle*transPi)*0.00125, 0, cos(angle*transPi)*0.00125 };
	}
	//circle
	if (circlepsys){
		Point margin1; Point center1;
		if (!change&&changedir == true){
			margin1 = tpcenpair1.second;
			center1 = tpcenpair1.first;
			center1.y -= margin1.y;
		}
		else if (!change&&changedir == false){
			margin1 = tpcenpair2.second;
			center1 = tpcenpair2.first;
			center1.y -= margin1.y;
		}
		//psystemCircle.moveCircle0(center1, 0.01, sqrt(margin1.x*margin1.x + margin1.z*margin1.z));
		//psystemCircle.moveWithPCircle(1, sqrt(margin1.x*margin1.x + margin1.z*margin1.z)*2);
		psystemCirclex.moveWithPCircle(1, sqrt(margin1.x*margin1.x + margin1.z*margin1.z) );
		psystemCircley.moveWithPCircle(2, sqrt(margin1.x*margin1.x + margin1.z*margin1.z));
		psystemCirclez.moveWithPCircle(3, sqrt(margin1.x*margin1.x + margin1.z*margin1.z));
		//vector<Particle>partvec0 = psystemCircle.getParticles();
		vector<Particle>partvecx = psystemCirclex.getParticles();
		vector<Particle>partvecy = psystemCircley.getParticles();
		vector<Particle>partvecz = psystemCirclez.getParticles();
		int count;
		glPointSize(3.0f);
		glBegin(GL_POINTS);
		//count = partvec0.size();
		//for (int i = 0; i < count; i++){
		//	if (partvec0[i].show>partvec0[i].lifetime){
		//		psystem.refreshDieP(i);
		//	}
		//	if (partvec0[i].show < 0)
		//		continue;
		//	glColor4f(partvec0[i].color.r, partvec0[i].color.g, partvec0[i].color.b, partvec0[i].color.alpha);
		//	glVertex3f(partvec0[i].x, partvec0[i].y, partvec0[i].z);
		//}
		count = partvecx.size();
		for (int i = 0; i < count; i++){
			if (partvecx[i].show < 0)
				continue;
			glColor4f(partvecx[i].color.r, partvecx[i].color.g, partvecx[i].color.b, partvecx[i].color.alpha);
			glVertex3f(partvecx[i].x, partvecx[i].y, partvecx[i].z);
		}
		 count = partvecy.size();
		for (int i = 0; i < count; i++){
			if (partvecy[i].show < 0)
				continue;
			glColor4f(partvecy[i].color.r, partvecy[i].color.g, partvecy[i].color.b, partvecy[i].color.alpha);
			glVertex3f(partvecy[i].x, partvecy[i].y, partvecy[i].z);
		}
		 count = partvecz.size();
		for (int i = 0; i < count; i++){
			if (partvecz[i].show < 0)
				continue;
			glColor4f(partvecz[i].color.r, partvecz[i].color.g, partvecz[i].color.b, partvecz[i].color.alpha);
			glVertex3f(partvecz[i].x, partvecz[i].y, partvecz[i].z);
		}
		glEnd();
	}
	
	vector<Particle> partvec = psystem.getParticles();

	int count = partvec.size();
	glPointSize(3.0f);

	bool draw = false;
	for (int i = 0; i < count; i++){
		if (partvec[i].show>partvec[i].lifetime){
			psystem.refreshDieP(i);
		}
		if (partvec[i].show < 0)
			continue;
		draw = true;
		glColor4f(partvec[i].color.r, partvec[i].color.g, partvec[i].color.b, partvec[i].color.alpha);
		//if (i%4==0)
		//	glColor4f(168.0 / 255, 216.0 / 255, 234.0 / 255, partvec[i].color.alpha);
		//else if (i%4==1)
		//	glColor4f(168.0 / 255, 149.0 / 255, 216.0 / 255, partvec[i].color.alpha);
		//else if (i % 4 == 2)
		//	glColor4f(251.0 / 255, 185.0 / 255, 210.0 / 255, partvec[i].color.alpha);
		//else
		//	glColor4f(255.0 / 255, 255.0 / 255, 210.0 / 255, partvec[i].color.alpha);
		
		glPushMatrix();
		glTranslatef(partvec[i].x, partvec[i].y, partvec[i].z);
		glutSolidSphere(partvec[i].size, 8, 8);
		glPopMatrix();
	}
	glEnd();
	return draw;
}


