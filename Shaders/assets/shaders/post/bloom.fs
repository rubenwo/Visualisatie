#version 330
uniform sampler2D s_texture;
in vec2 texCoord;

in vec3 normal; 
in vec3 color;

void main()
{
    float o = 24/2048.0;

	vec4 color;
	
	if (color.x > 0.4 || color.y > 0.4 || color.z > 0.4){


	} else {
		//color = texture2D(s_texture, texCoord);
	}

	color = (texture2D(s_texture, texCoord) +
        texture2D(s_texture, texCoord + vec2(-o, -o)) +
        texture2D(s_texture, texCoord + vec2(-o, o)) +
        texture2D(s_texture, texCoord + vec2(o, o)) +
        texture2D(s_texture, texCoord + vec2(o, -o))) / 5.0;
	
	if (color.x > 0.4)
		color.x += color.x * 0.5;
	if (color.y > 0.4)
		color.y += color.y * 0.5;
	if (color.z > 0.4)
		color.z += color.z * 0.5;

	gl_FragColor = color;
}