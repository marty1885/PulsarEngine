#ifndef PULSAR_H
#define PULSAR_H

#include <iostream>
#include <string>
#include <vector>

#include <PulsarMath.hpp>
#include <PulsarCamera.hpp>

#include <GL/glew.h>

namespace Pulsar
{

using namespace std;

class Renderer
{
public:
	void clearScreen();
	void initFrame();
	bool init();
	void setClearColor(vec3 color);
protected:
	vec3 clearColor;
};

class Transform
{
public:

	vec3 getTranslation();
	mat4x4 getTransformation();
	mat4x4 getScalingMatrix();
	mat4x4 getTranlationMatrix();
	void translate(vec3 vec);
	void rotate(vec3 vec);
	void scale(vec3 vec);

	mat4x4 getRotationMatrix();
protected:
	vec3 translation = vec3(0,0,0);
	vec3 rotation = vec3(0,0,0);
	vec3 scaling = vec3(1,1,1);
};

class File
{
public:
	static string readAllText(string path);
};

}

#endif
