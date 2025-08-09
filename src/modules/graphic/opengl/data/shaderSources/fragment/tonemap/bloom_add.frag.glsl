#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

layout(binding=0) uniform mediump sampler2DArray texColor;

UNIFORM_BIND(3) uniform float pLevel;

#include "shared/interface/2d/fragment.glsl"

layout(location=0) out vec3 outColor;

void main( void ){
	outColor = textureLod( texColor, vec3( vTexCoord, vLayer ), pLevel ).rgb;
}
