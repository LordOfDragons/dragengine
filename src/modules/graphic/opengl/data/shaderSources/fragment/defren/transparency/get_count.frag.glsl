#include "shared/preamble.glsl"

precision lowp float;
precision lowp int;

uniform lowp sampler2DArray texValues;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	VARYING_BIND(2) flat in int vLayer;
#else
	const int vLayer = 0;
#endif

layout(location=0) out lowp float outDummy;

void main( void ){
	// the 0.1 more than 255 is only use to ensure rounding
	if( int( texelFetch( texValues, ivec3( 0, 0, vLayer ), 0 ).r * 255.1 ) <= int( gl_FragCoord.x ) ){
		discard;
	}
	outDummy = 1.0;
}
