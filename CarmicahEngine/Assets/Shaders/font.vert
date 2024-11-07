#version 460 core

layout (location=0) in vec2 aVertexPosition;
layout (location=1) in vec2 aTextureCoord;

layout (location=0) out vec2 vTexCoord;

uniform float	uDepth;
uniform mat3	uModel_to_NDC;

void main(void){
	gl_Position		= vec4(vec2(uModel_to_NDC * vec3(aVertexPosition, 1.0)), uDepth, 1.0);
	vTexCoord		= vec2(aTextureCoord.x, 1.0 - aTextureCoord.y);
}