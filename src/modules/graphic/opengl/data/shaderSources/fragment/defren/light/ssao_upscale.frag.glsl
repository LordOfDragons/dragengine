#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

uniform lowp sampler2DArray texAO;

VARYING_BIND(0) in vec2 vTexCoord;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	VARYING_BIND(2) flat in int vLayer;
#else
	const int vLayer = 0;
#endif

layout(location=0) out vec3 outAO; // ao, ssao, solidity


void main( void ){
	outAO = vec3( textureLod( texAO, vec3( vTexCoord, vLayer ), 0.0 ) );
}
