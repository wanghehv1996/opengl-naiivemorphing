#include "model.h"
#include "read.h"
#include"SOIL.h"

inline int power_of_two(int n)
{
	if (n <= 0) return 0;
	return (n&(n - 1)) == 0;
}

GLuint readTexture(string pathname)
{

	if (pathname.substr(pathname.length() - 4) != ".bmp"){
		cout << pathname << "is tga file" << endl;
		int width1, height1, channels;

		unsigned char* map = SOIL_load_image(pathname.c_str(), &width1, &height1, &channels, SOIL_LOAD_AUTO);
		GLuint textureid, lastid;
		//glGenTextures(1, &textureid);
		textureid = SOIL_load_OGL_texture(pathname.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *)&lastid);
		//glBindTexture(GL_TEXTURE_2D, textureid);
		//设置一些效果 参考网上
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		//glTexImage2D(GL_TEXTURE_2D, 0, GLUT_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, map);
		glBindTexture(GL_TEXTURE_2D, lastid);
		return textureid;
	}
	GLint width, height, total_bytes;
	GLuint last_texture_ID, texture_ID = 0;
	GLubyte *pixels = NULL;
	FILE *pFile;
	if (fopen_s(&pFile, pathname.c_str(), "rb"))	{
		cout << "read texture error pathname: " <<pathname<< endl;
		return 0;
	}

	fseek(pFile, 18, SEEK_SET);
	fread(&width, sizeof(width), 1, pFile);
	fread(&height, sizeof(height), 1, pFile);
	fseek(pFile, 54, SEEK_SET);

	total_bytes = (width * 3 + (4 - width * 3 % 4) % 4)*height;//保证行对齐
	if ((pixels = (GLubyte *)malloc(total_bytes)) == NULL){
		fclose(pFile);
		return 0;
	}

	if (fread(pixels, total_bytes, 1, pFile) <= 0)	{
		free(pixels);
		fclose(pFile);
		return 0;
	}
	
	glGenTextures(1, &texture_ID);
	if (texture_ID == 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *)&last_texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	//设置一些效果 参考网上
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, last_texture_ID);

	free(pixels);
	fclose(pFile);
	return texture_ID;
}


GLubyte uTGAcompare[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	// Uncompressed TGA Header
GLubyte cTGAcompare[12] = { 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	// Compressed TGA Header
GLuint readTexturetga(string pathname){
	cout << "begin load tga texture" << endl;
	Texture texture;
	GLuint last_texture_ID, texture_ID = 0;

	GLubyte header[12];
	FILE * file;
	file = fopen(pathname.c_str(), "rb");

	if (!file){
		std::cout << "Could not open texture file" << std::endl;
		return 0;
	}

	if (fread(&header, sizeof(header), 1, file) == 0)	{
		std::cout << "Could not read file header" << std::endl;	
		if (file != NULL)	{
			fclose(file);
		}
		return 0;
	}
	if (memcmp(cTGAcompare, &header, sizeof(header)) == 0)	{
		std::cout << "load compressed" << std::endl;
		LoadCompressedTGA(&texture, pathname, file);
	}
	else if(memcmp(uTGAcompare, &header, sizeof(header)) == 0){
		std::cout << "load uncompressed" << std::endl;
		LoadUncompressedTGA(&texture, pathname, file);
	}
	else	{
		std::cout << "error type of tga file" << std::endl;
		fclose(file);
		return 0;
	}
	glGenTextures(1, &texture_ID);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *)&last_texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	//设置一些效果 参考网上
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, texture.type, texture.width, texture.height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture.data);
	glBindTexture(GL_TEXTURE_2D, last_texture_ID);

	return texture_ID;
}

