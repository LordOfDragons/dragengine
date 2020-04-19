precision mediump float;
precision mediump int;

uniform int pLevel;

uniform mediump sampler2D texSource;

void main( void ){
	ivec2 tc = ivec2( gl_FragCoord.xy ) * ivec2( 2 );
	
	vec4 samples;
	
	samples.x = float( texelFetch( texSource, tc, pLevel ) );
	samples.y = float( texelFetchOffset( texSource, tc, pLevel, ivec2( 1, 0 ) ) );
	samples.z = float( texelFetchOffset( texSource, tc, pLevel, ivec2( 0, 1 ) ) );
	samples.w = float( texelFetchOffset( texSource, tc, pLevel, ivec2( 1, 1 ) ) );
	
	samples.xy = max( samples.xy, samples.zw );
	gl_FragDepth = max( samples.x, samples.y );
}
