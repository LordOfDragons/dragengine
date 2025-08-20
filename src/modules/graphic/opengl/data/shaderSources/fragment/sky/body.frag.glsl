#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(1) uniform vec4 pColor; // TODO is now split into color/transparency...
UNIFORM_BIND(2) uniform vec4 pMaterialGamma;

layout(binding=0) uniform lowp sampler2D texColor;
layout(binding=1) uniform lowp sampler2D texTransparency;

#include "shared/interface/2d/fragment.glsl"

layout(location=0) out vec4 outColor;

void main( void ){
	outColor.rgb = texture( texColor, vTexCoord ).rgb;
	outColor.a = texture( texTransparency, vTexCoord ).r;
	outColor = pow( outColor, pMaterialGamma ) * pColor;
}
