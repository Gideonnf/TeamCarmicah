#version 460 core

layout (location=0) in vec2 aVertexPosition;

void main(void){
	gl_Position	= vec4(aVertexPosition, 0.0, 1.0);
}