#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

uniform int pLevel;

uniform mediump sampler2DArray texColor;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

layout(location=0) out vec3 outColor;

void main( void ){
	ivec3 tc = ivec3( gl_FragCoord.xy, vLayer );
	tc.xy *= ivec2( 2 );
	
	outColor = ( vec3( texelFetch( texColor, tc, pLevel ) )
		+ vec3( texelFetchOffset( texColor, tc, pLevel, ivec2( 1, 0 ) ) )
		+ vec3( texelFetchOffset( texColor, tc, pLevel, ivec2( 0, 1 ) ) )
		+ vec3( texelFetchOffset( texColor, tc, pLevel, ivec2( 1, 1 ) ) ) ) / vec3( 4 );
}
