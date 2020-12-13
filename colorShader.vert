#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 colorTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = colorTexCoords;    
    gl_Position = projection * view * model * vec4(Position, 1.0);
}