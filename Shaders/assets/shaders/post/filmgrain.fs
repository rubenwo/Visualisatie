#version 330

uniform sampler2D s_texture;
uniform float time;

in vec2 texCoord;

void main()
{
	float strength = 30.0;

	float x = (texCoord.x + 4.0)*(texCoord.y *4.0)*(time*10.0);
	
	vec4 grain = vec4(mod((mod(x, 13.0) + 1.0) * (mod(x, 123.0) + 1.0), 0.01)-0.005) * strength;
	
	vec4 color = texture2D(s_texture, texCoord);
	
//	if (texCoord.x > 0.5)
//	{
//		grain = 1.0 - grain;
//		gl_FragColor = color * grain;
//	}
//	else
//	{
		gl_FragColor = color + grain;
//	}
}