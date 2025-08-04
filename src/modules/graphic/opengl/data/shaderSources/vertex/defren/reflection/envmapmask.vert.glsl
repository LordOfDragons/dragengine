#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(0) uniform mat4x3 pMatrixMV;
UNIFORM_BIND(1) uniform mat4 pMatrixP;

layout(location=0) in vec3 inPosition;

out vec3 vEnvMapDir;



// Main Function
//////////////////

void main( void ){
	if(FullScreenQuad){
		gl_Position = vec4(vec2(inPosition), 0, 1);
		vEnvMapDir = vec3(vec2(inPosition), 1);
		
	}else{
		vEnvMapDir = pMatrixMV * vec4(inPosition, 1);
		gl_Position = pMatrixP * vec4(vEnvMapDir, 1);
	}
}
