#version 460 core

layout (location=0) in vec2 aVertexPosition;

uniform mat3	uMtx;

void main(void){
	vec3 calcPos = uMtx * vec3(aVertexPosition, 1.0);

	gl_Position	= vec4(calcPos.xy, -1.0, 1.0);
}