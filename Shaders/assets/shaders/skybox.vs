#version 330

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texcoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

out vec2 texCoord;

out vec3 TexCoords;


void main()
{
    vec4 pos = projectionMatrix * viewMatrix * modelMatrix * vec4(a_position, 1.0);
    TexCoords = vec3(modelMatrix * vec4(a_position,1.0));

	texCoord = a_texcoord;
    gl_Position = pos.xyww;

}

