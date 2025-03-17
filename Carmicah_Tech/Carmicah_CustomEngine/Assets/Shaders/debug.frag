#version 460 core

layout (location=0)	flat in unsigned int	vID;

layout (location=0) out vec4 fFragColor; // location 0 is default GL_BACK_LEFT color buffer
layout (location=1)	out unsigned int	fGID;

void main(void){
	fFragColor = vec4(0.0, 0.0, 0.0, 1.0);
	fGID = vID;
}
