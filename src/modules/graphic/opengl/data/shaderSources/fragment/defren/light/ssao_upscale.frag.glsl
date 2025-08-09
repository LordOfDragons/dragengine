#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

layout(binding=0) uniform lowp sampler2DArray texAO;

#include "shared/interface/2d/fragment.glsl"

layout(location=0) out vec3 outAO; // ao, ssao, solidity

void main( void ){
	outAO = vec3( textureLod( texAO, vec3( vTexCoord, vLayer ), 0.0 ) );
}
