#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(3) uniform vec4 pGamma;
UNIFORM_BIND(4) uniform vec4 pColor;

uniform mediump sampler2D texColor;

VARYING_BIND(0) in vec2 vTexCoord;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	VARYING_BIND(1) flat in int vLayer;
#else
	const int vLayer = 0;
#endif

layout(location=0) out mediump vec4 outColor;

void main( void ){
	outColor = pow( texture( texColor, vTexCoord ), pGamma ) * pColor;
}
