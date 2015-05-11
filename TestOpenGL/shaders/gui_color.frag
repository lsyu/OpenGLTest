#version 120
uniform float alpha;

varying vec4 v_color;

void main(void)
{
    gl_FragColor =  vec4(v_color.rgb, alpha);
}
