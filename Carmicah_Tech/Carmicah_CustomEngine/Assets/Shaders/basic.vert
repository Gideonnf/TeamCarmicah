#version 460 core

layout (location=0) in vec2 aVertexPosition;
layout (location=1) in vec2 aTextureCoord;
layout (location=2) in vec4 aColor;
layout (location=3) in float aDepth;
layout (location=4) in uvec2 aID;

layout (location=0) out vec2 vTexCoord;
layout (location=1) flat out uvec2 vID;
layout (location=2) out vec4 vColor;

uniform mat3	uNDC_to_Cam;
uniform bool	uIsText;
uniform vec2	uTextOffset;
uniform vec2	uTextScale;


void main(void){
	if(uIsText)
	{
		vec2 calcPos = uTextOffset + vec2(aVertexPosition.x * uTextScale.x, aVertexPosition.y * uTextScale.y);
		gl_Position	= vec4(vec2(uNDC_to_Cam * vec3(calcPos, 1.0)), aDepth, 1.0);
	}
	else
		gl_Position	= vec4(vec2(uNDC_to_Cam * vec3(aVertexPosition, 1.0)), aDepth, 1.0);

	vTexCoord	= vec2(aTextureCoord.x, -aTextureCoord.y);
	vID = aID;
	vColor = aColor;
}