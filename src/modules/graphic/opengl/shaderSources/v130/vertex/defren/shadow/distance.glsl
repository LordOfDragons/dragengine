precision highp float;
precision highp int;

#if defined( TRANSPARENT ) || defined( DISCARD_HOLES )
#define _USE_TEXTURE_
#endif

uniform mat4 pMatrixMVP; // matrix model-view-projection
uniform mat4x3 pMatrixMV; // matrix model-view
#ifdef _USE_TEXTURE_
uniform mat3x2 pMatrixDiffuse; // matrix for diffuse texture
#endif

in vec3 inPosition;
#ifdef _USE_TEXTURE_
in vec2 inTexCoord;
#endif
#ifdef HEIGHTTERRAIN
in float inHTHeight;
#endif

#ifdef _USE_TEXTURE_
out vec2 vTexCoord;
#endif
out vec3 vLSPosition;

void main( void ){
#ifdef HEIGHTTERRAIN
	vec4 position = vec4( inPosition.x, inHTHeight, inPosition.y, 1.0 );
#else // HEIGHTTERRAIN
	vec4 position = vec4( inPosition.xyz, 1.0 );
#endif // HEIGHTTERRAIN
	
	vLSPosition = pMatrixMV * position;
	
#ifdef _USE_TEXTURE_
	vTexCoord = pMatrixDiffuse * vec3( inTexCoord, 1.0 );
#endif
	
	gl_Position = pMatrixMVP * position;
}
