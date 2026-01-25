#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

VARYING_BIND(0) in vec3 vColor;

layout(location=0) out vec4 outColor;

void main( void ){
	outColor = vec4(vColor, 1);
}
