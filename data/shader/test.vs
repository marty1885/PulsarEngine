#version 330 core

uniform mat4 transform;
uniform mat4 camera;
uniform mat4 projection;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textCoord;
layout (location = 2) in vec3 normal;

out vec3 vertexCoord;
out vec2 textureCoord;
out vec3 normalVec;

void main()
{
	gl_Position = projection*camera*transform*vec4(position,1.0);
	textureCoord = textCoord;
	normalVec = (transform*vec4(normal,0.0)).xyz;
	vertexCoord = (transform*vec4(position,1.0)).xyz;
}
