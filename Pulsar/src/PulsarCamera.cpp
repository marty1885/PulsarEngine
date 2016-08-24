#include <Pulsar.hpp>
#include <PulsarCamera.hpp>
using namespace Pulsar;

void Projection::setProjection(float f, float w, float h, float near, float far)
{
	fov = f;
	width = w;
	height = h;
	zNear = near;
	zFar = far;
}

mat4x4 Projection::getProjectionMatrix()
{
	return glm::perspective(fov,width/height,zNear,zFar);
}

Camera::Camera()
{
	position = vec3(0,0,0);
	direction = vec3(0,0,-1);
	up = vec3(0,1,0);
    right = normalize(cross(direction,up));
}

Camera::Camera(vec3 pos, vec3 dir, vec3 upVec)
{
	position = pos;
	direction = normalize(dir);
	up = normalize(upVec);
    right = normalize(cross(direction,up));
}

Camera::Camera(vec3 camPosition, vec3 camDirection, vec3 camRight, vec3 camUp)
{
	position = camPosition;
	direction = camDirection;
	right = camRight;
	up = camUp;
}

Camera::~Camera()
{
}

void Camera::lookAt(vec3 pos, vec3 lookAt, vec3 upVec)
{
	vec3 diffBtw = lookAt - pos;
	position = pos;
	right = normalize(cross(diffBtw,upVec));
	direction = normalize(diffBtw);
	up = normalize(upVec);
}

mat4x4 Camera::getCameraMatrix()
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

void Camera::setPosition(vec3 pos)
{
	position = pos;
}

void Camera::setDirection(vec3 dir)
{
	direction = dir;
}

void Camera::setUp(vec3 upVec)
{
	up = upVec;
}

vec3 Camera::getPosition()
{
	return position;
}

vec3 Camera::getDirection()
{
	return direction;
}

vec3 Camera::getRight()
{
	return normalize(cross(direction,up));
}

vec3 Camera::getUp()
{
	return up;
}

Projection Camera::getProjection()
{
	return projection;
}

void Camera::setProjection(Projection proj)
{
	projection = proj;
}

void Camera::setAspectRatio(float ratio)
{
	pixelAspectRatio = ratio;
}

float Camera::getAspectRatio() const
{
	return pixelAspectRatio;
}

PerspectiveCamera::~PerspectiveCamera()
{
}

void PerspectiveCamera::setFOV(float2 fieldOfView)
{
	fov = fieldOfView;
}

void PerspectiveCamera::setFOV(float fovX, float fovY)
{
	fov = float2(fovX,fovY);
}

Ray PerspectiveCamera::createCameraRay(float2 uv) const
{
	vec3 rayVec = (uv.x*2.0f-1.0f)*right*tanf(fov.x*0.5f) * pixelAspectRatio
		+ (1.0f-uv.y*2.0f)*up*tanf(fov.y*0.5f)
		+ direction;
	rayVec = normalize(rayVec);

	Ray ray(position,rayVec);
	return ray;
}
