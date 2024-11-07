#version 460 core

layout (location=0) in vec2 aVertexPosition;
layout (location=1) in float aDepth;

uniform mat3	uNDC_to_Cam;

void main(void){
	gl_Position		= vec4(vec2(uNDC_to_Cam * vec3(aVertexPosition, 1.0)), aDepth, 1.0);
}