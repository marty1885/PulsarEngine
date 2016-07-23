#version 330 core

uniform mat4 transformMatrix;
uniform mat4 cameraMatrix;
uniform mat4 projectionMatrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textCoord;
layout (location = 2) in vec3 normal;

out vec3 vertexCoord;
out vec2 textureCoord;
out vec3 normalVec;

void main()
{
	gl_Position = projectionMatrix*cameraMatrix*transformMatrix*vec4(position,1.0);
	textureCoord = textCoord;
	normalVec = (transformMatrix*vec4(normal,0.0)).xyz;
	vertexCoord = (transformMatrix*vec4(position,1.0)).xyz;
}
