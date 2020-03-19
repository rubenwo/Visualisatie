#version 330

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 TexCoords;


void main()
{
    TexCoords = a_position;
    vec4 pos = projectionMatrix * viewMatrix * vec4(a_position, 1.0);
    gl_Position = pos.xyww;

}

