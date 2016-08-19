#ifndef PULSARRAY_HPP
#define PULSARRAY_HPP

#include <PulsarMath.hpp>

namespace Pulsar
{
    class Ray
    {
    public:
    	Ray(){}
    	Ray(vec3 ori, vec3 dir)
    	{
    		origin = ori;
    		direction = dir;
    	}

    	vec3 origin;
    	vec3 direction;

    	void calcSign(int* sign) const
    	{
    		sign[0] = direction[0] < 0;
    		sign[1] = direction[1] < 0;
    		sign[2] = direction[2] < 0;
    	}

    	/*inline float4 inverseDirection() const
    	{
    		//Workarround a bug/unlikelly issue in Ray-AABB intersection caused by inf in inverseDirection.
    		float4 inverseDirection = 1.0f/direction;
    		inverseDirection = inverseDirection + float4(glm::equal(direction,float4(0,0,0,1)))*float4(FLT_EPSILON);
    		return inverseDirection;
    	}*/
    };
}

#endif
