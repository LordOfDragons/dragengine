//#version 130
//@@DEFINES@@

precision highp float;
precision highp int;

uniform mat4 pMatrixMVP;
#ifdef USE_CLIP_PLANE
uniform mat4x3 pMatrixMV;
#endif
#if defined( IGNORE_HOLES ) || defined( CLIP_DEPTH )
uniform mat3x2 pMatrixDiffuse;
#endif
#ifdef PROP_FIELD
uniform mat4 pPFMatrix;
#endif

in vec3 inPosition;
#if defined( IGNORE_HOLES ) || defined( CLIP_DEPTH )
in vec2 inTexCoord;
#endif
#ifdef HEIGHTTERRAIN
in float inHTHeight;
#endif

#if defined( IGNORE_HOLES ) || defined( CLIP_DEPTH )
out vec2 vTexCoord;
#endif
#ifdef USE_CLIP_PLANE
out vec3 vClipCoord;
#endif

#include "v130/shared/defren/sanitize_position.glsl"

void main( void ){
#if defined( IGNORE_HOLES ) || defined( CLIP_DEPTH )
	vTexCoord = pMatrixDiffuse * vec3( inTexCoord, 1.0 );
#endif
	
#ifdef PROP_FIELD
	float bfac = clamp( pPFMatrix[ 3 ][ 3 ] * length( inPosition ), 0.0, 1.0 );
	float s = pPFMatrix[ 0 ][ 3 ];
	float bx = pPFMatrix[ 1 ][ 3 ] * bfac;
	float bz = pPFMatrix[ 2 ][ 3 ] * bfac;
	
	vec4 bs = sin( vec4( bx, bz, -bx, -bz ) );
	vec2 bc = cos( vec2( bx, bz ) );
	mat3 mb = mat3( bc.y, bs.y, 0.0, bc.x * bs.w, bc.x * bc.y, bs.x, bs.x * bs.y, bs.z * bc.y, bc.x );
	
	// out = Mmvp * vec4( Mb * ( Mr * ( in * vec3( s ) ) ) + T, 1 )
	gl_Position = sanitizePosition( pMatrixMVP * vec4( mb * ( mat3( pPFMatrix )
		* ( inPosition * vec3( s ) ) ) + vec3( pPFMatrix[ 3 ] ), 1.0 ) );
	
#else // PROP_FIELD
#ifdef HEIGHTTERRAIN
	gl_Position = sanitizePosition( pMatrixMVP * vec4( inPosition.x, inHTHeight, inPosition.y, 1.0 ) );
#else // HEIGHTTERRAIN
	gl_Position = sanitizePosition( pMatrixMVP * vec4( inPosition.xyz, 1.0 ) );
#endif // HEIGHTTERRAIN
#endif // PROP_FIELD
	
#ifdef USE_CLIP_PLANE
#ifdef HEIGHTTERRAIN
	vClipCoord = pMatrixMV * vec4( inPosition.x, inHTHeight, inPosition.y, 1.0 );
#else // HEIGHTTERRAIN
	vClipCoord = pMatrixMV * vec4( inPosition, 1.0 );
#endif // HEIGHTTERRAIN
#endif
}
