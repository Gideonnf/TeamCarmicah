#version 460 core

layout (location=0) in vec2 vTexCoord;

layout (location=0) out vec4 fFragColor; // location 0 is default GL_BACK_LEFT color buffer

uniform sampler2D	uTex2d;

void main(void){
	fFragColor = texture(uTex2d, vTexCoord);
}
