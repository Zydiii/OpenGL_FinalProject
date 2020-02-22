#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
struct Light { 
    vec3 ambient;
};

uniform Light light;

uniform sampler2D texture1;

void main()
{             
    // ambient
    vec3 ambient = light.ambient * texture(texture1, TexCoords).rgb;

    //vec4 texColor = texture(texture1, TexCoords);
    //if(texColor.a < 0.1)
        //discard;
    FragColor = vec4(ambient, 1.0);
}