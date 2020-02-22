#version 330 core

out vec4 FragColor;


uniform vec3 Ambient;
uniform vec3 Diffuse;
uniform vec3 LightPos;
uniform vec3 Color;

in vec3 Normal;
in vec3 FragPos;

void main()
{
	vec3 lightDir=normalize(LightPos-FragPos);
	vec3 diffuse=max(dot(lightDir,Normal),0.0)*Diffuse;
	vec3 result=(diffuse+Ambient)*Color;

	FragColor=vec4(result,1.0);
}