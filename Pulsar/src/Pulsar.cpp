#include <Pulsar.hpp>
using namespace Pulsar;

#include <fstream>
using namespace std;

void Renderer::clearScreen()
{
	//TODO: stencil buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(clearColor.r,clearColor.g,clearColor.b,1);
}

void Renderer::initFrame()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	//TODO: Depth clamp

	//Auto Gamma correction
	//glEnable(GL_FRAMEBUFFER_SRGB);
}

bool Renderer::init()
{
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if(glewError != GLEW_OK)
	{
		cout << "Failed to initlaze GLEW Error : " << glewGetErrorString(glewError) << endl;
		return false;
	}
	glGetError();//Clear any error of OpenGL that GLEW creates
	return true;
}

void Renderer::setClearColor(vec3 color)
{
	clearColor = color;
}

vec3 Transform::getTranslation()
{
	return translation;
}

mat4x4 Transform::getTransformation()
{
	mat4x4 translationMatrix = getTranlationMatrix();
	mat4x4 rotationMatrix = getRotationMatrix();
	mat4x4 scalingMatrix = getScalingMatrix();

	return translationMatrix*rotationMatrix*scalingMatrix;
}

void Transform::translate(vec3 vec)
{
	translation = vec;
}

void Transform::rotate(vec3 vec)
{
	rotation = vec;
}

void Transform::scale(vec3 vec)
{
	scaling = vec;
}

mat4x4 Transform::getRotationMatrix()
{
	mat4x4 rx;
	mat4x4 ry;
	mat4x4 rz;

	rx = glm::rotate(rotation.x,vec3(1,0,0));
	ry = glm::rotate(rotation.y,vec3(0,1,0));
	rz = glm::rotate(rotation.z,vec3(0,0,1));
	return rz*ry*rx;
}

mat4x4 Transform::getScalingMatrix()
{
	return glm::scale(scaling);
}

mat4x4 Transform::getTranlationMatrix()
{
	return glm::translate(translation);
}

string File::readAllText(string path)
{
	ifstream ifs(path);

	if(ifs.good() == false)
	{
		cout << "Failed to open file " << path << endl;
		return "";
	}

	string content( (istreambuf_iterator<char>(ifs) ),
		(istreambuf_iterator<char>()));
	return content;
}
