#version 330 core


out vec4 outColor;

in vec3 vertexCoord;
in vec2 textureCoord;
in vec3 normalVec;

void main()
{
	outColor = abs(vec4(normalize(normalVec),1));
}
