#version 330 core

uniform vec2 u_resolution;
uniform vec2 field;

float chessboard(vec2 uv)
{
    float xOffset = step(fract(uv.y), 0.5) * 0.5;
    return step(fract(uv.x + xOffset), 0.5);
}

void main() {
    if (u_resolution.x < u_resolution.y)
    {
        float gridsize = field.x / 2;//  gl_PointCoord
        gridsize += .5;
        gl_FragColor = vec4(0.,chessboard((gl_FragCoord.xy / u_resolution.xx) * gridsize) * .5 + .5, 0., 1.0);
    } else {
        float gridsize = field.y / 2;
        gridsize += .5;
        gl_FragColor = vec4(0.,chessboard((gl_FragCoord.xy / u_resolution.yy) * gridsize) * .5 + .5, 0., 1.0);
    }
}