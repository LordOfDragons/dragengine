#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(3) uniform vec4 pGamma;
UNIFORM_BIND(4) uniform vec4 pColor;

uniform mediump sampler2D texColor;

#include "shared/interface/2d_fragment.glsl"

layout(location=0) out mediump vec4 outColor;

void main( void ){
	outColor = pow( texture( texColor, vTexCoord ), pGamma ) * pColor;
}
