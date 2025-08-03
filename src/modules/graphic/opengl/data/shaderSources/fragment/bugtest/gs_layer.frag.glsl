#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

layout(location=0) in vec3 vColor;

layout(location=0) out vec3 outColor;

void main( void ){
	outColor = vColor;
}
