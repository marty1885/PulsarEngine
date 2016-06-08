#version 330 core
out vec4 color;

in vec3 vertexCoord;
in vec3 normalVec;

void main()
{
	const vec3 lightPos = vec3(0.4,2,0);
	const vec3 strength = vec3(1,1,1);
	vec3 diff = lightPos-vertexCoord;
	vec3 directionVec = normalize(diff);
	float dist = length(diff);
	vec3 renderedColor = strength*(dot(normalVec,directionVec)/(dist*dist));
	renderedColor = max(renderedColor,vec3(0,0,0));
	const float gamma = 2.2f;
	const float inverseGamma = 1.0f/gamma;
	renderedColor = pow(renderedColor,vec3(inverseGamma));
	color = vec4(renderedColor,1);
}