void readMtl(string pathname,map<string,Material> &mtlMap,int texid)
{
	cout << "readmtl:" << pathname << endl;
	size_t lastPos = pathname.find_last_of('\\');
	string path;
	if (lastPos != std::string::npos)
		path = pathname.substr(0, lastPos);
	else
		path = ".";

	string line,word,mtlname;
	unsigned KaTexID = 0, KdTexID = 0, KsTexID = 0, KeTexID = 0;
	float KaArr[3] = { 0.0, 0.0, 0.0 };//no use
	float KdArr[3] = { 0.0, 0.0, 0.0 };//no use
	float KsArr[3] = { 0.0, 0.0, 0.0 };//no use
	float KeArr[3] = { 0.0, 0.0, 0.0 };//no use
	float ns = 0, d = 0;//no use

	ifstream in;

	in.open((pathname).c_str());
	if(!in)	{
		cout<<"[* - *]read mtl error pathname: "<<pathname<<endl;
		char ch;
		std::cin >> ch;
		std::exit(0);
	}

	while(getline(in,line))	{
		if (line.size() == 0||line[0] == '#'  ) continue;
		istringstream is(line);
		is>>word;
		//new mtl
		if(word=="newmtl")	{
			if (mtlname.empty()){
				is >> mtlname;
				continue;
			}
			Material mtl(KaArr, KdArr, KsArr, KeArr, 0);
			mtl.ambientId = KaTexID;
			mtl.diffuseId = KdTexID;
			mtl.specularId = KsTexID;
			mtl.emissionId = KeTexID;
			mtlMap.insert(make_pair(mtlname, mtl));
			KaTexID = 0;
			KdTexID = 0;
			KsTexID = 0;
			KeTexID = 0;
			is >> mtlname;
		}

		//GL_AMBIENT 模型环境颜色 （ambient color）用Ka声明。
		else if(word=="Ka") {
			is >> KaArr[0] >> KaArr[1] >> KaArr[2];
		}

		//GL_DIFFUSE 模型散射颜色（diffuse color）用Kd。
		else if(word=="Kd") {
			is >> KdArr[0] >> KdArr[1] >> KdArr[2];
		}

		//GL_SPECULATE 模型镜面颜色（specular color）用Ks。
		else if(word=="Ks") {
			is >> KsArr[0] >> KsArr[1] >> KsArr[2];
		}
		//GL_EMISSION 模型发射颜色
		else if(word=="Ke")	{
			is >> KeArr[0] >> KeArr[1] >> KeArr[2];
		}
		//模型镜面指数
		else if (word == "Ns")	{
			is >> ns;
		}

		else if(word=="map_Ka") {
			string texfilename;
			is >> texfilename;
			//if (texid != 0){
			//	int pos = texfilename.find(".bmp");
			//	if (pos != std::string::npos){
			//		sout.clear();
			//		char texch = '0' + texid;
			//		sout << texfilename.substr(0, pos)<<texch<<".bmp";
			//		texfilename = sout.str();
			//	}
			//}
			KaTexID = readTexture(path + "\\" + texfilename);
		}
		else if (word == "map_Kd") {
			string texfilename;
			is >> texfilename;
			//if (texid != 0){
			//	int pos = texfilename.find(".bmp");
			//	if (pos != std::string::npos){
			//		sout.clear();
			//		char texch = '0' + texid;
			//		sout << texfilename.substr(0, pos) << texch << ".bmp";
			//		texfilename = sout.str();
			//	}
			//}
			KdTexID = readTexture(path + "\\" + texfilename);
		}
		else if (word == "map_Ks") {
			string texfilename;
			is >> texfilename;
			//if (texid != 0){
			//	int pos = texfilename.find(".bmp");
			//	if (pos != std::string::npos){
			//		sout.clear();
			//		char texch = '0' + texid;
			//		sout << texfilename.substr(0, pos) << texch << ".bmp";
			//		texfilename = sout.str();
			//	}
			//}
			KsTexID = readTexture(path + "\\" + texfilename);
		}
		else if (word == "map_Ke") {
			string texfilename;
			is >> texfilename;
			//if (texid != 0){
			//	int pos = texfilename.find(".bmp");
			//	if (pos != std::string::npos){
			//		sout.clear();
			//		char texch = '0' + texid;
			//		sout << texfilename.substr(0, pos) << texch << ".bmp";
			//		texfilename = sout.str();
			//	}
			//}
			KeTexID = readTexture(path + "\\" + texfilename);
		}
	}//end while

	if(!mtlname.empty())
	{
		Material mtl(KaArr, KdArr, KsArr, KeArr, 0);
		mtl.ambientId = KaTexID;
		mtl.diffuseId = KdTexID;
		mtl.specularId = KsTexID;
		mtl.emissionId = KeTexID;
		cout << KaTexID << "show texid" << KdTexID << endl;
		mtlMap.insert(make_pair(mtlname, mtl));
		KaTexID = 0;
		KdTexID = 0;
		KsTexID = 0;
		KeTexID = 0;
		cout << "tex id: ka:" << KaTexID << "\tkd:" << KdTexID << "\tks:" << KsTexID << "\tke:" << KeTexID << endl;
	}
	in.close();
}

