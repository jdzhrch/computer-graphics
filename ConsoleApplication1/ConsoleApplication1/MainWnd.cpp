#include "stdafx.h"
#include "MainWnd.h"
#include <sstream>
#include <fstream>
using namespace std;
void Mesh::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffers  // 清除屏幕及深度缓存
	glLoadIdentity(); // Load identity matrix	// 重置模型观察矩阵
	extern GLdouble y;
	glTranslated(0, y, 0);
	for (int i = 0; i < faces.size(); i++)
	{
		glBegin(GL_TRIANGLES);
		Face face = faces[i];
		for (int j = 0; j < 3; j++) {
			Texture texture = textures[face.textureId[j]-1];
			Vertex vertex = vertexes[face.vertexId[j]-1];
			glTexCoord2f(texture.tx, texture.ty);
			glVertex3f(vertex.x, vertex.y, vertex.z);
		}
		NormalVector normalVector = normalVectors[face.normalVectorId[0]-1];
		glNormal3f(normalVector.nx, normalVector.ny, normalVector.nz);		
		glEnd();
	}
	glFlush();
}

void Mesh::parse(const char* filename)
{
	ifstream fin(filename);//cube bunny Eight
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
			for (int i = 0; i < line.size(); i++) {
				if (line[i] == '/') {
					line[i] = ' ';
				}
			}
			istringstream iss(line.substr(1));
			Face *face = new Face();
			for (int i = 0; i < 3; i++) {
				iss >> face->textureId[i];
				iss >> face->vertexId[i];
				iss >> face->normalVectorId[i];
			}
			faces.push_back(*face);
		}
	}
}
