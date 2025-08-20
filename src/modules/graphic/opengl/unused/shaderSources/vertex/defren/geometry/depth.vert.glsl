#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(0) uniform mat4 pMatrixMVP;

// UseClipPlane
UNIFORM_BIND(1) uniform mat4x3 pMatrixMV;

#if defined( IGNORE_HOLES ) || defined( CLIP_DEPTH )
UNIFORM_BIND(2) uniform mat3x2 pMatrixDiffuse;
#endif

// GeometryMode
UNIFORM_BIND(3) uniform mat4 pPFMatrix;

layout(location=0) in vec3 inPosition;
#if defined( IGNORE_HOLES ) || defined( CLIP_DEPTH )
layout(location=3) in vec2 inTexCoord;
#endif

// GeometryMode == GeometryModeHeightMap
layout(location=4) in float inHTHeight;

#if defined( IGNORE_HOLES ) || defined( CLIP_DEPTH )
VARYING_BIND(0) out vec2 vTexCoord;
#endif

// UseClipPlane
VARYING_BIND(1) out vec3 vClipCoord;

#include "shared/defren/sanitize_position.glsl"

void main( void ){
#if defined( IGNORE_HOLES ) || defined( CLIP_DEPTH )
	vTexCoord = pMatrixDiffuse * vec3( inTexCoord, 1.0 );
#endif
	
	if(AnyKindPropField){
		float bfac = clamp( pPFMatrix[ 3 ][ 3 ] * length( inPosition ), 0, 1 );
		float s = pPFMatrix[ 0 ][ 3 ];
		float bx = pPFMatrix[ 1 ][ 3 ] * bfac;
		float bz = pPFMatrix[ 2 ][ 3 ] * bfac;
		
		vec4 bs = sin( vec4( bx, bz, -bx, -bz ) );
		vec2 bc = cos( vec2( bx, bz ) );
		mat3 mb = mat3( bc.y, bs.y, 0.0, bc.x * bs.w, bc.x * bc.y, bs.x, bs.x * bs.y, bs.z * bc.y, bc.x );
		
		// out = Mmvp * vec4( Mb * ( Mr * ( in * vec3( s ) ) ) + T, 1 )
		// problem: sanitizePosition has to come after MatrixM but before MatrixVP
		gl_Position = pMatrixMVP * vec4( mb * ( mat3( pPFMatrix ) * ( inPosition * vec3( s ) ) ) + vec3( pPFMatrix[ 3 ] ), 1.0 );
		
	}else{
		if(GeometryMode == GeometryModeHeightMap){
			gl_Position = pMatrixMVP * vec4( inPosition.x, inHTHeight, inPosition.y, 1 );
			
		}else{
			gl_Position = pMatrixMVP * vec4( inPosition.xyz, 1 );
		}
	}
	
	if(UseClipPlane){
		if(GeometryMode == GeometryModeHeightMap){
			vClipCoord = pMatrixMV * vec4( inPosition.x, inHTHeight, inPosition.y, 1 );
			
		}else{
			vClipCoord = pMatrixMV * vec4( inPosition, 1 );
		}
	}
}
