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

#ifdef WITH_GEOMETRY_SHADER
	// opengl
	VARYING_BIND(0) out float vGSDepth;
	VARYING_BIND(1) out vec3 vGSPosition;
	VARYING_BIND(2) out vec3 vGSClipCoord;
	VARYING_BIND(3) flat out int vGSSPBIndex;
	VARYING_BIND(4) flat out int vGSSPBFlags;
	VARYING_BIND(5) flat out int vGSLayer;
	
	#define vDepth vGSDepth
	#define vPosition vGSPosition
	#define vClipCoord vGSClipCoord
	#define vSPBIndex vGSSPBIndex
	#define vSPBFlags vGSSPBFlags
	#define vLayer vGSLayer
	
#else
	// spir-v
	VARYING_BIND(0) out float vDepth;
	VARYING_BIND(1) out vec3 vPosition;
	VARYING_BIND(2) out vec3 vClipCoord;
	VARYING_BIND(3) flat out int vSPBIndex;
	VARYING_BIND(4) flat out int vSPBFlags;
	VARYING_BIND(5) flat out int vLayer;
#endif

// VSRenderLayer
UNIFORM_BIND(1) uniform int pDrawIDOffset;

#include "shared/defren/sanitize_position.glsl"

#include "shared/defren/skin/depth_offset.glsl"

void main(void){
	#include "shared/defren/skin/shared_spb_index2.glsl"
	
	vLayer = 0;
	if(VSRenderLayer){
		#ifdef SUPPORTS_VSDRAWPARAM
		vLayer = gl_DrawID + pDrawIDOffset;
		#endif
	}
	
	vec4 position = vec4(sanitizePosition(pMatrixModel * vec4(inPosition, 1)), 1);
	
	if(LayeredRendering != LayeredRenderingNone && !VSRenderLayer){
		gl_Position = position;
		
	}else{
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
	}
	
	vSPBIndex = spbIndex;
	vSPBFlags = spbFlags;
	
	#if defined SUPPORTS_VSLAYER && ! defined OPENGLES
	if(VSRenderLayer){
		gl_Layer = vLayer;
	}
	#endif
}
