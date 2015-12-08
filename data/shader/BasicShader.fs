#version 330 core

struct  BaseLight
{
	vec3 color;
	vec4 position;
};

struct SpotLight
{
	vec3 color;
	vec3 position;
	vec3 direction;
	float coneAngle;
};

out vec4 outColor;

in vec3 vertexCoord;
in vec2 textureCoord;
in vec3 normalVec;

uniform vec3 color;
uniform int withTexture;
uniform vec3 embientLight;
uniform float diffuseIntensity;
uniform vec3 cameraPosition;
uniform float specularIntensity;
uniform float specularExponent;

uniform sampler2D texture;

uniform BaseLight light;
uniform SpotLight spotLight;

uniform mat4 transform;
uniform mat4 camera;
uniform mat4 projection;

vec3 calcLight(vec3 lightColor, vec3 direction, vec3 surfaceNormal)
{
	const float cutoffFactor = 10e-6;
	float diffuseFactor = dot(normalize(direction),normalize(surfaceNormal));
	vec3 diffuseColor = vec3(0,0,0);
	vec3 specularColor = vec3(0,0,0);
	if(diffuseFactor > 0 || diffuseFactor < cutoffFactor)
	{
		 diffuseColor = lightColor*diffuseFactor*diffuseIntensity;

		 vec3 eyeVec = normalize(cameraPosition-vertexCoord);
		 vec3 reflectVec = normalize(reflect(direction, surfaceNormal));
		 float specularFactor = dot(eyeVec,reflectVec);
		 specularFactor = pow(specularFactor,specularExponent);
		 if(specularFactor > 0  || diffuseFactor < cutoffFactor)
		 {
			 specularColor = lightColor*specularIntensity*specularFactor;
		 }
	}
	return diffuseColor + specularColor;
}

vec3 calcSpotLight(SpotLight spotLight, vec3 direction, vec3 surfaceNormal)
{
	vec3 lightDirection = normalize(direction);
	float factor = dot(lightDirection,normalize(spotLight.direction));
	float angle = acos(factor);
	float dist = length(vertexCoord - spotLight.position);
	vec3 transmitLight = vec3(0,0,0);
	if(angle < spotLight.coneAngle)
	{
		vec3 transmitColor = spotLight.color/(dist*dist);
		transmitLight = calcLight(transmitColor,lightDirection, surfaceNormal)*
			(1.0 - (1-factor)/(1.0 - cos(spotLight.coneAngle)));
	}
	return transmitLight;
}

vec3 calcPointLight(BaseLight pointLight, vec3 direction, vec3 surfaceNormal)
{
	float dist = length(vertexCoord - pointLight.position.xyz);
	vec3 transmitColor = pointLight.color/(dist*dist);
	return calcLight(transmitColor, direction, surfaceNormal);
}

vec3 calcDirectionalLight(BaseLight directionalLight, vec3 direction, vec3 surfaceNormal)
{
	float dist = length(vertexCoord - directionalLight.position.xyz);
	vec3 transmitColor = directionalLight.color;
	return calcLight(transmitColor, direction, surfaceNormal);
}

void main()
{
	vec3 totalLight = embientLight;

	vec4 reasult = vec4(color,1.0);
	if(withTexture == 1)
		reasult *= texture2D(texture, vec2(textureCoord.x,textureCoord.y));
	//reasult *= dot(vec3(0,1,1),normalVec);//Basic Lighting
	float dist = length(vertexCoord - light.position.xyz);
	float cosVal = dot(normalize(normalVec),normalize(light.position.xyz));

	if(light.position.w == 0.0f)//spot light
	{
		// vec3 transmitColor = light.color/(dist*dist);
		// totalLight += calcLight(transmitColor, light.position.xyz-vertexCoord, normalVec);
		totalLight += calcPointLight(light, light.position.xyz-vertexCoord, normalVec);
	}
	else if(light.position.w == 1.0f)//directional light
	{
		// vec3 transmitColor = light.color;
		// totalLight += calcLight(transmitColor, light.position.xyz-vertexCoord, normalVec);
		totalLight += calcDirectionalLight(light, light.position.xyz-vertexCoord, normalVec);
	}
	totalLight += calcSpotLight(spotLight, spotLight.position-vertexCoord, normalVec);

	reasult.a = 1.0;
	reasult *= vec4(totalLight,1);
	if(reasult.r < 0 || reasult.g < 0 || reasult.b < 0)
            reasult = vec4(0,0,0,1);
	//reasult = clamp(reasult, vec4(0,0,0,0), vec4(1,1,1,1));

	const float gamma = 2.2;
        vec3 correctedColor = pow(reasult.rgb,vec3(1/gamma,1/gamma,1/gamma));
	outColor = vec4(correctedColor,1);
}
