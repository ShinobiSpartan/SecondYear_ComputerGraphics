// A simple textured shader
#version 410

in vec2 vTexCoord;

uniform smapler2D diffuseTexture;

out vec4 FragColour;

void main()
{
	FragColour = texture( diffuseTexture, vTexCoord );
}