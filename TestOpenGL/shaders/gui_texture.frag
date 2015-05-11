#version 120
uniform float alpha;
uniform sampler2D texture;

varying vec2 v_uv;

void main(void)
{
	vec4 outColor = texture2D(texture, v_uv);
	outColor.a = outColor.a * alpha;
    gl_FragColor = outColor;
}
