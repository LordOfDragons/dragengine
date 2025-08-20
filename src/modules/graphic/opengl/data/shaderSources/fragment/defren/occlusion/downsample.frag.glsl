#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(0) uniform int pLevel;

layout(binding=0) uniform HIGHP sampler2DArray texSource;

#include "shared/interface/2d/fragment.glsl"

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
