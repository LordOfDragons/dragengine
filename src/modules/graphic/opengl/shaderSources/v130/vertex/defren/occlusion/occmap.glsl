precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"

#ifdef WITH_SHADOWMAP
	#include "v130/shared/defren/ubo_render_parameters.glsl"
	
#else
	UBOLAYOUT uniform RenderParameters{
		mat4 pMatrixVP[ 6 ];
		mat4x3 pMatrixV[ 6 ];
		vec4 pTransformZ[ 6 ];
		vec2 pZToDepth;
		vec4 pClipPlane[ 2 ]; // normal.xyz, distance
	};
#endif

#include "v130/shared/defren/occmap.glsl"

#ifdef SHARED_SPB
	#include "v130/shared/defren/skin/shared_spb_index.glsl"
	#define pMatrixModel pSharedSPB[ spbIndex ].pSPBMatrixModel
#endif

in vec3 inPosition;

#if defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED || defined GS_RENDER_STEREO
	#ifdef SHARED_SPB
		flat out int vGSSPBIndex;
		#define vSPBIndex vGSSPBIndex
		
		#if defined GS_RENDER_CUBE && defined GS_RENDER_CUBE_CULLING
			flat out int vGSSPBFlags;
		#endif
	#endif
	
#else
	#ifdef PERSPECTIVE_TO_LINEAR
		out float vDepth;
	#endif
	#ifdef DEPTH_DISTANCE
		out vec3 vPosition;
	#endif
	#ifdef USE_CLIP_PLANE
		out vec3 vClipCoord;
	#endif
	
	#ifdef SHARED_SPB
		flat out int vSPBIndex;
	#endif
#endif

#include "v130/shared/defren/sanitize_position.glsl"

void main( void ){
	#include "v130/shared/defren/skin/shared_spb_index2.glsl"
	
	vec4 position = vec4( pMatrixModel * vec4( inPosition, 1 ), 1 );
	
	#if defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED || defined GS_RENDER_STEREO
		gl_Position = sanitizePosition( position );
		
	#else
		gl_Position = sanitizePosition( pMatrixVP[ 0 ] * position );
		#ifdef PERSPECTIVE_TO_LINEAR
			vDepth = dot( pTransformZ[ 0 ], position );
		#endif
		#ifdef DEPTH_DISTANCE
			vPosition = pMatrixV[ 0 ] * position;
		#endif
		#ifdef USE_CLIP_PLANE
			vClipCoord = pMatrixV[ 0 ] * position;
		#endif
	#endif
	
	#ifdef SHARED_SPB
		vSPBIndex = spbIndex;
		#if defined GS_RENDER_CUBE && defined GS_RENDER_CUBE_CULLING
			vGSSPBFlags = spbFlags;
		#endif
	#endif
}
