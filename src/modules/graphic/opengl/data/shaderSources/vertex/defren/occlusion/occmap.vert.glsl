#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/occmap.glsl"

#ifdef SHARED_SPB
	#include "shared/defren/skin/shared_spb_index.glsl"
	#define pMatrixModel pSharedSPB[spbIndex].pSPBMatrixModel
#endif
#include "shared/defren/skin/ubo_special_parameters.glsl"

layout(location=0) in vec3 inPosition;

#if defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED || defined GS_RENDER_STEREO
	VARYING_BIND(0) flat out int vGSSPBIndex;
	VARYING_BIND(1) flat out int vGSSPBFlags;
	
	#define vSPBIndex vGSSPBIndex
	#define vSPBFlags vGSSPBFlags
	
#else
	VARYING_BIND(0) out float vDepth;
	VARYING_BIND(1) out vec3 vPosition;
	VARYING_BIND(2) out vec3 vClipCoord;
	VARYING_BIND(3) flat out int vSPBIndex;
	VARYING_BIND(4) flat out int vSPBFlags;
#endif

flat out int vLayer;

// VSRenderStereo
UNIFORM_BIND(0) uniform int pDrawIDOffset;

#include "shared/defren/sanitize_position.glsl"

#include "shared/defren/skin/depth_offset.glsl"

void main(void){
	#include "shared/defren/skin/shared_spb_index2.glsl"
	
	vLayer = 0;
	if(VSRenderStereo){
		#ifdef SUPPORTS_VSDRAWPARAM
		vLayer = gl_DrawID + pDrawIDOffset;
		#endif
	}
	
	vec4 position = vec4(sanitizePosition(pMatrixModel * vec4(inPosition, 1)), 1);
	
	#if defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED || defined GS_RENDER_STEREO
		gl_Position = position;
		
	#else
		gl_Position = getMatrixVP(vLayer) * position;
		vDepth = dot(getTransformZ(vLayer), position);
		vPosition = getMatrixV(vLayer) * position;
		vClipCoord = getMatrixV(vLayer) * position;
		
		if(DepthOffset){
			if(DepthDistance){
				applyDepthOffset(vLayer, vPosition.z);
				
			}else{
				applyDepthOffset(vLayer);
			}
		}
	#endif
	
	vSPBIndex = spbIndex;
	vSPBFlags = spbFlags;
	
	#ifdef SUPPORTS_VSLAYER
	if(VSRenderStereo){
		gl_Layer = vLayer;
	}
	#endif
}
