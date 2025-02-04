#version 460 core

layout (location=0)		out vec4			fFragColor; // location 0 is default GL_BACK_LEFT color buffer
layout (location=1)		flat in uvec2		vID;

layout (binding = 0) uniform sampler2D 	uAccum;
layout (binding = 1) uniform sampler2D 	uReveal;

const float EPSILON = 0.00001f;

bool isAproxEql(float a, float b)
{
	return abs(a - b) <= (abs(a) < abs(b) ? abs(b) : abs(a)) * EPSILON;
}
float max3(vec3 v)
{
	return max(max(v.x, v.y), v.z);
}

void main(void){
	ivec2 texCoord = ivec2(gl_FragCoord.xy);

	float revealage = texelFetch(uReveal, texCoord, 0).r;

	if(isAproxEql(revealage, 1.0f))
		discard;

	vec4 accmulation = texelFetch(uAccum, texCoord, 0);

	if(isinf(max3(abs(accmulation.rgb))))
		accmulation.rgb = vec3(accmulation.a);

	vec3 avgCol = accmulation.rgb / max(accmulation.a, EPSILON);
	fFragColor = vec4(avgCol, 1.0f - revealage);
}
