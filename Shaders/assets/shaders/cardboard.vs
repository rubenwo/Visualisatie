#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec2 a_texcoord;
layout (location = 3) in vec3 a_normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

out vec2 texCoordIn;
out vec3 colorIn;
out vec3 normalIn;

void main()
{
	vec3 pos = a_position;
	pos.z = 0;
	colorIn = a_color;
	normalIn = a_normal;
	texCoordIn = a_texcoord;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(pos,1);
}