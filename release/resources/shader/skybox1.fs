#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

struct Light { 
    vec3 ambient;
};

uniform Light light;

uniform samplerCube skybox;

void main()
{    
    // ambient
    vec3 ambient = light.ambient * texture(skybox, TexCoords).rgb;

    FragColor = vec4(ambient, 1.0);
}