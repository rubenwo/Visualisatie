#version 330

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texcoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;


out vec3 Normal;
out vec3 Position;

void main()
{
    Normal = mat3(transpose(inverse(modelMatrix))) * a_normal;
    Position = vec3(modelMatrix * vec4(a_position, 1.0));
    gl_Position = projectionMatrix * viewMatrix * vec4(Position, 1.0);
}