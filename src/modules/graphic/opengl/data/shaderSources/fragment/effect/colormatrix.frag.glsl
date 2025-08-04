#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(3) uniform mat4 pColorMatrix;
UNIFORM_BIND(4) uniform vec4 pColorOffset;

uniform mediump sampler2DArray texColor;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	VARYING_BIND(0) flat in int vLayer;
#else
	const int vLayer = 0;
#endif

layout(location=0) out mediump vec4 outColor;

void main( void ){
	outColor = pColorMatrix * texelFetch( texColor, ivec3( gl_FragCoord.xy, vLayer ), 0 ) + pColorOffset;
}
