#include "v130/shared/defren/skin/macros_geometry.glsl"

// Uniform Parameters
///////////////////////

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/skin/ubo_render_parameters.glsl"
#include "v130/shared/defren/skin/ubo_instance_parameters.glsl"
#include "v130/shared/defren/skin/ubo_texture_parameters.glsl"
#ifdef SHARED_SPB
	#include "v130/shared/defren/skin/shared_spb_index.glsl"
	#include "v130/shared/defren/skin/shared_spb_redirect.glsl"
#endif
#include "v130/shared/defren/skin/ubo_dynamic_parameters.glsl"

#ifdef NODE_VERTEX_UNIFORMS
NODE_VERTEX_UNIFORMS
#endif



// Samplers
/////////////

#ifdef HEIGHT_MAP
	uniform lowp sampler2D texHeightMapMask;
#endif
#ifdef PROP_FIELD
	uniform mediump samplerBuffer texSubInstance1;
	uniform mediump samplerBuffer texSubInstance2;
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
	in vec4 inTangent;
	in vec2 inTexCoord;
#endif

#ifdef NODE_VERTEX_INPUTS
NODE_VERTEX_INPUTS
#endif



// Outputs
////////////

#ifdef HAS_TESSELLATION_SHADER
	#ifdef REQUIRES_TEX_COLOR
		out vec2 vTCSTCColor;
		#define vTCColor vTCSTCColor
	#endif
	#ifdef CLIP_PLANE
		out vec3 vTCSClipCoord;
		#define vClipCoord vTCSClipCoord
	#endif
	#ifdef DEPTH_DISTANCE
		out vec3 vTCSPosition;
		#define vPosition vTCSPosition
	#endif
	#ifdef HEIGHT_MAP
		out float vTCSHTMask;
		#define vHTMask vTCSHTMask
	#endif
	#ifdef PROP_FIELD
		out float vTCSRenderCondition;
	#endif
	#ifdef REQUIRES_NORMAL
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
	#endif
	#ifdef WITH_REFLECT_DIR
		out vec3 vTCSReflectDir;
		#define vReflectDir vTCSReflectDir
	#endif
	
#elif defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED
	#ifdef REQUIRES_TEX_COLOR
		out vec2 vGSTCColor;
		#define vTCColor vGSTCColor
	#endif
	#ifdef CLIP_PLANE
		out vec3 vGSClipCoord;
		#define vClipCoord vGSClipCoord
	#endif
	#ifdef DEPTH_DISTANCE
		out vec3 vGSPosition;
		#define vPosition vGSPosition
	#endif
	#ifdef HEIGHT_MAP
		out float vGSHTMask;
		#define vHTMask vGSHTMask
	#endif
	#ifdef REQUIRES_NORMAL
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
	#endif
	#ifdef WITH_REFLECT_DIR
		out vec3 vGSReflectDir;
		#define vReflectDir vGSReflectDir
	#endif
	
	#ifdef SHARED_SPB
		flat out int vGSSPBIndex;
		#define vSPBIndex vGSSPBIndex
		
		#if defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED
			flat out int vGSSPBFlags;
		#endif
	#endif
	
#else
	#ifdef REQUIRES_TEX_COLOR
		out vec2 vTCColor;
	#endif
	#ifdef CLIP_PLANE
		out vec3 vClipCoord;
	#endif
	#ifdef DEPTH_ORTHOGONAL
		out float vZCoord;
	#endif
	#ifdef DEPTH_DISTANCE
		out vec3 vPosition;
	#endif
	#ifdef HEIGHT_MAP
		out float vHTMask;
	#endif
	#ifdef REQUIRES_NORMAL
		out vec3 vNormal;
		#ifdef WITH_TANGENT
			out vec3 vTangent;
		#endif
		#ifdef WITH_BITANGENT
			out vec3 vBitangent;
		#endif
	#endif
	#ifdef WITH_REFLECT_DIR
		out vec3 vReflectDir;
	#endif
	#ifdef FADEOUT_RANGE
		out float vFadeZ;
	#endif
	
	#ifdef SHARED_SPB
		flat out int vSPBIndex;
	#endif
#endif

#ifdef NODE_VERTEX_OUTPUTS
NODE_VERTEX_OUTPUTS
#endif



// Constants
//////////////



// Main Function
//////////////////

#include "v130/shared/defren/skin/transform_position.glsl"
#ifdef REQUIRES_NORMAL
	#include "v130/shared/defren/skin/transform_normal.glsl"
