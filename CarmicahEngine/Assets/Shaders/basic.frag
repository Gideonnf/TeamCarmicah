#version 460 core

layout (location=0) in vec2 vTexCoord;

layout (location=0) out vec4 fFragColor; // location 0 is default GL_BACK_LEFT color buffer
layout (location=1) out unsigned int fGID;

uniform sampler2D		uTex2d;
uniform unsigned int	uID;

void main(void){
	fFragColor = texture(uTex2d, vTexCoord);
	fGID = uID;
}
