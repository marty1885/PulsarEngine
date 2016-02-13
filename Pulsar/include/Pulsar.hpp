#ifndef PULSAR_H
#define PULSAR_H

#include <iostream>
#include <string>
#include <vector>

#include <PulsarMath.hpp>

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

	vec3 getTranslation() const;
	mat4x4 getTransformMatrix() const;
	mat4x4 getScalingMatrix() const;
	mat4x4 getTranlationMatrix() const;
	void translate(vec3 vec);
	void rotate(vec3 vec);
	void scale(vec3 vec);

	mat4x4 getRotationMatrix() const;
protected:
	vec3 translation = vec3(0,0,0);
	vec3 rotation = vec3(0,0,0);
	vec3 scaling = vec3(1,1,1);
};

class Projection
{
public:
	void setProjection(float f, float w, float h, float near, float far);
	mat4x4 getProjectionMatrix() const;
protected:
	float zNear;
	float zFar;
	float width;
	float height;
	float fov;
};

class File
{
public:
	static string readAllText(string path);
};

class Camera
{
public:
	Camera(vec3 pos, vec3 dir, vec3 upVec);
	Camera();

	void move(vec3 dir);
	void rotateX(float rad);
	void rotateY(float rad);
	void rotateZ(float rad);

	void setDirection(vec3 dir);
	void setUp(vec3 upVec);

	vec3 getPosition() const;
	vec3 getDirection() const;
	vec3 getRight() const;
	vec3 getUp() const;

	mat4x4 getCameraMatrix() const;

	Projection getProjection() const;
	void setProjection(Projection proj);
protected:
	vec3 position;
	vec3 direction;
	vec3 up;
	Projection projection;
};

}

#endif
