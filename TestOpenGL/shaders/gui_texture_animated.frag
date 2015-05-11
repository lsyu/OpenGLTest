#version 120
uniform float alpha;
uniform float mix_factor;
uniform sampler2D texture1;
uniform sampler2D texture2;

varying vec2 v_uv;

void main(void)
{
	vec4 outColor = mix(texture2D(texture1, v_uv), texture2D(texture2, v_uv), mix_factor);
	outColor.a = outColor.a * alpha;
    gl_FragColor = outColor;
}
