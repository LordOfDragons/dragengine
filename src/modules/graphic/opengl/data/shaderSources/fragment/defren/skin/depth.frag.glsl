#include "shared/preamble.glsl"
#include "shared/defren/skin/macros_geometry.glsl"

// Uniform Parameters
///////////////////////

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"
#include "shared/defren/skin/ubo_texture_parameters.glsl"
#include "shared/defren/skin/ubo_instance_parameters.glsl"
#include "shared/defren/skin/ubo_dynamic_parameters.glsl"


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

uniform lowp sampler2D texColor;
uniform lowp sampler2D texSolidity;
uniform lowp SAMPLER_2D texNormal;
uniform mediump SAMPLER_2D texEmissivity;
uniform mediump SAMPLER_2D texRimEmissivity;
uniform HIGHP sampler2DArray texDepthTest;


// Inputs
///////////

in vec2 vTCColor;
in vec2 vTCNormal;
in vec2 vTCEmissivity;
// in float vParticleEmissivity; // from curve property
in vec3 vClipCoord;
in vec3 vSkinClipCoord;
in vec3 vPosition;
in float vHTMask;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBitangent;
in vec3 vReflectDir;
in float vFadeZ;
flat in int vSPBIndex;
flat in int vLayer;

#define tcColor vTCColor
#define tcNormal vTCNormal
#define tcEmissivity vTCEmissivity
#define spbIndex vSPBIndex

#ifdef SHARED_SPB
	#include "shared/defren/skin/shared_spb_redirect.glsl"
#endif

#include "shared/defren/skin/shared_spb_texture_redirect.glsl"


// Outputs
////////////

#ifdef OUTPUT_CONSTANT
	layout(location=0) out float outConstant;
#endif
#ifdef OUTPUT_COLOR
	layout(location=0) out vec4 outColor;
#endif
#ifdef ENCODE_OUT_DEPTH
	layout(location=1) out vec4 outDepth;
#endif


// Constants
//////////////

const vec3 packShift = vec3(1.0, 256.0, 65536.0);
const vec3 packMask = vec3(1.0 / 256.0, 1.0 / 256.0, 0.0);


// Includes requiring inputs to be defined
////////////////////////////////////////////

#if defined REQUIRES_NORMAL && ! defined HAS_TESSELLATION_SHADER
	#include "shared/defren/skin/relief_mapping.glsl"
#endif

#include "shared/defren/depth_to_position.glsl"

/*
#if defined TEXTURE_ENVROOM || defined TEXTURE_ENVROOM_EMISSIVITY
#include "shared/defren/skin/environment_room.glsl"
// this is not going to work since vReflectDir is not defined and without normal map
// vTanget is also not defined
#endif
*/


float finalEmissivityIntensity(in float intensity){
	return mix(intensity, intensity * pCameraAdaptedIntensity, pEmissivityCameraAdapted);
}

vec3 finalEmissivityIntensity(in vec3 intensity){
	return mix(intensity, intensity * vec3(pCameraAdaptedIntensity), bvec3(pEmissivityCameraAdapted));
}


// Main Function
//////////////////

