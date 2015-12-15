#ifndef PULSAR_MESH_HPP
#define PULSAR_MESH_HPP

#include <GL/glew.h>
#include <vector>
#include <string>
#include <iostream>

#include <PulsarMath.hpp>
#include <PulsarTexture.hpp>

namespace Pulsar
{
using namespace std;

class Mesh
{
public:
	Mesh();
	virtual ~Mesh();
	void setVertices(vec3* vertices, int size);
	void setIndices(unsigned int* indecies, int size);
	void setTextureCoord(vec2* uvCoord, int size);
	void setNormals(vec3* normals, int size);
	void render();
protected:
	GLuint vbo = 0;
	GLuint vao = 0;
	GLuint ebo = 0;
	GLuint tco = 0;//Texture coordinate object
	GLuint nbo = 0;//Normal array object
	int dataSize = 0;
};

class Model
{
public:
	bool load(string path);
	void render();
	void unload();

	bool good();
	int getMeshNum();
	const Mesh** getMeshes();
protected:
	vector<Mesh*> meshList;
};

}

#endif
