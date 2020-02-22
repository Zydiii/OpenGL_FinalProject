#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;
out float time;

uniform float u_time;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    time = u_time;
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}  