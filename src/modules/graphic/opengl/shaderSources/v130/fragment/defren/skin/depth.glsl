// request high precision if the graphic card supports this
#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

// some helper definitions to make the code easier to read
#if defined TEXTURE_SOLIDITY || defined TEXTURE_HEIGHT || defined TEXTURE_EMISSIVITY
	#define _REQ_TEX_CLR_1 1
#endif
#if defined OUTPUT_COLOR || _REQ_TEX_CLR_1
	#define REQUIRES_TEX_COLOR 1
#endif

#ifdef TEXTURE_HEIGHT
	#define REQUIRES_NORMAL 1
#endif

#if defined TEXTURE_SOLIDITY || WITH_OUTLINE
	#define WITH_SOLIDITY 1
#endif
#if defined TEXTURE_EMISSIVITY || WITH_OUTLINE
	#define WITH_EMISSIVITY 1
#endif


// Uniform Parameters
///////////////////////

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/skin/ubo_render_parameters.glsl"
#include "v130/shared/defren/skin/ubo_texture_parameters.glsl"
#include "v130/shared/defren/skin/ubo_instance_parameters.glsl"
#include "v130/shared/defren/skin/ubo_dynamic_parameters.glsl"

#ifdef NODE_FRAGMENT_UNIFORMS
NODE_FRAGMENT_UNIFORMS
#endif



// Samplers
/////////////

#ifdef WITH_VARIATIONS
	// functions are defined right before main due to Shared-SPB support
	#define SAMPLER_2D sampler2DArray
	#define TEXTURE(s,tc) texture(s, tcTexVar(tc, textureSize(s, 0).z))
#else
	#define SAMPLER_2D sampler2D
	#define TEXTURE(s,tc) texture(s, tc)
#endif

#ifdef OUTPUT_COLOR
	uniform lowp sampler2D texColor;
#endif
#ifdef TEXTURE_SOLIDITY
	uniform lowp sampler2D texSolidity;
#endif
#ifdef TEXTURE_EMISSIVITY
	uniform mediump SAMPLER_2D texEmissivity;
#endif
#ifdef TEXTURE_ENVROOM_EMISSIVITY
	uniform mediump samplerCube texEnvRoomEmissivity;
#endif
#ifdef DEPTH_TEST
	uniform HIGHP sampler2D texDepthTest;
#endif
#ifdef NODE_FRAGMENT_SAMPLERS
NODE_FRAGMENT_SAMPLERS
#endif



// Inputs
///////////

#ifdef REQUIRES_TEX_COLOR
	in vec2 vTCColor;
	#define tcColor vTCColor
#endif
// #if defined PARTICLE && defined TEXTURE_EMISSIVITY
// 	in float vParticleEmissivity; // from curve property
// #endif
#ifdef CLIP_PLANE
	in vec3 vClipCoord;
#endif
#ifdef DEPTH_ORTHOGONAL
	in float vZCoord;
#endif
#ifdef DEPTH_DISTANCE
	in vec3 vPosition;
#endif
#ifdef HEIGHT_MAP
	in float vHTMask;
#endif
#ifdef REQUIRES_NORMAL
	in vec3 vNormal;
	#ifdef TEXTURE_NORMAL
		in vec3 vTangent;
		in vec3 vBitangent;
	#endif
#endif
#ifdef FADEOUT_RANGE
	in float vFadeZ;
#endif
#ifdef SHARED_SPB
	flat in int vSPBIndex;
	#define spbIndex vSPBIndex
	#include "v130/shared/defren/skin/shared_spb_redirect.glsl"
#endif
#ifdef NODE_FRAGMENT_INPUTS
NODE_FRAGMENT_INPUTS
#endif



// Outputs
////////////

#ifdef OUTPUT_CONSTANT
	out float outConstant;
#endif
#ifdef OUTPUT_COLOR
	out vec4 outColor;
#endif
#ifdef ENCODE_OUT_DEPTH
	out vec4 outDepth;
#endif
#ifdef NODE_FRAGMENT_OUTPUTS
NODE_FRAGMENT_OUTPUTS
#endif



// Constants
//////////////

#ifdef ENCODE_OUT_DEPTH
	const vec3 packShift = vec3( 1.0, 256.0, 65536.0 );
	const vec3 packMask = vec3( 1.0 / 256.0, 1.0 / 256.0, 0.0 );
#endif
#ifdef DECODE_IN_DEPTH
	const vec3 unpackDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );
#endif



#if defined TEXTURE_ENVROOM || defined TEXTURE_ENVROOM_EMISSIVITY
//	#include "v130/shared/defren/skin/environment_room.glsl"
// this is not going to work since vReflectDir is not defined and without normal map
// vTanget is also not defined
#endif



// Main Function
//////////////////

/*
#ifndef HAS_TESSELLATION_SHADER
#include "v130/shared/defren/skin/relief_mapping.glsl"
#endif
*/

