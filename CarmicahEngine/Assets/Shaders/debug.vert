#version 460 core

layout (location=0) in vec2 aVertexPosition;

uniform float	uDepth;
uniform mat3	uModel_to_NDC;

void main(void){
	gl_Position		= vec4(vec2(uModel_to_NDC * vec3(aVertexPosition, 1.0)), uDepth, 1.0);
}