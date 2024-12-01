#version 460 core

layout (location=0)		out vec4			fFragColor; // location 0 is default GL_BACK_LEFT color buffer
layout (location=1)		out unsigned int	fGID;

uniform unsigned int	uID;
uniform vec4			uColor;

void main(void){
	fFragColor = uColor;
	fGID = uID;
}
