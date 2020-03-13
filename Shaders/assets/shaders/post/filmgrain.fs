#version 330 core

uniform sampler2D s_texture;
in vec2 texCoord;

float random (in vec2 st) { 
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))
                 * 43758.5453123);
}

void main()
{
	//gl_FragColor = vec4(random(colorIn.xy),random(colorIn.xy),random(colorIn.xy), 1.0);
    	gl_FragColor = texture2D(s_texture, texCoord);

}