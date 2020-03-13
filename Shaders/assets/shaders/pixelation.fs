#version 330

uniform sampler2D s_texture;
in vec2 texCoord;

void main()
{
        float Pixels = 256.0;
        float dx = 15.0 * (1.0 / Pixels);
        float dy = 10.0 * (1.0 / Pixels);
        vec2 Coord = vec2(dx * floor(texCoord.x / dx),
                          dy * floor(texCoord.y / dy));
        gl_FragColor = texture2D(s_texture, Coord);
}