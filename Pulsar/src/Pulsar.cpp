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

vec3 Transform::getTranslation() const
{
	return translation;
}

mat4x4 Transform::getTransformation() const
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

mat4x4 Transform::getRotationMatrix() const
{
	mat4x4 rx;
	mat4x4 ry;
	mat4x4 rz;

	rx = glm::rotate(rotation.x,vec3(1,0,0));
	ry = glm::rotate(rotation.y,vec3(0,1,0));
	rz = glm::rotate(rotation.z,vec3(0,0,1));
	return rz*ry*rx;
}

mat4x4 Transform::getScalingMatrix() const
{
	return glm::scale(scaling);
}

mat4x4 Transform::getTranlationMatrix() const
{
	return glm::translate(translation);
}

void Projection::setProjection(float f, float w, float h, float near, float far)
{
	fov = f;
	width = w;
	height = h;
	zNear = near;
	zFar = far;
}

mat4x4 Projection::getProjectionMatrix() const
{
	return glm::perspective(fov,width/height,zNear,zFar);
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

Camera::Camera(vec3 pos, vec3 dir, vec3 upVec)
{
	position = pos;
	direction = normalize(dir);
	up = normalize(upVec);
}

Camera::Camera()
{
	position = vec3(0,0,0);
	direction = vec3(0,0,-1);
	up = vec3(0,1,0);
}

mat4x4 Camera::getCameraMatrix() const
{
	mat4x4 viewMatrix = glm::lookAt(position, direction+position, up);
	return viewMatrix;
}

void Camera::move(vec3 dir)
{
	position += dir;
}

void Camera::rotateX(float rad)
{
	vec3 right = normalize(cross(direction,up));

	direction = normalize(glm::rotate(direction,rad,right));
	up = normalize(cross(right,direction));

}

void Camera::rotateY(float rad)
{
	direction = normalize(glm::rotate(direction,rad,up));

}

void Camera::rotateZ(float rad)
{
	up = normalize(glm::rotate(up,rad,direction));
}

void Camera::setDirection(vec3 dir)
{
	direction = dir;
}

void Camera::setUp(vec3 upVec)
{
	up = upVec;
}

vec3 Camera::getPosition() const
{
	return position;
}

vec3 Camera::getDirection() const
{
	return direction;
}

vec3 Camera::getRight() const
{
	return normalize(cross(direction,up));
}

vec3 Camera::getUp() const
{
	return up;
}

Projection Camera::getProjection() const
{
	return projection;
}

void Camera::setProjection(Projection proj)
{
	projection = proj;
}
