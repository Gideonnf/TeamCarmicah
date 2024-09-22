#version 460 core

layout (location=0) in vec2 vTexCoord;

layout (location=0) out vec4 fFragColor; // location 0 is default GL_BACK_LEFT color buffer

uniform bool		isDebug;
uniform sampler2D	uTex2d;

void main(void){
	if(isDebug)
	{
		fFragColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else
	{
		vec4 tex = texture(uTex2d, vTexCoord);
		fFragColor = tex;
	}
}
