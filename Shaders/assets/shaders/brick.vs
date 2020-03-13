#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texcoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

out vec2 texCoord;


in vec3 a_color;
in vec2 screenSize;
uniform float time;

out vec3 colorIn;
out vec3 normalIn;


void main(){
	vec3 pos = a_position;
	
	vec2 brickSize = vec2(1, 1);

	colorIn = a_color;
	
	normalIn = a_normal;
	texCoord = a_texcoord;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(pos,1);
}
