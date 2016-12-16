#version 330 core
out vec4 color;
in vec2 pos;

uniform sampler2D tex;

const float dx = 1.0f/1600;
const float dy = 1.0f/1600;

uniform float feed = 0.060;
uniform float kill = 0.0609;
uniform float delta = 0.2f;

uniform vec2 brush;

const float brushrad = 0.005;

uniform bool paused = false;

uniform bool delete = false;

void main()
{
	vec2 npos = 0.5 * pos + vec2(0.5,0.5);

	vec4 uv = texture(tex, npos);
	vec4 uv0 = texture(tex, npos + vec2(-dx, 0));
	vec4 uv1 = texture(tex, npos + vec2( dx, 0));
	vec4 uv2 = 1.0*texture(tex, npos + vec2(  0, -dy));
	vec4 uv3 = 1.0*texture(tex, npos + vec2(  0, dy));

	vec4 lapl = uv0 + uv1 + uv2 + uv3 - 4 * uv;

	const float coup = 1.0;
	float du = 0.8*lapl.r - coup * uv.r * uv.g * uv.g + feed * (1.0 - uv.r);
	float dv = 0.4*lapl.g + coup * uv.r * uv.g * uv.g - (feed + kill) * uv.g;

	if (!paused) { 
		color = uv + delta * vec4(du,dv,0,0);
	}
	else { 
		color = uv;
	}
	if (length(brush - npos) < brushrad) { 
		color = vec4(0.0,delete ? 0.0 : 1.0,0,0);
	}
}