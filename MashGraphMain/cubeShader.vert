#version 330 core
layout (location = 0) in vec3 cubePos;
layout (location = 1) in vec3 cubeNormal;
layout (location = 2) in vec2 cubeTexCoords;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = cubeTexCoords;
    FragPos = vec3(model * vec4(cubePos, 1.0));
    Normal = mat3(transpose(inverse(model))) * cubeNormal;  
    
    
    gl_Position = projection * view * model * vec4(cubePos, 1.0f);
}