void main(void){
	// calculate depth if non-projective depth is used. this has to be done before any branching
	// because derivatives become undefined otherwise.
	#ifdef DEPTH_DISTANCE
		float depth = length(vPosition) * pDepthTransform.x + pDepthTransform.y;
		#ifdef DEPTH_OFFSET
// 			vec2 depthDeriv = vec2(dFdx(depth), dFdy(depth));
		#endif
		gl_FragDepth = depth;
	#endif
	
	// discard fragments beyond render range
	#ifdef FADEOUT_RANGE
		if(vFadeZ > pFadeRange.y){
			discard;
		}
	#endif
	
	#if defined DEPTH_OFFSET && defined DEPTH_ORTHOGONAL && NEVER_DEFINED
		/*if(gl_FrontFacing){
			gl_FragDepth += length(depthDeriv) * pDepthOffset[vLayer].x + pDepthOffset[vLayer].y;
			
		}else{
			gl_FragDepth += length(depthDeriv) * pDepthOffset[vLayer].z + pDepthOffset[vLayer].w;
		}*/
		
		vec2 depthOffset = mix(pDepthOffset[vLayer].zw, pDepthOffset[vLayer].xy,
			bvec2(gl_FrontFacing || pDoubleSided)); // mix(if-false, if-true, condition)
		gl_FragDepth += length(depthDeriv) * depthOffset.x + depthOffset.y;
	#endif
	
	// discard fragments using the clip plane
	#ifdef CLIP_PLANE
		if(dot(vClipCoord, pClipPlane[vLayer].xyz) <= pClipPlane[vLayer].w) discard;
	#endif
	
	
	
	#ifdef REQUIRES_NORMAL
		// determine the correct normal. for back facing fragments the normal has to be flipped. care
		// has to be taken if the rendering is mirrored. in this case the front facing is exactly the
		// opposite of what we are really looking for. The xor operator does exactly this
		vec3 realNormal = mix(-vNormal, vNormal, vec3(pFlipCulling ^^ gl_FrontFacing)); // mix(if-false, if-true, condition)
		
		// relief mapping. definition of macros has to be delied until here since undefine
		// symbols can lead to tricky situations resulting in compilers failing
		#if ! defined HAS_TESSELLATION_SHADER && defined REQUIRES_TEX_COLOR
			vec2 tcReliefMapped = vTCColor;
			reliefMapping(tcReliefMapped, realNormal);
			
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
			vec3 normal = TEXTURE(texNormal, tcNormal).rgb;
			normal = normal * vec3(1.9921569) + vec3(-0.9921722);
		#else
			vec3 normal = realNormal; // (0,0,1) => realNormal
		#endif
		
		// normal and normal variance
		#ifdef TEXTURE_NORMAL
			normal = vTangent * vec3(normal.x) + vBitangent * vec3(normal.y) + realNormal * vec3(normal.z);
		#endif
		
		#ifdef TP_NORMAL_STRENGTH
			// mix() is not an option since the texture property can be negative or larger than 1 for special effects
			normal = (normal - realNormal) * vec3(pNormalStrength) + realNormal;
		#endif
		
		// various hacks that should go away later on
		if(dot(normal, normal) < 1e-6){
			normal = vec3(0.0, 0.0, 1.0);
		}
		
		// normalize is required for the later passes to work correctly
		normal = normalize(normal);
	#endif
	
	#ifdef TEXTURE_RIM_EMISSIVITY
		vec3 fragmentDirection = normalize(vReflectDir);
	#endif
	
	// get texture properties from textures
	#ifdef OUTPUT_COLOR
		vec3 color = texture(texColor, tcColor).rgb;
	#endif
	
	#ifdef WITH_SOLIDITY
		float solidity;
		#ifdef TEXTURE_SOLIDITY
			solidity = texture(texSolidity, tcColor).r * pSolidityMultiplier;
		#elif defined WITH_OUTLINE
			solidity = pOutlineSolidity;
		#else
			solidity = pSolidityMultiplier;
		#endif
		
		#ifdef SKIN_CLIP_PLANE
			float skinClipDist = dot(vSkinClipCoord, vec3(pInstSkinClipPlaneNormal));
			
			float skinClipSolidity = pSkinClipPlaneBorder > 0
				? smoothstep(pInstSkinClipPlaneNormal.w, pInstSkinClipPlaneNormal.w + pSkinClipPlaneBorder, skinClipDist)
				: smoothstep(pInstSkinClipPlaneNormal.w + pSkinClipPlaneBorder, pInstSkinClipPlaneNormal.w, skinClipDist);
			
			solidity *= mix(1, skinClipSolidity, pSkinClipPlane);
		#endif
	#endif
	
	#ifdef WITH_EMISSIVITY
		#ifdef WITH_OUTLINE
			vec3 emissivity = pOutlineEmissivity;
		#else
			vec3 emissivity = vec3(0.0);
			#ifdef TEXTURE_EMISSIVITY
				emissivity += TEXTURE(texEmissivity, tcColor).rgb;
			#endif
			#ifdef TEXTURE_RIM_EMISSIVITY
				if(pRimAngle > 0.5){
					// deoglSkinTexture: pRimAngle = angle > 0.001 ? 1 / (angle * pi/2) : 0
					// for "angle = 0.001 .. 1" we have "pRimAngle = 0.637 .. 636.62". hence 0.5 as threshold
					// 
					// using "normal" is not giving the results one expects especially if close up.
					// instead the normal is dotted with the normalized fragment direction.
					emissivity += pow(TEXTURE(texRimEmissivity, tcEmissivity).rgb, vec3(pColorGamma))
						* finalEmissivityIntensity(pRimEmissivityIntensity)
						* vec3(max(1.0 - pow(asin(abs(dot(fragmentDirection, normal)))
							* pRimAngle, pRimExponent), 0.0));
				}
			#endif
		#endif
	#endif
	
	// discard fragments using masked solidity
	#ifdef WITH_SOLIDITY
		#ifdef MASKED_SOLIDITY
		if(solidity < 0.35)
		#else
		if(solidity < 0.001)
		#endif
		{
			#ifdef WITH_EMISSIVITY
				//vec2 tcReliefMapped = vTCColor;
				//reliefMapping(tcReliefMapped, realNormal);
				//#define tcColor tcReliefMapped
				//#define tcEmissivity tcReliefMapped
				
				if(all(lessThan(emissivity, vec3(0.001)))){
					discard;
				}
				solidity = 0.0;
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
	
	// discard against previous depth
	#ifdef DEPTH_TEST
		float depthTestValue = sampleDepth(texDepthTest, ivec3(gl_FragCoord.xy, vLayer));
		
		#ifdef INVERSE_DEPTH
			#ifdef DEPTH_TEST_LARGER
			if(fragmentDepth <= depthTestValue) discard;
			#else
			if(fragmentDepth >= depthTestValue) discard;
			#endif
		#else
			#ifdef DEPTH_TEST_LARGER
			if(fragmentDepth >= depthTestValue) discard;
			#else
			if(fragmentDepth <= depthTestValue) discard;
			#endif
		#endif
	#endif
	
	// encode the output depth
	#ifdef ENCODE_OUT_DEPTH
		vec3 encoded = fract(packShift * vec3(fragmentDepth));
		outDepth = vec4(encoded - (encoded.yzz * packMask), 1.0);
	#endif
	
	// output constant value or color
	#ifdef OUTPUT_CONSTANT
		outConstant = 1.0;
	#endif
	#ifdef OUTPUT_COLOR
		color = pow(color, vec3(pColorGamma));
		outColor = vec4(color, 1.0);
	#endif
}
