#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

uniform mediump sampler2DArray texColor;

uniform float pLevel;

in vec2 vTexCoord;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

layout(location=0) out vec3 outColor;

void main( void ){
	outColor = textureLod( texColor, vec3( vTexCoord, vLayer ), pLevel ).rgb;
}
