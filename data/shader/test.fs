#version 330 core
out vec4 color;
in vec4 theColor;
in vec2 textureCoord;

uniform sampler2D surfaceTexture;

void main()
{
	vec4 textureColor = texture(surfaceTexture,textureCoord);

	if(dot(textureColor.xyz,vec3(1)) == 0)
		color = vec4(1,1,1,1);
	else
		color = textureColor;
}
