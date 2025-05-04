#ifdef EXT_ARB_SHADER_VIEWPORT_LAYER_ARRAY
	#extension GL_ARB_shader_viewport_layer_array : require
#endif
#ifdef EXT_AMD_VERTEX_SHADER_LAYER
	#extension GL_AMD_vertex_shader_layer : require
#endif
#ifdef EXT_ARB_SHADER_DRAW_PARAMETERS
	#extension GL_ARB_shader_draw_parameters : require
#endif

// Uniform Parameters
///////////////////////

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"
#include "shared/defren/skin/ubo_instance_parameters.glsl"
#include "shared/defren/skin/ubo_texture_parameters.glsl"
#include "shared/defren/skin/ubo_dynamic_parameters_vertex.glsl"
#ifdef SHARED_SPB
	#include "shared/defren/skin/shared_spb_index.glsl"
	#include "shared/defren/skin/shared_spb_redirect.glsl"
#endif
#include "shared/defren/skin/shared_spb_texture_redirect.glsl"
#include "shared/defren/skin/ubo_special_parameters.glsl"


// Samplers
/////////////

uniform lowp sampler2D texHeightMapMask;

uniform HIGHP samplerBuffer texSubInstance1;
uniform HIGHP samplerBuffer texSubInstance2;



// Inputs
///////////

#ifdef HEIGHT_MAP
	layout(location=0) in vec2 inPosition;
	layout(location=1) in float inHeight;
	layout(location=2) in float inNormal;
#else
	layout(location=0) in vec3 inPosition;
	layout(location=1) in vec3 inRealNormal;
	layout(location=2) in vec3 inNormal;
	layout(location=3) in vec4 inTangent;
	layout(location=4) in vec2 inTexCoord;
#endif



// Outputs
////////////

#ifdef HAS_TESSELLATION_SHADER
	#define PASS_ON_NEXT_STAGE 1
	
	out vec2 vTCSTCColor;
	out float vTCSHTMask;
	out vec3 vTCSNormal;
	out vec3 vTCSTangent;
	out vec3 vTCSBitangent;
	flat out int vTCSDoubleSided;
	
	#define vTCColor vTCSTCColor
	#define vHTMask vTCSHTMask
	#define vNormal vTCSNormal
	#define vTangent vTCSTangent
	#define vBitangent vTCSBitangent
	#define vDoubleSided vTCSDoubleSided
	
#elif defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED || defined GS_RENDER_STEREO
	#define PASS_ON_NEXT_STAGE 1
	
	out vec2 vGSTCColor;
	out float vGSHTMask;
	out vec3 vGSNormal;
	out vec3 vGSTangent;
	out vec3 vGSBitangent;
	flat out int vGSDoubleSided;
	flat out int vGSSPBIndex;
	flat out int vGSSPBFlags;
	
	#define vTCColor vGSTCColor
	#define vHTMask vGSHTMask
	#define vNormal vGSNormal
	#define vTangent vGSTangent
	#define vBitangent vGSBitangent
	#define vDoubleSided vGSDoubleSided
	#define vSPBIndex vGSSPBIndex
	#define vSPBFlags vGSSPBFlags
	
#else
	#define WRITE_VLAYER 1
	
	out vec2 vTCColor;
	out vec3 vClipCoord;
	out vec3 vSkinClipCoord;
	out vec3 vPosition;
	out float vHTMask;
	out vec3 vNormal;
	out vec3 vTangent;
	out vec3 vBitangent;
	out vec3 vReflectDir;
	out float vFadeZ;
	flat out int vSPBIndex;
	flat out int vSPBFlags;
	flat out int vLayer;
#endif

#ifdef VS_RENDER_STEREO
	uniform int pDrawIDOffset;
	#define inLayer (gl_DrawID + pDrawIDOffset)
#else
	const int inLayer = 0;
#endif



// Main Function
//////////////////

#include "shared/defren/sanitize_position.glsl"
#include "shared/defren/skin/transform_position.glsl"
#include "shared/defren/skin/transform_normal.glsl"
#include "shared/defren/skin/depth_offset.glsl"

void main(void){
	#include "shared/defren/skin/shared_spb_index2.glsl"
	
	// transform the texture coordinates
	#ifdef HEIGHT_MAP
		vec2 tc = pMatrixTexCoord * vec3(inPosition, 1.0);
	#else
		vec2 tc = pMatrixTexCoord * vec3(inTexCoord, 1.0);
	#endif
	vTCColor = tc; // * pTCTransformColor.xy + pTCTransformColor.zw;
	
	// transform position
	vec3 position;
	sTransformTransfer transformTransfer;
	transformPosition(position, spbIndex, transformTransfer);
	
	// normal calculation
	transformNormal(spbIndex, transformTransfer);
	
	#ifdef PASS_ON_NEXT_STAGE
		vDoubleSided = pDoubleSided ? 1 : 0;
		
	#else
		#ifdef BILLBOARD
			vReflectDir = position;
			vPosition = position;
			
			// clip coordinates for use with the clip plane.
			// for the clip plane we need camera space coordinates
			vClipCoord = position;
			
			// fade range requires non-perspective z.
			// and when we are at it already spare some calculations
			vFadeZ = position.z;
			
		#else
			vReflectDir = pMatrixV[inLayer] * vec4(position, 1.0);
			vPosition = pMatrixV[inLayer] * vec4(position, 1.0);
			vClipCoord = pMatrixV[inLayer] * vec4(position, 1.0);
			vFadeZ = (pMatrixV[inLayer] * vec4(position, 1.0)).z;
		#endif
		
		#ifdef HEIGHT_MAP
			vSkinClipCoord = vec3(inPosition.x, inHeight, inPosition.y);
		#else
			vSkinClipCoord = vec3(inPosition);
		#endif
		
		#if defined DEPTH_OFFSET
			#ifdef DEPTH_DISTANCE
				applyDepthOffset(inLayer, vNormal, pDoubleSided, vPosition.z);
			#else
				applyDepthOffset(inLayer, vNormal, pDoubleSided);
			#endif
		#endif
	#endif
	
	// height terrain mask. this can be interpolated since each texel refers to exactly one vertex in the height map
	#ifdef HEIGHT_MAP
		//vHTMask = texelFetch(texHeightMapMask, ivec2(inPosition * pHeightTerrainMaskTCTransform + vec2(0.1)), 0)[pHeightTerrainMaskSelector.y];
		vHTMask = texture(texHeightMapMask, inPosition * pHeightTerrainMaskTCTransform + vec2(0.5))[pHeightTerrainMaskSelector.y];
	#else
		vHTMask = 1.0;
	#endif
	
	vSPBIndex = spbIndex;
	vSPBFlags = spbFlags;
	
	#ifdef WRITE_VLAYER
		vLayer = inLayer;
	#endif
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
	#endif
}
