#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D bilboardTexture;

void main()
{             
    FragColor = texture(bilboardTexture, TexCoords);
}