#endif

void main( void ){
	#include "v130/shared/defren/skin/shared_spb_index2.glsl"
	
	// transform the texture coordinates
	#ifdef REQUIRES_TEX_COLOR
		#ifdef HEIGHT_MAP
			vec2 tc = pMatrixTexCoord * vec3( inPosition, 1.0 );
		#else
			vec2 tc = pMatrixTexCoord * vec3( inTexCoord, 1.0 );
		#endif
		vTCColor = tc; // * pTCTransformColor.xy + pTCTransformColor.zw;
	#endif
	
	// transform position
	vec3 position;
	#ifdef REQUIRES_TRANSFORM_TRANSFER
		sTransformTransfer transformTransfer;
		transformPosition( position, spbIndex, transformTransfer );
	#else
		transformPosition( position, spbIndex );
	#endif
	
	// normal calculation
	#ifdef REQUIRES_NORMAL
		#ifdef REQUIRES_TRANSFORM_TRANSFER
			transformNormal( spbIndex, transformTransfer );
		#else
			transformNormal( spbIndex );
		#endif
	#endif
	
	// reflection directory for environment map reflections
	#ifdef WITH_REFLECT_DIR
		#ifdef HAS_TESSELLATION_SHADER
			vReflectDir = position;
		#else
			#ifdef BILLBOARD
				vReflectDir = position;
			#else
				#ifdef GS_RENDER_CUBE
					vReflectDir = pMatrixV[ 0 ] * vec4( position, 1.0 );
				#else
					vReflectDir = pMatrixV * vec4( position, 1.0 );
				#endif
			#endif
		#endif
	#endif
	
	// non-perspective depth values if required
	#if ! defined HAS_TESSELLATION_SHADER && ! defined GS_RENDER_CUBE && ! defined GS_RENDER_CASCADED
		#ifdef DEPTH_ORTHOGONAL
			#ifdef NO_ZCLIP
				vZCoord = gl_Position.z * 0.5 + 0.5; // we have to do the normalization ourself
				gl_Position.z = 0.0;
			#else
				vZCoord = gl_Position.z;
			#endif
		#endif
	#endif
	#ifdef DEPTH_DISTANCE
		#ifdef HAS_TESSELLATION_SHADER
			vPosition = position;
		#else
			#ifdef BILLBOARD
				vPosition = position;
			#else
				#ifdef GS_RENDER_CUBE
					vPosition = pMatrixV[ 0 ] * vec4( position, 1.0 );
				#else
					vPosition = pMatrixV * vec4( position, 1.0 );
				#endif
			#endif
		#endif
	#endif
	
	// clip coordinates for use with the clip plane. for the clip plane we need camera space coordinates
	#ifdef CLIP_PLANE
		#ifdef HAS_TESSELLATION_SHADER
			vClipCoord = position;
		#else
			#ifdef BILLBOARD
				vClipCoord = position;
			#else
				#ifdef GS_RENDER_CUBE
					vClipCoord = pMatrixV[ 0 ] * vec4( position, 1.0 );
				#else
					vClipCoord = pMatrixV * vec4( position, 1.0 );
				#endif
			#endif
		#endif
	#endif
	
	// height terrain mask. this can be interpolated since each texel refers to exactly one vertex in the height map
	#ifdef HEIGHT_MAP
		//vHTMask = texelFetch( texHeightMapMask, ivec2( inPosition * pHeightTerrainMaskTCTransform + vec2( 0.1 ) ), 0 )[ pHeightTerrainMaskSelector.y ];
		vHTMask = texture( texHeightMapMask, inPosition * pHeightTerrainMaskTCTransform + vec2( 0.5 ) )[ pHeightTerrainMaskSelector.y ];
	#endif
	
	// fade range requires non-perspective z. and when we are at it already spare some calculations
	#ifdef FADEOUT_RANGE
		#ifdef BILLBOARD
			vFadeZ = position.z;
		#else
			#ifdef GS_RENDER_CUBE
				vFadeZ = ( pMatrixV[ 0 ] * vec4( position, 1.0 ) ).z;
			#else
				vFadeZ = ( pMatrixV * vec4( position, 1.0 ) ).z;
			#endif
		#endif
	#endif
	
	#ifdef SHARED_SPB
		vSPBIndex = spbIndex;
		#if defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED
			vGSSPBFlags = spbFlags;
		#endif
	#endif
	
	#ifdef NODE_VERTEX_MAIN
	NODE_VERTEX_MAIN
	#endif
}
