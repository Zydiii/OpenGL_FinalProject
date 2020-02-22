#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 Normal;
out vec3 FragPos;

void main()
{	
	Normal=normalize(aNormal);
	vec4 WorldPos=model*vec4(aPos,1.0);
	FragPos=WorldPos.xyz;
	gl_Position=projection*view*model*vec4(aPos,1.0f);

}
