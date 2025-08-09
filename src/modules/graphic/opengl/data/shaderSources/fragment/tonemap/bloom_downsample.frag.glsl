#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(3) uniform int pLevel;

layout(binding=0) uniform mediump sampler2DArray texColor;

#include "shared/interface/2d/fragment.glsl"

layout(location=0) out vec3 outColor;

void main( void ){
	ivec3 tc = ivec3( gl_FragCoord.xy, vLayer );
	tc.xy *= ivec2( 2 );
	
	outColor = ( vec3( texelFetch( texColor, tc, pLevel ) )
		+ vec3( texelFetchOffset( texColor, tc, pLevel, ivec2( 1, 0 ) ) )
		+ vec3( texelFetchOffset( texColor, tc, pLevel, ivec2( 0, 1 ) ) )
		+ vec3( texelFetchOffset( texColor, tc, pLevel, ivec2( 1, 1 ) ) ) ) / vec3( 4 );
}
