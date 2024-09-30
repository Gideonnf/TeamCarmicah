#version 460 core

layout (location=0) in vec2 vTexCoord;

layout (location=0) out vec4 fFragColor;

uniform sampler2D uTex2d;
uniform vec3 uTextColor;

void main(void){
	fFragColor = vec4(uTextColor, texture(uTex2d, vTexCoord).r);
}
