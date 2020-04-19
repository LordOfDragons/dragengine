precision highp float;
precision highp int;

uniform mat3x2 pTransform;
#ifdef WITH_TEXTURE
uniform mat3x2 pTCTransform;
#endif

in vec2 inPosition;

#ifdef WITH_TEXTURE
out vec2 vTexCoord;
#endif

void main( void ){
	gl_Position = vec4( vec2( pTransform * vec3( inPosition, 1.0 ) ), 0.0, 1.0 );
	
	#ifdef WITH_TEXTURE
	vTexCoord = vec2( pTCTransform * vec3( inPosition, 1.0 ) );
	#endif
}
