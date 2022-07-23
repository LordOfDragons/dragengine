precision highp float;
precision highp int;

#if defined WITH_TEXTURE || defined WITH_RENDER_WORLD
#define REQUIRES_TEXTURE 1
#endif

uniform mat3x2 pTransform;
#ifdef REQUIRES_TEXTURE
uniform mat3x2 pTCTransform;
#endif
#ifdef WITH_MASK
uniform mat3x2 pTCTransformMask;
#endif

in vec2 inPosition;

#ifdef REQUIRES_TEXTURE
out vec2 vTexCoord;
#endif
#ifdef WITH_MASK
out vec2 vTexCoordMask;
#endif

void main( void ){
	gl_Position = vec4( vec2( pTransform * vec3( inPosition, 1.0 ) ), 0.0, 1.0 );
	
	#ifdef REQUIRES_TEXTURE
	vTexCoord = vec2( pTCTransform * vec3( inPosition, 1.0 ) );
	#endif
	#ifdef WITH_MASK
	vTexCoordMask = vec2( pTCTransformMask * vec3( gl_Position.xy, 1.0 ) );
	#endif
}
