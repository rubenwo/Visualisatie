#version 330

uniform sampler2D s_texture;
in vec2 texCoord;

void main()
{
        float pixel_amt = 256.0;
        float dx = 15.0 * (1.0 / pixel_amt);
        float dy = 10.0 * (1.0 / pixel_amt);
        vec2 Coord = vec2(dx * floor(texCoord.x / dx),
                          dy * floor(texCoord.y / dy));
        gl_FragColor = texture2D(s_texture, Coord);
}