void readObj(string pathname,map<string,Object> &objMap,map<string,Material> &mtlMap, int texid)
{
	objMap.clear();
	mtlMap.clear();
	vector<Point> points;
	vector<TexCoord> texcoords;
	vector<Point> norvecs;
	vector<FacePoint> facepnts;
	int pol = 0;
	string line, word, objname, mtlname;

	size_t lastPos = pathname.find_last_of('\\');
	string path;
	if (lastPos != std::string::npos)
		path = pathname.substr(0, lastPos);
	else
		path = ".";

	ifstream in;
	in.open(pathname.c_str());
	if(!in){
		cout<<"[* - *]read obj error pathname: "<<pathname<<endl;
		char ch;
		std::cin >> ch;
		std::exit(0);
	}
	while(getline(in,line)){
		if(line.size()==0||line[0]=='#') continue;
		istringstream is(line);
		is>>word;
		//v for point
		if(word=="v"){
			Point p;
			is>>p.x>>p.y>>p.z;
			points.push_back(p);
		}
		//vt for texture coordinate
		else if(word=="vt"){
			TexCoord tc;
			is >> tc.x >> tc.y;
			texcoords.push_back(tc);
		}
		//vn for normal vector faxiangliang
		else if(word=="vn"){
			Point p;
			is>>p.x>>p.y>>p.z;
			norvecs.push_back(p);
		}
		//o/g for object/group
		else if(word=="o"||word=="g"){
			if (objname.empty() || facepnts.empty()){
				is >> objname;
				continue;
			}
			Object obj;
			obj.points = points;
			obj.texcoords = texcoords;
			obj.norvecs = norvecs;
			obj.facepnts = facepnts;
			obj.material = mtlname;
			obj.pol = pol;
			objMap.insert(make_pair(objname, obj));
			facepnts.clear();
			is >> objname;
		}
		//f for face
		else if(word=="f"){
			int ptmp = 0;
			while(is>>word){
				size_t  slashPos = word.find('/');
				//顶点
				if (slashPos == std::string::npos){
					FacePoint fp;
					fp.pid = atoi(word.c_str());
					facepnts.push_back(fp);
					ptmp++;
					continue;
				}

				size_t slashPos1 = word.find("/", slashPos + 1);
				//顶点/纹理坐标
				if (slashPos1 == std::string::npos){
					string pointStr = word.substr(0, slashPos);
					string tcStr = word.substr(slashPos+1, word.size());
					FacePoint fp;
					fp.pid = (atoi(pointStr.c_str()));
					fp.tcid = atoi(tcStr.c_str());
					facepnts.push_back(fp);
					ptmp++;
					continue;
				}

				//顶点/纹理坐标/法向
				string pointStr = word.substr(0, slashPos);
				string tcStr = word.substr(slashPos + 1, slashPos1);
				string nvStr = word.substr(slashPos1 + 1, word.size());

				FacePoint fp;
				fp.pid = atoi(pointStr.c_str());
				fp.tcid = atoi(tcStr.c_str());
				fp.vnid = atoi(nvStr.c_str());
				facepnts.push_back(fp);

				ptmp++;
				continue;
			}
			pol = ptmp;
		}
		else if(word=="mtllib"){
			//is>>word;
			//cout << "usemtlib as"<<path<<"\\"<<word << endl;
			stringstream sout;
			string filename;
			is >> filename;
			string filename0;
			if (texid != 0){
				int pos = filename.find(".mtl");
				if (pos != std::string::npos){
					sout.clear();
					char texch = '0' + texid;
					sout << filename.substr(0, pos) << texch << ".mtl";
					filename0 = sout.str();
				}
			}
			string filestr = path + "\\" + filename0;
			if (fopen(filestr.c_str(),"r")!=NULL)
				readMtl(path + "\\" + filename0, mtlMap, 0);
			else
				readMtl(path + "\\" + filename, mtlMap, 0);
		}else if(word=="usemtl"){
			if (!facepnts.empty()){
				Object obj;
				obj.points = points;
				obj.texcoords = texcoords;
				obj.norvecs = norvecs;
				obj.facepnts = facepnts;
				obj.material = mtlname;
				obj.pol = pol;
				objMap.insert(make_pair("model", obj));
				cout << "read model:\tpsize: " << points.size() << "\ttexsize: " << texcoords.size() << "\tnorvec.size: " << norvecs.size() << "\tfacepnt: " << facepnts.size() << "\n\tmtl: " << mtlname << "\tpol" << pol << endl;
				facepnts.clear();
			}
			is>>mtlname;
		}
	}
	if (!objname.empty() && !facepnts.empty()){
		Object obj;
		obj.points = points;
		obj.texcoords = texcoords;
		obj.norvecs = norvecs;
		obj.facepnts = facepnts;
		obj.material = mtlname;
		obj.pol = pol;
		objMap.insert(make_pair(objname, obj));
		facepnts.clear();
		cout << "read model:\tpsize: " << points.size() << "\ttexsize: " << texcoords.size() << "\tnorvec.size: " << norvecs.size() << "\tfacepnt: " << facepnts.size() << "\n\tmtl: " << mtlname << "\tpol" << pol << endl;
	}
	if (!facepnts.empty()){
		Object obj;
		obj.points = points;
		obj.texcoords = texcoords;
		obj.norvecs = norvecs;
		obj.facepnts = facepnts;
		obj.material = mtlname;
		obj.pol = pol;
		objMap.insert(make_pair("model", obj));
		cout << "read model:\tpsize: " << points.size() << "\ttexsize: " << texcoords.size() << "\tnorvec.size: " << norvecs.size() << "\tfacepnt: " << facepnts.size() << "\n\tmtl: " << mtlname << "\tpol" << pol << endl;
		facepnts.clear();
	}
	in.close();
}
