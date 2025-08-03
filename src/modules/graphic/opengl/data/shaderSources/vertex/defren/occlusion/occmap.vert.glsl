#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"

#ifdef WITH_SHADOWMAP
	#include "shared/defren/ubo_render_parameters.glsl"
	const vec4 pTransformZ[6] = vec4[6](vec4(0.0), vec4(0.0), vec4(0.0), vec4(0.0), vec4(0.0), vec4(0.0));
	
#else
	UBOLAYOUT_BIND(0) uniform RenderParameters{
		mat4 pMatrixVP[6];
		mat4x3 pMatrixV[6];
		vec4 pTransformZ[6];
		vec2 pZToDepth;
		vec4 pClipPlane[2]; // normal.xyz, distance
	};
	const vec4 pDepthOffset[4] = vec4[4](vec4(0.0), vec4(0.0), vec4(0.0), vec4(0.0));
#endif

#include "shared/defren/occmap.glsl"

#ifdef SHARED_SPB
	#include "shared/defren/skin/shared_spb_index.glsl"
	#define pMatrixModel pSharedSPB[spbIndex].pSPBMatrixModel
#endif
#include "shared/defren/skin/ubo_special_parameters.glsl"

layout(location=0) in vec3 inPosition;

#if defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED || defined GS_RENDER_STEREO
	flat out int vGSSPBIndex;
	flat out int vGSSPBFlags;
	
	#define vSPBIndex vGSSPBIndex
	#define vSPBFlags vGSSPBFlags
	
#else
	out float vDepth;
	out vec3 vPosition;
	out vec3 vClipCoord;
	flat out int vSPBIndex;
	flat out int vSPBFlags;
#endif

#ifdef VS_RENDER_STEREO
	uniform int pDrawIDOffset;
	#define inLayer (gl_DrawID + pDrawIDOffset)
	flat out int vLayer;
#else
	const int inLayer = 0;
#endif

#include "shared/defren/sanitize_position.glsl"

#include "shared/defren/skin/depth_offset.glsl"

void main(void){
	#include "shared/defren/skin/shared_spb_index2.glsl"
	
	vec4 position = vec4(sanitizePosition(pMatrixModel * vec4(inPosition, 1)), 1);
	
	#if defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED || defined GS_RENDER_STEREO
		gl_Position = position;
		
	#else
		gl_Position = pMatrixVP[inLayer] * position;
		vDepth = dot(pTransformZ[inLayer], position);
		vPosition = pMatrixV[inLayer] * position;
		vClipCoord = pMatrixV[inLayer] * position;
		
		#if defined DEPTH_OFFSET
			#ifdef DEPTH_DISTANCE
				applyDepthOffset(inLayer, vPosition.z);
			#else
				applyDepthOffset(inLayer);
			#endif
		#endif
	#endif
	
	vSPBIndex = spbIndex;
	vSPBFlags = spbFlags;
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
		vLayer = inLayer;
	#endif
}
