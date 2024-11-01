#version 460 core

layout (location=0) in vec2 aVertexPosition;
layout (location=1) in vec2 aTextureCoord;

layout (location=0) out vec2 vTexCoord;

uniform float	uDepth;
uniform mat3	uModel_to_NDC;
uniform mat3	uAnimationMult;


void main(void){
	gl_Position	= vec4(vec2(uModel_to_NDC * vec3(aVertexPosition, 1.0)), uDepth, 1.0);
	vec2 tt		= vec2(uAnimationMult * vec3(aTextureCoord, 1.0));
	vTexCoord	= vec2(tt.x, -tt.y);
}