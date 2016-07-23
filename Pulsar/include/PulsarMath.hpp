#ifndef PULSARMATH_H
#define PULSARMATH_H

#define _USE_MATH_DEFINES
#include <math.h>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
    #define M_PI_2 (M_PI/2.0)
    #define M_PI_4 (M_PI/4.0)
#endif

//This will effect every file that includes Pulsar.hpp
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include <Quaternion.hpp>

namespace Pulsar
{
	typedef glm::vec2 vec2;
	typedef glm::vec3 vec3;
	typedef glm::vec4 vec4;
	typedef glm::mat2 mat2;
	typedef glm::mat3 mat3;
	typedef glm::mat4 mat4;

	typedef vec4 float4;
	typedef vec3 float3;
	typedef vec2 float2;
	typedef glm::ivec4 int4;
	typedef glm::ivec3 int3;
	typedef glm::ivec2 int2;

	using namespace glm;

    inline float4 cross(float4 u, float4 v)
	{
		return float4(glm::cross(float3(u), float3(v)), 0);
	}

	// cout float4
	inline std::ostream& operator<< (std::ostream& out, const float4 v)
	{
		return out << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")" ;
	}

	// cout float3
	inline std::ostream& operator<< (std::ostream& out, const float3 v)
	{
		return out << "(" << v.x << ", " << v.y << ", " << v.z << ")" ;
	}

	// input float4
	inline std::istream& operator>>(std::istream& is, float4& v)
	{
	    float x, y, z, w;
	    is >> x >> y >> z >> w;
	    v = float4(x, y, z, w);
	    return is;
	}

	// input float3
	inline std::istream& operator>>(std::istream& is, float3& v)
	{
	    float x, y, z;
	    is >> x >> y >> z;
	    v = float3(x, y, z);
	    return is;
	}
}

#endif
