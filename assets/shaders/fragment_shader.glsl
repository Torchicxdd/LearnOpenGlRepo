#version 330 core
out vec4 FragColour;
in vec3 vertexColour;
in vec2 texCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float textureVisibility = 0.5f;

void main()
{
	// You have to do 1.0 - coord to reverse the texture because if you make it negative, it may work at first glance,
	// however coords are from 0.0 - 1.0, therefore negatives will act differently depending on the texture settings (repeat, linear etc)
	// if (texCoord.x < 0.25f || texCoord.x > 0.75f || texCoord.y < 0.25f || texCoord.y > 0.75f) {
	// 	FragColour = vec4(1.0f, 1.0f, 1.0f, 0.0f);
	// } else {
		FragColour = mix(texture(texture1, texCoord), texture(texture2, texCoord), textureVisibility);
	// }
}