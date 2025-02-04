#version 460 core

layout (location=0)		in vec2				vTexCoord;
layout (location=1)		flat in uvec2		vID;

layout (location=0)		out vec4			fFragColor; // location 0 is default GL_BACK_LEFT color buffer
layout (location=1)		out unsigned int	fGID;

layout (binding = 0) uniform sampler2DArray 	uTex;

void main(void){
	fFragColor = texture(uTex, vec3(vTexCoord, vID.y) );

	// Simple backup Blend test
	if(fFragColor.a < 0.500001)
		discard;

	fGID = vID.x;
}
