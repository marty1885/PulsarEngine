#version 330 core
out vec4 color;

in vec3 vertexCoord;
in vec3 normalVec;
in vec2 textureCoord;

uniform sampler2D surfaceTexture;

struct PointLight
{
	vec3 position;
	vec3 radiant;
};

uniform PointLight pointLight;

void main()
{
	vec3 diff = pointLight.position-vertexCoord;
	vec3 directionVec = normalize(diff);
	float dist = length(diff);
	vec3 lightStrengthOnFragment = pointLight.radiant*(dot(normalVec,directionVec)/(dist*dist));

	vec4 textureColor = texture(surfaceTexture,textureCoord);

	if(dot(textureColor.xyz,vec3(1)) == 0)//If no texture is applied
		textureColor = vec4(1,1,1,1);

	//Apply lighting
	vec3 renderedColor = textureColor.xyz*lightStrengthOnFragment;

	//Gamma correction
	const float gamma = 2.2f;
	const float inverseGamma = 1.0f/gamma;
	renderedColor = max(renderedColor,vec3(0,0,0));
	vec3 correctedColor = pow(renderedColor,vec3(inverseGamma));
	color = vec4(correctedColor,1);
}
