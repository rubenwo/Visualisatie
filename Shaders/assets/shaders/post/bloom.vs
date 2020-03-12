#version 330 core

layout(location=0) in vec3 a_position;
out vec2 texCoord;
out vec3 normal;

void main()
{
	texCoord = (vec2(a_position) + vec2(1, 1)) / 2.0;
	gl_Position = vec4(a_position, 1);
	normal = cross(vec3(1,1,1), a_position);
}