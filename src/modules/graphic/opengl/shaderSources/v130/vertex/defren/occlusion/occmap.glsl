precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"

#ifdef WITH_SHADOWMAP
	#include "v130/shared/defren/skin/ubo_render_parameters.glsl"
	
	#if defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED
		#define MATRIX_VP_0 pMatrixVP[0]
		#define MATRIX_V_0 pMatrixV[0]
		#define TRANSFORM_Z_0 pTransformZ[0]
	#else
		#define MATRIX_VP_0 pMatrixVP
		#define MATRIX_V_0 pMatrixV
		#define TRANSFORM_Z_0 pTransformZ
	#endif
	
#else
	UBOLAYOUT uniform RenderParameters{
		mat4 pMatrixVP[ 6 ];
		mat4x3 pMatrixV[ 6 ];
		vec4 pTransformZ[ 6 ];
		vec2 pZToDepth;
		vec4 pClipPlane; // normal.xyz, distance
	};
	#define MATRIX_VP_0 pMatrixVP[0]
	#define MATRIX_V_0 pMatrixV[0]
	#define TRANSFORM_Z_0 pTransformZ[0]
#endif

#include "v130/shared/defren/occmap.glsl"

#ifdef SHARED_SPB
	#include "v130/shared/defren/skin/shared_spb_index.glsl"
	#define pMatrixModel pSharedSPB[ spbIndex ].pSPBMatrixModel
#endif

in vec3 inPosition;

#if defined PERSPECTIVE_TO_LINEAR && ! defined GS_RENDER_CUBE && ! defined GS_RENDER_CASCADED
out float vDepth;
#endif
#ifdef DEPTH_DISTANCE
out vec3 vPosition;
#endif
#ifdef USE_CLIP_PLANE
out vec3 vClipCoord;
#endif

#if defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED
	flat out int vSPBIndex;
#endif
#if defined GS_RENDER_CUBE && defined GS_RENDER_CUBE_CULLING
	flat out int vSPBFlags;
#endif

void main( void ){
	#include "v130/shared/defren/skin/shared_spb_index2.glsl"
	
	vec4 position = vec4( pMatrixModel * vec4( inPosition, 1.0 ), 1.0 );
	#if defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED
		gl_Position = position;
	#else
		gl_Position = MATRIX_VP_0 * position;
		#ifdef PERSPECTIVE_TO_LINEAR
			vDepth = dot( TRANSFORM_Z_0, position );
		#endif
		#ifdef DEPTH_DISTANCE
			vPosition = MATRIX_V_0 * position;
		#endif
	#endif
	
	#if defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED
		vSPBIndex = spbIndex;
	#endif
	#if defined GS_RENDER_CUBE && defined GS_RENDER_CUBE_CULLING
		vSPBFlags = spbFlags;
	#endif
	
	#ifdef USE_CLIP_PLANE
		vClipCoord = MATRIX_V_0 * position;
	#endif
}
