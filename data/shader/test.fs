#version 330 core
out vec4 color;
in vec4 theColor;
in vec2 textureCoord;

uniform sampler2D texture;

void main()
{
	color = texture2D(texture, vec2(textureCoord.x,textureCoord.y));
}
