precision highp float;
precision highp int;

uniform mat4x4 pMatrixMVP; // matrix model-view-projection
#ifdef WITH_SELECTOR
	uniform mat4x4 pMatrixMVP2;
#endif

in vec3 inPosition;
#ifdef WITH_SELECTOR
	in float inSelector;
#endif

void main( void ){
	#ifdef WITH_SELECTOR
		/*
		vec3 scale = mix( pSelFalseScale, pSelTrueScale, bvec3( inSelector ) );
		vec3 offset = mix( pSelFalseOffset, pSelTrueOffset, bvec3( inSelector ) );
		
		vec3 position = inPosition * scale + offset;
		
		gl_Position = pMatrixMVP * vec4( position, 1.0 );
		*/
		if( bool( inSelector ) ){
			gl_Position = pMatrixMVP2 * vec4( inPosition, 1.0 );
		}else{
			gl_Position = pMatrixMVP * vec4( inPosition, 1.0 );
		}
	#else
		gl_Position = pMatrixMVP * vec4( inPosition, 1.0 );
	#endif
}
