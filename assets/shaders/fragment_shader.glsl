#version 330 core
out vec4 FragColour;
in vec3 vertexColour;
in vec3 vertexPosition;

// uniform vec4 ourColour;

void main()
{
	FragColour = vec4(vertexPosition, 1.0f);
}