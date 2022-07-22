precision lowp float;
precision lowp int;

uniform lowp sampler2DArray texValues;

in lowp float vThreshold;

#ifdef GS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

out lowp float outDummy;

//const vec2 TC = vec2( 0.0, 1.0 );

void main( void ){
	//if( texture( texValues, vec2( 0.0 ) ).r < vThreshold ) discard;
	if( texelFetch( texValues, ivec3( 0, 0, vLayer ), 0 ).r < vThreshold ) discard;
	outDummy = 1.0;
}
