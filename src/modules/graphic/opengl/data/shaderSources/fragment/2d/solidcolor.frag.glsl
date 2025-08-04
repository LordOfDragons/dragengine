#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(1) uniform vec4 pColor;

layout(location=0) out vec4 outColor;

#include "shared/interface/2d_fragment.glsl"

void main( void ){
	outColor = pColor;
}
