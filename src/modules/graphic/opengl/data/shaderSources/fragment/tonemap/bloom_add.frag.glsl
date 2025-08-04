#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

uniform mediump sampler2DArray texColor;

uniform float pLevel;

VARYING_BIND(0) in vec2 vTexCoord;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	VARYING_BIND(2) flat in int vLayer;
#else
	const int vLayer = 0;
#endif

layout(location=0) out vec3 outColor;

void main( void ){
	outColor = textureLod( texColor, vec3( vTexCoord, vLayer ), pLevel ).rgb;
}
