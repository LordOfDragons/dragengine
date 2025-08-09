#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(3) uniform vec4 pGamma;
UNIFORM_BIND(4) uniform vec4 pColor;

layout(binding=0) uniform mediump sampler2D texColor;

#include "shared/interface/2d/fragment.glsl"

layout(location=0) out mediump vec4 outColor;

void main( void ){
	outColor = pow( texture( texColor, vTexCoord ), pGamma ) * pColor;
}
