#version 120
attribute vec2 a_position;
attribute vec3 a_color;

varying vec4 v_color;

void main(void)
{
    v_color = vec4(a_color,1.0f);
    gl_Position = vec4(a_position, 0.0f, 1.0);
}