void main( void ){
	// discard fragments beyond render range
	#ifdef FADEOUT_RANGE
		if( vFadeZ > pFadeRange.y ){
			discard;
		}
	#endif
	
	// calculate depth if non-projective depth is used. this has to be done before any branching
	// because derivatives become undefined otherwise.
	#ifdef DEPTH_ORTHOGONAL
		vec2 depthDeriv = vec2( dFdx( vZCoord ), dFdy( vZCoord ) );
		gl_FragDepth = vZCoord;
	#endif
	#ifdef DEPTH_DISTANCE
		float depth = length( vPosition ) * pDepthTransform.x + pDepthTransform.y;
		vec2 depthDeriv = vec2( dFdx( depth ), dFdy( depth ) );
		gl_FragDepth = depth;
	#endif
	
	#ifdef DEPTH_OFFSET
		/*if( gl_FrontFacing ){
			gl_FragDepth += length( depthDeriv ) * pDepthOffset.x + pDepthOffset.y;
			
		}else{
			gl_FragDepth += length( depthDeriv ) * pDepthOffset.z + pDepthOffset.w;
		}*/
		vec2 depthOffset = mix( pDepthOffset.zw, pDepthOffset.xy, bvec2( gl_FrontFacing || pDoubleSided ) ); // mix( if-false, if-true, condition )
		gl_FragDepth += length( depthDeriv ) * depthOffset.x + depthOffset.y;
	#endif
	
	#ifdef NO_ZCLIP
		gl_FragDepth = max( gl_FragDepth, 0.0 );
	#endif
	
	// discard fragments using the clip plane
	#ifdef CLIP_PLANE
		if( dot( vClipCoord, pClipPlane.xyz ) <= pClipPlane.w ) discard;
	#endif
	
	
	
	// determine the correct normal. for back facing fragments the normal has to be flipped. care
	// has to be taken if the rendering is mirrored. in this case the front facing is exactly the
	// opposite of what we are really looking for. The xor operator does exactly this
	/*
	#if defined REQUIRES_NORMAL && defined REQUIRES_TEX_COLOR && ! defined HAS_TESSELLATION_SHADER
		vec3 realNormal = mix( -vNormal, vNormal, vec3( pFlipCulling ^^ gl_FrontFacing ) ); // mix( if-false, if-true, condition )
		
		// relief mapping
		vec2 tcReliefMapped = vTCColor;
		reliefMapping( tcReliefMapped, realNormal );
		
		#undef tcColor
		#define tcColor tcReliefMapped
	#endif
	*/
	
	
	// get texture properties from textures
	#ifdef OUTPUT_COLOR
		vec3 color = texture( texColor, tcColor ).rgb;
	#endif
	
	#ifdef WITH_SOLIDITY
		float solidity;
		#ifdef TEXTURE_SOLIDITY
			solidity = texture( texSolidity, tcColor ).r * pSolidityMultiplier;
		#elif defined WITH_OUTLINE
			solidity = pOutlineSolidity;
		#endif
	#endif
	
	#ifdef WITH_EMISSIVITY
		vec3 emissivity;
		#ifdef TEXTURE_EMISSIVITY
			emissivity = TEXTURE( texEmissivity, tcColor ).rgb;
		#elif defined WITH_OUTLINE
			emissivity = pOutlineEmissivity;
		#endif
	#endif
	
	#ifdef NODE_FRAGMENT_MAIN
	NODE_FRAGMENT_MAIN
	#endif
	
	// discard fragments using masked solidity
	#ifdef WITH_SOLIDITY
		#ifdef MASKED_SOLIDITY
		if( solidity < 0.35 )
		#else
		if( solidity < 0.001 )
		#endif
		{
			#ifdef WITH_EMISSIVITY
				//vec2 tcReliefMapped = vTCColor;
				//reliefMapping( tcReliefMapped, realNormal );
				//#define tcColor tcReliefMapped
				//#define tcEmissivity tcReliefMapped
				
				if( all( lessThan( emissivity, vec3( 0.001 ) ) ) ){
					discard;
				}
			#else
				discard;
			#endif
		}
	#endif
	
	// determine where the depth is coming from. this is different depending if projective depth is used or not
	#if defined DEPTH_ORTHOGONAL || defined DEPTH_DISTANCE
		#define fragmentDepth gl_FragDepth
	#else
		#define fragmentDepth gl_FragCoord.z
	#endif
	
	// discard fragments if the depth is larger than the corresponding fragment in the back depth texture
	#ifdef DEPTH_TEST
		ivec2 tc = ivec2( gl_FragCoord.xy );
		
		#ifdef DECODE_IN_DEPTH
		float depthTestValue = dot( texelFetch( texDepthTest, tc, 0 ).rgb, unpackDepth );
		#else
		float depthTestValue = texelFetch( texDepthTest, tc, 0 ).r;
		#endif
		
		#ifdef INVERSE_DEPTH
			#ifdef DEPTH_TEST_LARGER
			if( fragmentDepth <= depthTestValue ) discard;
			#else
			if( fragmentDepth >= depthTestValue ) discard;
			#endif
		#else
			#ifdef DEPTH_TEST_LARGER
			if( fragmentDepth >= depthTestValue ) discard;
			#else
			if( fragmentDepth <= depthTestValue ) discard;
			#endif
		#endif
	#endif
	
	// encode the output depth
	#ifdef ENCODE_OUT_DEPTH
		vec3 encoded = fract( packShift * vec3( fragmentDepth ) );
		outDepth = vec4( encoded - ( encoded.yzz * packMask ), 1.0 );
	#endif
	
	// output constant value or color
	#ifdef OUTPUT_CONSTANT
		outConstant = 1.0;
	#endif
	#ifdef OUTPUT_COLOR
		color = pow( color, vec3( pColorGamma ) );
		outColor = vec4( color, 1.0 );
	#endif
}
