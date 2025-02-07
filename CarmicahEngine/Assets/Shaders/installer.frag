#version 460 core

layout (location=0) in vec2 vTexCoord;

layout (location=0)	out vec4 fFragColor; // location 0 is default GL_BACK_LEFT color buffer

uniform sampler2D screen;

void main(void){
	fFragColor = vec4(texture(screen, vTexCoord).rgb, 1.0f);
}
