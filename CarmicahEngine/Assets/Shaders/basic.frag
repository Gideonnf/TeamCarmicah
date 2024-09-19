#version 460 core

layout (location=0) in vec3 vInterpolatedColor;
layout (location=1) in vec2 vTexCoord;

layout (location=0) out vec4 fFragColor; // location 0 is default GL_BACK_LEFT color buffer

uniform sampler2D	uTex2d;

void main(void){
	
	fFragColor = vec4(vInterpolatedColor, 1.0);

	//fFragColor = vec4(texture(uTex2d, vTexCoord).rgb * vInterpolatedColor, fFragColor.a);
}
