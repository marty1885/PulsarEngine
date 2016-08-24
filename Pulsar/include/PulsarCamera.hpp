#ifndef PULSARCAMERA_HPP
#define PULSARCAMERA_HPP

#include <PulsarMath.hpp>
#include <PulsarRay.hpp>

namespace Pulsar
{
class Projection
{
public:
        void setProjection(float f, float w, float h, float near, float far);
        mat4x4 getProjectionMatrix();
protected:
        float zNear;
        float zFar;
        float width;
        float height;
        float fov;
};

class Camera
{
public:
        Camera();
        Camera(vec3 pos, vec3 dir, vec3 upVec);
        Camera(vec3 pos, vec3 dir, vec3 rightVec, vec3 upVec);
	virtual ~Camera();
        void lookAt(vec3 pos, vec3 lookAt, vec3 upVec);

        void move(vec3 dir);
        void rotateX(float rad);
        void rotateY(float rad);
        void rotateZ(float rad);

        void setPosition(vec3 pos);
        void setDirection(vec3 dir);
        void setUp(vec3 upVec);
        void setAspectRatio(float ratio);

        vec3 getPosition();
        vec3 getDirection();
        vec3 getRight();
        vec3 getUp();
        float getAspectRatio() const;

        mat4x4 getCameraMatrix();

        Projection getProjection();
        void setProjection(Projection proj);

        virtual Ray createCameraRay(float2 uv) const{return Ray();}

protected:
        vec3 position;
        vec3 direction;
        vec3 up;
        vec3 right;
        Projection projection;
        float pixelAspectRatio = 1.0f;
};

class PerspectiveCamera : public Camera
{
public:
        using Camera::Camera;
	virtual ~PerspectiveCamera();
        void setFOV(float2 fieldOfView);
        void setFOV(float fovX, float fovY);
        inline float2 getFOV() const
        {
        	return fov;
        }
        virtual Ray createCameraRay(float2 uv) const;
protected:
        float2 fov = float2(M_PI/2.0);//default as 45 deg of fov
};
}

#endif
