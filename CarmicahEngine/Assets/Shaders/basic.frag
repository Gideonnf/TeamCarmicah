#version 460 core

layout (location=0)		in vec2				vTexCoord;
layout (location=1)		flat in uvec2		vID;

layout (location=0)		out vec4			fFragColor; // location 0 is default GL_BACK_LEFT color buffer
layout (location=1)		out unsigned int	fGID;
layout (location=2)		out vec4			fAccum;
layout (location=3)		out float			fReveal;

layout (binding = 0) uniform sampler2DArray 	uTex;

uniform int		uPassNum;	

void main(void){
	vec4 col = texture(uTex, vec3(vTexCoord, vID.y) );

	if(uPassNum == 0)// Solid Pass
	{
		if(col.a < 0.8f)
			discard;

		fFragColor = col;
		fGID = vID.x;
	}
	else if(uPassNum == 1)// Transparent Pass
	{
		if(col.a >= 0.8f)
			discard;
		//if(col.a > 0.500001)
		//	fGID = vID.x;
		

		float weight = clamp(pow(min(1.0, col.a * 10.0) + 0.01, 3.0) * 1e8 * pow(1.0 - gl_FragCoord.z * 0.9, 3.0), 1e-2, 3e3);
		fAccum = vec4(col.rgb * col.a, col.a) * weight;
		fReveal = col.a;
	}

}
