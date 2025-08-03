#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

uniform int pLevel;

uniform HIGHP sampler2DArray texSource;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

void main( void ){
	ivec3 tc = ivec3( gl_FragCoord.xy, vLayer );
	tc.xy *= ivec2( 2 );
	
	vec4 samples;
	
	samples.x = float( texelFetch( texSource, tc, pLevel ) );
	samples.y = float( texelFetchOffset( texSource, tc, pLevel, ivec2( 1, 0 ) ) );
	samples.z = float( texelFetchOffset( texSource, tc, pLevel, ivec2( 0, 1 ) ) );
	samples.w = float( texelFetchOffset( texSource, tc, pLevel, ivec2( 1, 1 ) ) );
	
	samples.xy = max( samples.xy, samples.zw );
	gl_FragDepth = max( samples.x, samples.y );
}
