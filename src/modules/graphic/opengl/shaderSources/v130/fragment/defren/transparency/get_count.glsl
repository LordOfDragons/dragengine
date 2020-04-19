precision lowp float;
precision lowp int;

uniform lowp sampler2D texValues;

in lowp float vThreshold;

out lowp float outDummy;

//const vec2 TC = vec2( 0.0, 1.0 );

void main( void ){
	//if( texture( texValues, vec2( 0.0 ) ).r < vThreshold ) discard;
	if( texelFetch( texValues, ivec2( 0, 0 ), 0 ).r < vThreshold ) discard;
	outDummy = 1.0;
}
