#include "stdafx.h"
#include "objLoader.h"
#include <sstream>
#include <fstream>
using namespace std;
//以面为单位进行绘制（面是一个个的三角形）
void Mesh::draw()
{
	for (int i = 0; i < faces.size(); i++)
	{
		glBegin(GL_TRIANGLES);
		Face face = faces[i];
		for (int j = 0; j < 3; j++) {
			if (face.textureId[0] != 0) {
				Texture texture = textures[face.textureId[j] - 1];
				glTexCoord2f(texture.tx, texture.ty);
			}
			if (face.normalVectorId[0] != 0) {
				NormalVector normalVector = normalVectors[face.normalVectorId[j] - 1];
				glNormal3f(normalVector.nx, normalVector.ny, normalVector.nz);
			}
			if (face.vertexId[0] != 0) {
				Vertex vertex = vertexes[face.vertexId[j] - 1];
				glVertex3f(vertex.x, vertex.y, vertex.z);
			}
		}
		glEnd();
	}
}

void Mesh::parse(const char* filename)
{
	ifstream fin(filename);
	string line;
	float x, y, z, nx, ny, nz, tx, ty;
	while (getline(fin, line))
	{
		if (line.substr(0, 2) == "vt")//texture
		{
			istringstream iss(line.substr(2));
			iss >> tx;
			iss >> ty;
			textures.push_back(*(new Texture(tx, ty)));
		}
		else if (line.substr(0, 2) == "vn") //normal vector
		{
			istringstream iss(line.substr(2));
			iss >> nx;
			iss >> ny;
			iss >> nz;
			normalVectors.push_back(*(new NormalVector(nx, ny, nz)));
		}
		else if (line.substr(0, 1) == "v") //vertex
		{
			istringstream iss(line.substr(1));
			iss >> x;
			iss >> y;
			iss >> z;
			vertexes.push_back(*(new Vertex(x, y, z)));
		}
		else if (line[0] == 'f') {
			int fcase = 1;
			/*case1:f  1 2 3
			  case2:f  1/3 2/5 3/4
			  case3:f  1/3/4 2/5/6 3/4/2
			  case4:f  1//4 2//6 3//2
			*/
			for (int i = 1; i < line.size(); i++) {
				if(line[i]!=' '){
					for(int j = i;j < line.size(); j++){
						if (line[j] == ' ') break;
						if(line[j]=='/'){
							line[j] = ' ';
							fcase++;
							if(line[j+1]=='/'){
								fcase = 4;
								break;
							}
						}
					}
					break;
				}
			}
			for(int i= 0;i< line.size();i++){
				if(line[i]=='/'){
					line[i] = ' ';
				}
			}
			istringstream iss(line.substr(1));
			Face *face = new Face();
			if(fcase == 1){
				for (int i = 0; i < 3; i++) {
					iss >> face->vertexId[i];
				}
			}else if(fcase == 2){
				for (int i = 0; i < 3; i++) {
					iss >> face->vertexId[i];
					iss >> face->textureId[i];
				}
			}else if(fcase == 3){
				for (int i = 0; i < 3; i++) {
					iss >> face->vertexId[i];
					iss >> face->textureId[i];
					iss >> face->normalVectorId[i];
				}
			}else if(fcase ==4){
				for (int i = 0; i < 3; i++) {
					iss >> face->vertexId[i];
					iss >> face->normalVectorId[i];
				}
			}
			faces.push_back(*face);
		}
	}
}
