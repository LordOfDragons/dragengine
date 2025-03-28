#ifdef EXT_ARB_SHADER_VIEWPORT_LAYER_ARRAY
	#extension GL_ARB_shader_viewport_layer_array : require
#endif
#ifdef EXT_AMD_VERTEX_SHADER_LAYER
	#extension GL_AMD_vertex_shader_layer : require
#endif
#ifdef EXT_ARB_SHADER_DRAW_PARAMETERS
	#extension GL_ARB_shader_draw_parameters : require
#endif

#include "shared/defren/skin/macros_geometry.glsl"

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



// Samplers
/////////////

#ifdef HEIGHT_MAP
	uniform lowp sampler2D texHeightMapMask;
#endif
#ifdef PROP_FIELD
	uniform HIGHP samplerBuffer texSubInstance1;
	uniform HIGHP samplerBuffer texSubInstance2;
#endif



// Inputs
///////////

#ifdef HEIGHT_MAP
	in vec2 inPosition;
	in float inHeight;
	in float inNormal;
#else
	in vec3 inPosition;
	in vec3 inRealNormal;
	in vec3 inNormal;
	#ifdef TEXTURE_NORMAL
		in vec4 inTangent;
	#endif
	in vec2 inTexCoord;
#endif

#ifdef NODE_VERTEX_INPUTS
NODE_VERTEX_INPUTS
#endif



// Outputs
////////////

#ifdef HAS_TESSELLATION_SHADER
	#define PASS_ON_NEXT_STAGE 1
	out vec2 vTCSTCColor;
	#define vTCColor vTCSTCColor
	#ifdef TEXTURE_COLOR_TINT_MASK
		out vec2 vTCSTCColorTintMask;
		#define vTCColorTintMask vTCSTCColorTintMask
	#endif
	#ifdef TEXTURE_NORMAL
		out vec2 vTCSTCNormal;
		#define vTCNormal vTCSTCNormal
	#endif
	#if defined TEXTURE_REFLECTIVITY || defined TEXTURE_ROUGHNESS
		out vec2 vTCSTCReflectivity;
		#define vTCReflectivity vTCSTCReflectivity
	#endif
	#ifdef WITH_EMISSIVITY
		out vec2 vTCSTCEmissivity;
		#define vTCEmissivity vTCSTCEmissivity
	#endif
	#ifdef TEXTURE_REFRACTION_DISTORT
		out vec2 vTCSTCRefractionDistort;
		#define vTCRefractionDistort vTCSTCRefractionDistort
	#endif
	#ifdef TEXTURE_AO
		out vec2 vTCSTCAO;
		#define vTCAO vTCSTCAO
	#endif
	
	out vec3 vTCSNormal;
	#define vNormal vTCSNormal
	#ifdef WITH_TANGENT
		out vec3 vTCSTangent;
		#define vTangent vTCSTangent
	#endif
	#ifdef WITH_BITANGENT
		out vec3 vTCSBitangent;
		#define vBitangent vTCSBitangent
	#endif
	#ifdef HEIGHT_MAP
		out float vTCSHTMask;
		#define vHTMask vTCSHTMask
	#endif
	#ifdef PROP_FIELD
// 		out float vTCSRenderCondition;
	#endif
	#ifdef DEPTH_OFFSET
		flat out int vTCSDoubleSided;
		#define vDoubleSided vTCSDoubleSided
	#endif
	
#elif defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED || defined GS_RENDER_STEREO
	#define PASS_ON_NEXT_STAGE 1
	out vec2 vGSTCColor;
	#define vTCColor vGSTCColor
	#ifdef TEXTURE_COLOR_TINT_MASK
		out vec2 vGSTCColorTintMask;
		#define vTCColorTintMask vGSTCColorTintMask
	#endif
	#ifdef TEXTURE_NORMAL
		out vec2 vGSTCNormal;
		#define vTCNormal vGSTCNormal
	#endif
	#if defined TEXTURE_REFLECTIVITY || defined TEXTURE_ROUGHNESS
		out vec2 vGSTCReflectivity;
		#define vTCReflectivity vGSTCReflectivity
	#endif
	#ifdef WITH_EMISSIVITY
		out vec2 vGSTCEmissivity;
		#define vTCEmissivity vGSTCEmissivity
	#endif
	#ifdef TEXTURE_REFRACTION_DISTORT
		out vec2 vGSTCRefractionDistort;
		#define vTCRefractionDistort vGSTCRefractionDistort
	#endif
	#ifdef TEXTURE_AO
		out vec2 vGSTCAO;
		#define vTCAO vGSTCAO
	#endif
	
	out vec3 vGSNormal;
	#define vNormal vGSNormal
	#ifdef WITH_TANGENT
		out vec3 vGSTangent;
		#define vTangent vGSTangent
	#endif
	#ifdef WITH_BITANGENT
		out vec3 vGSBitangent;
		#define vBitangent vGSBitangent
	#endif
	#ifdef HEIGHT_MAP
		out float vGSHTMask;
		#define vHTMask vGSHTMask
	#endif
	#ifdef PROP_FIELD
// 		out float vGSRenderCondition;
	#endif
	
	#ifdef SHARED_SPB
		flat out int vGSSPBIndex;
		#define vSPBIndex vGSSPBIndex
		
		#if defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED
			flat out int vGSSPBFlags;
		#endif
	#endif
	
	#ifdef DEPTH_OFFSET
		flat out int vGSDoubleSided;
		#define vDoubleSided vGSDoubleSided
	#endif
	
