#include "shared/preamble.glsl"

// Uniform Parameters
///////////////////////

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"
#include "shared/defren/skin/ubo_instance_parameters.glsl"
#include "shared/defren/skin/ubo_texture_parameters.glsl"
#ifdef SHARED_SPB
	#include "shared/defren/skin/shared_spb_index.glsl"
	#include "shared/defren/skin/shared_spb_redirect.glsl"
#endif
#include "shared/defren/skin/shared_spb_texture_redirect.glsl"
#include "shared/defren/skin/ubo_dynamic_parameters.glsl"
#include "shared/defren/skin/ubo_special_parameters.glsl"



// Samplers
/////////////

layout(binding=20) uniform lowp sampler2D texHeightMapMask;

layout(binding=20) uniform HIGHP samplerBuffer texSubInstance1;
layout(binding=21) uniform HIGHP samplerBuffer texSubInstance2;



// Inputs
///////////

// GeometryMode == GeometryModeHeightMap
#define inHMPosition (inPosition.xy)
#define inHMHeight (inRealNormal.x)
#define inHMNormal (inNormal.x)

// GeometryMode != GeometryModeHeightMap
layout(location=0) in vec3 inPosition;
layout(location=1) in vec3 inRealNormal;
layout(location=2) in vec3 inNormal;
layout(location=3) in vec4 inTangent;
layout(location=4) in vec2 inTexCoord;



// Outputs
////////////

#include "shared/interface/skin/vertex.glsl"



// Main Function
//////////////////

#include "shared/defren/sanitize_position.glsl"
#include "shared/defren/skin/transform_position.glsl"
#include "shared/defren/skin/transform_normal.glsl"

void main( void ){
	#include "shared/defren/skin/shared_spb_index2.glsl"
	
	vertexShaderDefaultOutputs();
	
	// transform the texture coordinates
	vec2 tc;
	if(GeometryMode == GeometryModeHeightMap){
		tc = pMatrixTexCoord * vec3(inHMPosition, 1.0);
		
	}else{
		tc = pMatrixTexCoord * vec3(inTexCoord, 1.0);
	}
	
	vTCColor = tc; // * pTCTransformColor.xy + pTCTransformColor.zw;
	vTCColorTintMask = tc; // * pTCTransformColorTintMask.xy + pTCTransformColorTintMask.zw;
	vTCNormal = tc; // * pTCTransformNormal.xy + pTCTransformNormal.zw;
	vTCReflectivity = tc; // * pTCTransformReflectivity.xy + pTCTransformReflectivity.zw;
	vTCEmissivity = tc; // * pTCTransformEmissivity.xy + pTCTransformEmissivity.zw;
	vTCRefractionDistort = tc; // * pTCTransformRefractionDistort.xy + pTCTransformRefractionDistort.zw;
	vTCAO = tc; // * pTCTransformAO.xy + pTCTransformAO.zw;
	
	// transform position and normal
	vec3 position;
	sTransformTransfer transformTransfer;
	transformPosition(position, spbIndex, transformTransfer);
	transformNormal(spbIndex, transformTransfer);
	
	if(PassOnNextStage){
		vDoubleSided = pDoubleSided ? 1 : 0;
		
	}else{
		if(AnyKindBillboard){
			// reflection directory for environment map reflections
			vReflectDir = position;
			
			// fade range requires non-perspective z.
			// and when we are at it already spare some calculations
			vFadeZ = position.z;
			
		}else{
			vReflectDir = pMatrixV[inLayer] * vec4(position, 1.0);
			vFadeZ = (pMatrixV[inLayer] * vec4(position, 1.0)).z;
		}
		
		if(GeometryMode == GeometryModeHeightMap){
			vSkinClipCoord = vec3(inHMPosition.x, inHMHeight, inHMPosition.y);
			
		}else{
			vSkinClipCoord = vec3(inPosition);
		}
	}
	
	// height terrain mask. this can be interpolated since each texel refers to exactly one vertex in the height map
	if(GeometryMode == GeometryModeHeightMap){
		//vHTMask = texelFetch(texHeightMapMask, ivec2(inHMPosition * pHeightTerrainMaskTCTransform + vec2(0.1)), 0.0)[pHeightTerrainMaskSelector.y];
		vHTMask = texture(texHeightMapMask, inHMPosition * pHeightTerrainMaskTCTransform + vec2(0.5))[pHeightTerrainMaskSelector.y];
		
	}else{
		vHTMask = 1.0;
	}
	
	vSPBIndex = spbIndex;
	vSPBFlags = spbFlags;
	
	vLayer = 0;
	if(VSRenderLayer){
		vLayer = getInLayer();
		#if defined SUPPORTS_VSLAYER && ! defined OPENGLES
		gl_Layer = vLayer;
		#endif
	}
}
