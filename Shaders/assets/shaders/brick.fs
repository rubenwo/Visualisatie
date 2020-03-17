#version 330

uniform sampler2D s_texture;
in vec2 texCoord;
in vec3 color;
in vec4 position;
uniform float time;

vec2 SineWave( vec2 p )
{
	float value1 = 0.6f;
	float value2 = 0.0f;
	float value3 = 0.05f;
	float x = sin( 25.0*p.y + 30.0*p.x + 6.28*time) * value3;
	float y = sin( 25.0*p.y + 30.0*p.x + 6.28*time) * value3;
	return vec2(p.x+x, p.y+y);
}


void main()
{
	vec2 brickSize = vec2(1, 0.25);

	brickSize = SineWave(brickSize);

	if (mod(position.x, brickSize.x) < 0.2 || mod(position.y, brickSize.y) < 0.025 )
		gl_FragColor = mix(vec4(1,1,1,1), texture2D(s_texture, texCoord), 0.5);
	else
		gl_FragColor = mix(normalize(vec4(210,105,30,255)), texture2D(s_texture, texCoord), 0.5);
}