#else
	out vec2 vTCColor;
	#ifdef TEXTURE_COLOR_TINT_MASK
		out vec2 vTCColorTintMask;
	#endif
	#ifdef TEXTURE_NORMAL
		out vec2 vTCNormal;
	#endif
	#if defined TEXTURE_REFLECTIVITY || defined TEXTURE_ROUGHNESS
		out vec2 vTCReflectivity;
	#endif
	#ifdef WITH_EMISSIVITY
		out vec2 vTCEmissivity;
	#endif
	#ifdef TEXTURE_REFRACTION_DISTORT
		out vec2 vTCRefractionDistort;
	#endif
	#ifdef TEXTURE_AO
		out vec2 vTCAO;
	#endif
	
	out vec3 vNormal;
	#ifdef WITH_TANGENT
		out vec3 vTangent;
	#endif
	#ifdef WITH_BITANGENT
		out vec3 vBitangent;
	#endif
	#ifdef WITH_REFLECT_DIR
		out vec3 vReflectDir;
	#endif
	#ifdef HEIGHT_MAP
		out float vHTMask;
	#endif
	#ifdef FADEOUT_RANGE
		out float vFadeZ;
	#endif
	#ifdef SKIN_CLIP_PLANE
		out vec3 vSkinClipCoord;
	#endif
	
	#ifdef SHARED_SPB
		flat out int vSPBIndex;
	#endif
#endif

#ifdef VS_RENDER_STEREO
	uniform int pDrawIDOffset;
	#define inLayer (gl_DrawID + pDrawIDOffset)
	flat out int vLayer;
#else
	const int inLayer = 0;
#endif



// Main Function
//////////////////

#include "shared/defren/sanitize_position.glsl"
#include "shared/defren/skin/transform_position.glsl"
#include "shared/defren/skin/transform_normal.glsl"

void main( void ){
	#include "shared/defren/skin/shared_spb_index2.glsl"
	
	// transform the texture coordinates
	#ifdef HEIGHT_MAP
		vec2 tc = pMatrixTexCoord * vec3( inPosition, 1 );
	#else
		vec2 tc = pMatrixTexCoord * vec3( inTexCoord, 1 );
	#endif
	
	vTCColor = tc; // * pTCTransformColor.xy + pTCTransformColor.zw;
	#ifdef TEXTURE_COLOR_TINT_MASK
		vTCColorTintMask = tc; // * pTCTransformColorTintMask.xy + pTCTransformColorTintMask.zw;
	#endif
	#ifdef TEXTURE_NORMAL
		vTCNormal = tc; // * pTCTransformNormal.xy + pTCTransformNormal.zw;
	#endif
	#if defined TEXTURE_REFLECTIVITY || defined TEXTURE_ROUGHNESS
		vTCReflectivity = tc; // * pTCTransformReflectivity.xy + pTCTransformReflectivity.zw;
	#endif
	#ifdef WITH_EMISSIVITY
		vTCEmissivity = tc; // * pTCTransformEmissivity.xy + pTCTransformEmissivity.zw;
	#endif
	#ifdef TEXTURE_REFRACTION_DISTORT
		vTCRefractionDistort = tc; // * pTCTransformRefractionDistort.xy + pTCTransformRefractionDistort.zw;
	#endif
	#ifdef TEXTURE_AO
		vTCAO = tc; // * pTCTransformAO.xy + pTCTransformAO.zw;
	#endif
	
	// transform position and normal
	vec3 position;
	#ifdef REQUIRES_TRANSFORM_TRANSFER
		sTransformTransfer transformTransfer;
		transformPosition( position, spbIndex, transformTransfer );
		transformNormal( spbIndex, transformTransfer );
	#else
		transformPosition( position, spbIndex );
		transformNormal( spbIndex );
	#endif
	
	#ifdef PASS_ON_NEXT_STAGE
		#ifdef DEPTH_OFFSET
			vDoubleSided = pDoubleSided ? 1 : 0;
		#endif
		
	#else
		// reflection directory for environment map reflections
		#ifdef WITH_REFLECT_DIR
			#ifdef BILLBOARD
				vReflectDir = position;
			#else
				vReflectDir = pMatrixV[ inLayer ] * vec4( position, 1 );
			#endif
		#endif
		
		// fade range requires non-perspective z. and when we are at it already spare some calculations
		#ifdef FADEOUT_RANGE
			#ifdef BILLBOARD
				vFadeZ = position.z;
			#else
				vFadeZ = ( pMatrixV[ inLayer ] * vec4( position, 1 ) ).z;
			#endif
		#endif
		
		// cliping
		#ifdef SKIN_CLIP_PLANE
			#ifdef HEIGHT_MAP
				vSkinClipCoord = vec3( inPosition.x, inHeight, inPosition.y );
			#else
				vSkinClipCoord = vec3( inPosition );
			#endif
		#endif
	#endif
	
	// height terrain mask. this can be interpolated since each texel refers to exactly one vertex in the height map
	#ifdef HEIGHT_MAP
		//vHTMask = texelFetch( texHeightMapMask, ivec2( inPosition * pHeightTerrainMaskTCTransform + vec2( 0.1 ) ), 0 )[ pHeightTerrainMaskSelector.y ];
		vHTMask = texture( texHeightMapMask, inPosition * pHeightTerrainMaskTCTransform + vec2( 0.5 ) )[ pHeightTerrainMaskSelector.y ];
	#endif
	
	#ifdef SHARED_SPB
		vSPBIndex = spbIndex;
		#if defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED
			vGSSPBFlags = spbFlags;
		#endif
	#endif
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
		vLayer = inLayer;
	#endif
}
