#version 460 core

layout (location=0) in vec2 aVertexPosition;
layout (location=1) in vec2 aTextureCoord;
layout (location=2) in vec4 aColor;
layout (location=3) in float aDepth;
layout (location=4) in uvec2 aID;

layout (location=0) out vec2 vTexCoord;
layout (location=1) flat out uvec2 vID;

uniform mat3	uNDC_to_Cam;
uniform vec2	uFontDisplace;
uniform vec2	uFontScale;

void main(void){
	vec2 calcPos = uFontDisplace + vec2(aVertexPosition.x * uFontScale.x, aVertexPosition.y * uFontScale.y);
	gl_Position	= vec4(vec2(uNDC_to_Cam * vec3(calcPos, 1.0)), aDepth, 1.0);
	vTexCoord	= vec2(aTextureCoord.x, -aTextureCoord.y);
	vID = aID;
}