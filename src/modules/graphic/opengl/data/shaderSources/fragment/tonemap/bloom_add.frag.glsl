#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

layout(binding=0) uniform mediump sampler2DArray texColor;

uniform float pLevel;

#include "shared/interface/2d_fragment.glsl"

layout(location=0) out vec3 outColor;

void main( void ){
	outColor = textureLod( texColor, vec3( vTexCoord, vLayer ), pLevel ).rgb;
}
