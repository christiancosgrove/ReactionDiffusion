#version 330 core
out vec4 color;
in vec2 pos;

uniform sampler2D tex;

uniform float scale;

void main()
{
	vec2 npos = 0.5 * pos + vec2(0.5,0.5);

	float g = texture(tex,npos).g;

	float brightness;

	if (2*g < 0.5) brightness = mix(0,1,2 * 2 * g);
	else if (2*g < 0.75) brightness = mix(1,0.5, 4*(2 * g-0.5));
	else brightness = mix(1,0,4*(2 * g - 0.75));

	color = vec4(brightness,brightness,brightness,1.0f);

}