#version 330
uniform sampler2D s_texture;
uniform float time;

in vec2 texCoord;

void main()
{  
    vec2 resolution = vec2(1920,1080);

	vec2 uv = texCoord;
    vec4 tex = texture2D(s_texture, uv);
    vec2 n = texture2D(s_texture, uv * .1).rg;
    for (float r = 4.; r > 0.; r--) {
        vec2 x = resolution.xy * r * .015;
        vec2 p = 6.28 * uv * x + (n - .5) * 2.;
        vec2 s = sin(p);
        vec4 d = texture2D(s_texture, floor(uv * x - 0.25 + 0.5) / x);
        float t = (s.x + s.y) * max(0.,1.-fract(time * (d.b+.1)+d.g) * 2.);

        if (d.r < (5.-r) * .08 && t > .5) {
            vec4 color = vec4(sin(p), 0.0, 1.0);
            gl_FragColor = tex + color;
        }
    }
}