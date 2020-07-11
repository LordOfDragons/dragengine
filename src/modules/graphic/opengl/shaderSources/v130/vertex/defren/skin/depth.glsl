// request high precision if the graphic card supports this
#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

// some helper definitions to make the code easier to read
#if defined OUTPUT_COLOR || defined TEXTURE_SOLIDITY || defined TEXTURE_HEIGHT
	#define _REQ_TEX_CLR_1 1
#endif
#if defined TEXTURE_EMISSIVITY || defined TEXTURE_RIM_EMISSIVITY
	#define _REQ_TEX_CLR_2 1
#endif
#if defined _REQ_TEX_CLR_1 || defined _REQ_TEX_CLR_2
	#define REQUIRES_TEX_COLOR 1
#endif

#if defined TEXTURE_HEIGHT || defined TEXTURE_RIM_EMISSIVITY
	#define REQUIRES_NORMAL 1
#endif
#if defined TEXTURE_ENVMAP || defined TEXTURE_RIM_EMISSIVITY
	#define WITH_REFLECT_DIR 1
#endif



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
		#ifdef TEXTURE_NORMAL
			out vec3 vTCSTangent;
			#define vTangent vTCSTangent
			out vec3 vTCSBitangent;
			#define vBitangent vTCSBitangent
		#endif
	#endif
	#ifdef WITH_REFLECT_DIR
		out vec3 vTCSReflectDir;
		#define vReflectDir vTCSReflectDir
	#endif
	
#elif defined GS_RENDER_CUBE
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
		#ifdef TEXTURE_NORMAL
			out vec3 vGSTangent;
			#define vTangent vGSTangent
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
		
		#ifdef GS_RENDER_CUBE
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
		#ifdef TEXTURE_NORMAL
			out vec3 vTangent;
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
	transformPosition( position, spbIndex );
	
	// normal calculation
	#ifdef REQUIRES_NORMAL
		transformNormal( spbIndex );
	#endif
	
	// reflection directory for environment map reflections
	#ifdef WITH_REFLECT_DIR
		#ifdef HAS_TESSELLATION_SHADER
			vReflectDir = position;
		#else
			#ifdef BILLBOARD
				vReflectDir = position;
			#else
				vReflectDir = pMatrixV * vec4( position, 1.0 );
			#endif
		#endif
	#endif
	
	// non-perspective depth values if required
	#ifndef HAS_TESSELLATION_SHADER
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
				vPosition = pMatrixV * vec4( position, 1.0 );
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
				vClipCoord = pMatrixV * vec4( position, 1.0 );
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
			vFadeZ = ( pMatrixV * vec4( position, 1.0 ) ).z;
		#endif
	#endif
	
	#ifdef SHARED_SPB
		vSPBIndex = spbIndex;
		#ifdef GS_RENDER_CUBE
			vGSSPBFlags = spbFlags;
		#endif
	#endif
	
	#ifdef NODE_VERTEX_MAIN
	NODE_VERTEX_MAIN
	#endif
}
