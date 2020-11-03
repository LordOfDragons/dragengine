#include "v130/shared/defren/skin/macros_geometry.glsl"

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
#if defined REQUIRES_NORMAL && defined TEXTURE_NORMAL
	uniform lowp SAMPLER_2D texNormal;
#endif
#ifdef TEXTURE_EMISSIVITY
	uniform mediump SAMPLER_2D texEmissivity;
#endif
#ifdef TEXTURE_RIM_EMISSIVITY
	uniform mediump SAMPLER_2D texRimEmissivity;
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
#ifdef TEXTURE_NORMAL
	in vec2 vTCNormal;
	#define tcNormal vTCNormal
#endif
#if defined TEXTURE_EMISSIVITY || defined TEXTURE_RIM_EMISSIVITY
	in vec2 vTCEmissivity;
	#define tcEmissivity vTCEmissivity
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
	#ifdef WITH_TANGENT
		in vec3 vTangent;
	#endif
	#ifdef WITH_BITANGENT
		in vec3 vBitangent;
	#endif
#endif
#ifdef TEXTURE_RIM_EMISSIVITY
	in vec3 vReflectDir;
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



// Includes requiring inputs to be defined
////////////////////////////////////////////

#if defined REQUIRES_NORMAL && ! defined HAS_TESSELLATION_SHADER
	#include "v130/shared/defren/skin/relief_mapping.glsl"
#endif

/*
#if defined TEXTURE_ENVROOM || defined TEXTURE_ENVROOM_EMISSIVITY
#include "v130/shared/defren/skin/environment_room.glsl"
// this is not going to work since vReflectDir is not defined and without normal map
// vTanget is also not defined
#endif
*/



// Main Function
//////////////////

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
	
	
	
	#ifdef REQUIRES_NORMAL
		// determine the correct normal. for back facing fragments the normal has to be flipped. care
		// has to be taken if the rendering is mirrored. in this case the front facing is exactly the
		// opposite of what we are really looking for. The xor operator does exactly this
		vec3 realNormal = mix( -vNormal, vNormal, vec3( pFlipCulling ^^ gl_FrontFacing ) ); // mix( if-false, if-true, condition )
		
		// relief mapping. definition of macros has to be delied until here since undefine
		// symbols can lead to tricky situations resulting in compilers failing
		#ifndef HAS_TESSELLATION_SHADER
			vec2 tcReliefMapped = vTCColor;
			reliefMapping( tcReliefMapped, realNormal );
			
			#undef tcColor
			#define tcColor tcReliefMapped
			#ifdef TEXTURE_NORMAL
				#undef tcNormal
				#define tcNormal tcReliefMapped
			#endif
			#if defined TEXTURE_EMISSIVITY || defined TEXTURE_RIM_EMISSIVITY
				#undef tcEmissivity
				#define tcEmissivity tcReliefMapped
			#endif
		#endif
		
		#ifdef TEXTURE_NORMAL
			vec3 normal = TEXTURE( texNormal, tcNormal ).rgb;
			normal = normal * vec3( 1.9921569 ) + vec3( -0.9921722 );
		#else
			vec3 normal = realNormal; // (0,0,1) => realNormal
		#endif
		
		// normal and normal variance
		#ifdef TEXTURE_NORMAL
			normal = vTangent * vec3( normal.x ) + vBitangent * vec3( normal.y ) + realNormal * vec3( normal.z );
		#endif
		
		#ifdef TP_NORMAL_STRENGTH
			// mix() is not an option since the texture property can be negative or larger than 1 for special effects
			normal = ( normal - realNormal ) * vec3( pNormalStrength ) + realNormal;
		#endif
		
		// various hacks that should go away later on
		if( dot( normal, normal ) < 1e-6 ){
			normal = vec3( 0.0, 0.0, 1.0 );
		}
		
		// normalize is required for the later passes to work correctly
		normal = normalize( normal );
	#endif
	
	#ifdef TEXTURE_RIM_EMISSIVITY
		vec3 fragmentDirection = normalize( vReflectDir );
	#endif
	
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
		#ifdef WITH_OUTLINE
			vec3 emissivity = pOutlineEmissivity;
		#else
			vec3 emissivity = vec3( 0.0 );
			#ifdef TEXTURE_EMISSIVITY
				emissivity += TEXTURE( texEmissivity, tcColor ).rgb;
			#endif
			#ifdef TEXTURE_RIM_EMISSIVITY
				if( pRimAngle > 0.5 ){
					// deoglSkinTexture: pRimAngle = angle > 0.001 ? 1 / ( angle * pi/2 ) : 0
					// for "angle = 0.001 .. 1" we have "pRimAngle = 0.637 .. 636.62". hence 0.5 as threshold
					// 
					// using "normal" is not giving the results one expects especially if close up.
					// instead the normal is dotted with the normalized fragment direction.
					emissivity += pow( TEXTURE( texRimEmissivity, tcEmissivity ).rgb, vec3( pColorGamma ) )
						* pRimEmissivityIntensity
						* vec3( max( 1.0 - pow( asin( abs( dot( fragmentDirection, normal ) ) )
							* pRimAngle, pRimExponent ), 0.0 ) );
				}
			#endif
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
