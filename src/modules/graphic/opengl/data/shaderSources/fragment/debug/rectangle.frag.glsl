#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

flat in vec4 vColor;

layout(location=0) out vec4 outColor;

void main( void ){
	outColor = vColor;
}
