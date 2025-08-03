#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(0) uniform vec4 pColor;

layout(location=0) out vec4 outColor;

void main( void ){
	outColor = pColor;
}
