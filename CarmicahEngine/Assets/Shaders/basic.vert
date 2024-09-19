#version 460 core

layout (location=0) in vec2 uVertexPosition;
layout (location=1) in vec3 uVertexColor;
layout (location=2) in vec2 uTextureCoord;

layout (location=0) out vec3 vColor;
layout (location=1) out vec2 vTexCoord;

uniform mat3	uModel_to_NDC;

void main(void){
	gl_Position		= vec4(vec2(uModel_to_NDC * vec3(uVertexPosition, 1.0)), 0.0, 1.0);
	//gl_Position		= vec4(uVertexPosition, 0.0, 1.0);
	vColor			= uVertexColor;
	vTexCoord		= uTextureCoord;
}
