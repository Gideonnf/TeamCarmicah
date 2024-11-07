#version 460 core

layout (location=0) in vec2 aVertexPosition;
layout (location=1) in vec2 aTextureCoord;
layout (location=2) in float aDepth;
layout (location=3) in uvec2 aID;

layout (location=0) out vec2 vTexCoord;
layout (location=1) flat out uvec2 vID;

uniform mat3	uNDC_to_Cam;


void main(void){
	gl_Position	= vec4(vec2(uNDC_to_Cam * vec3(aVertexPosition, 1.0)), aDepth, 1.0);
	vTexCoord	= vec2(aTextureCoord.x, 1.0 - aTextureCoord.y);
	vID = aID;
}