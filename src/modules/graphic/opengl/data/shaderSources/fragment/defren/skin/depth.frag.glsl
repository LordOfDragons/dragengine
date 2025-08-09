#include "shared/preamble.glsl"
#include "shared/defren/skin/macros_geometry.glsl"

// Uniform Parameters
///////////////////////

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"
#include "shared/defren/skin/ubo_texture_parameters.glsl"
#include "shared/defren/skin/ubo_instance_parameters.glsl"


// Samplers
/////////////

#include "shared/interface/skin/samplers.glsl"


// Inputs
///////////

#include "shared/interface/skin/fragment.glsl"

#ifdef SHARED_SPB
	#define VAR_SPB_INDEX vSPBIndex
	#include "shared/defren/skin/shared_spb_redirect.glsl"
#endif

#include "shared/defren/skin/shared_spb_texture_redirect.glsl"
#include "shared/defren/skin/ubo_dynamic_parameters.glsl"


// Outputs
////////////

// OutputMode == OutputModeConstant
#define outConstant outColor.r

// OutputMode == OutputModeColor
layout(location=0) out vec4 outColor;

// EncodeOutDepth
layout(location=1) out vec4 outDepth;


// Constants
//////////////

const vec3 packShift = vec3(1.0, 256.0, 65536.0);
const vec3 packMask = vec3(1.0 / 256.0, 1.0 / 256.0, 0.0);


// Includes requiring inputs to be defined
////////////////////////////////////////////

// WithTessellationShader && RequireNormal
#include "shared/defren/skin/relief_mapping.glsl"

#include "shared/defren/depth_to_position.glsl"

/*
// TextureEnvRoom || TextureEnvRoomEmissivity
#include "shared/defren/skin/environment_room.glsl"
// this is not going to work since vReflectDir is not defined and without normal map
// vTanget is also not defined
*/

#include "shared/interface/skin/variation.glsl"
#include "shared/interface/skin/emissivity.glsl"

// Main Function
//////////////////

void main(void){
	vec2 tcColor = vTCColor;
	vec2 tcNormal = vTCNormal;
	vec2 tcEmissivity = vTCEmissivity;
	
	// calculate depth if non-projective depth is used. this has to be done before any branching
	// because derivatives become undefined otherwise.
	if(DepthDistance){
		float depth = length(vPosition) * pDepthTransform.x + pDepthTransform.y;
#if 0
		if(DepthOffset){
// 			vec2 depthDeriv = vec2(dFdx(depth), dFdy(depth));
		}
#endif
		gl_FragDepth = depth;
	}
	
	// discard fragments beyond render range
	if(FadeOutRange && vFadeZ > pFadeRange.y){
		discard;
	}
	
#if 0
	if(DepthOffset && DepthOrthogonal){
		/*if(gl_FrontFacing){
			gl_FragDepth += length(depthDeriv) * pDepthOffset[vLayer].x + pDepthOffset[vLayer].y;
			
		}else{
			gl_FragDepth += length(depthDeriv) * pDepthOffset[vLayer].z + pDepthOffset[vLayer].w;
		}*/
		
		vec2 depthOffset = mix(pDepthOffset[vLayer].zw, pDepthOffset[vLayer].xy,
			bvec2(gl_FrontFacing || pDoubleSided)); // mix(if-false, if-true, condition)
		gl_FragDepth += length(depthDeriv) * depthOffset.x + depthOffset.y;
	}
#endif
	
	// discard fragments using the clip plane
	if(ClipPlane){
		if(dot(vClipCoord, pClipPlane[vLayer].xyz) <= pClipPlane[vLayer].w){
			discard;
		}
	}
	
	
	vec3 realNormal, normal;
	
	if(RequireNormal){
		// determine the correct normal. for back facing fragments the normal has to be flipped. care
		// has to be taken if the rendering is mirrored. in this case the front facing is exactly the
		// opposite of what we are really looking for. The xor operator does exactly this
		realNormal = mix(-vNormal, vNormal, vec3(pFlipCulling ^^ gl_FrontFacing)); // mix(if-false, if-true, condition)
		
		// relief mapping. definition of macros has to be delied until here since undefine
		// symbols can lead to tricky situations resulting in compilers failing
		if(!WithTessellationShader && RequireTextureColor){
			vec2 tcReliefMapped = vTCColor;
			reliefMapping(tcReliefMapped, realNormal, vSPBIndex);
			
			tcColor = tcReliefMapped;
			if(TextureNormal){
				tcNormal = tcReliefMapped;
			}
			if(TextureEmissivity || TextureRimEmissivity){
				tcEmissivity = tcReliefMapped;
			}
		}
		
		if(TextureNormal){
			normal = TEXTURE(texNormal, tcNormal).rgb;
			normal = normal * vec3(1.9921569) + vec3(-0.9921722);
			
		}else{
			normal = realNormal; // (0,0,1) => realNormal
		}
		
		// normal and normal variance
		if(TextureNormal){
			normal = vTangent * vec3(normal.x) + vBitangent * vec3(normal.y) + realNormal * vec3(normal.z);
		}
		
		if(TPNormalStrength){
			// mix() is not an option since the texture property can be negative or larger than 1 for special effects
			normal = (normal - realNormal) * vec3(getNormalStrength(vSPBIndex)) + realNormal;
		}
		
		// various hacks that should go away later on
		if(dot(normal, normal) < 1e-6){
			normal = vec3(0.0, 0.0, 1.0);
		}
		
		// normalize is required for the later passes to work correctly
		normal = normalize(normal);
	}
	
	vec3 fragmentDirection;
	if(TextureRimEmissivity){
		fragmentDirection = normalize(vReflectDir);
	}
	
	// get texture properties from textures
	vec3 color;
	if(OutputMode == OutputModeColor){
		color = vec3(TEXTURE(texColor, tcColor));
	}
	
	float solidity;
	if(WithSolidity){
		if(TextureSolidity){
			solidity = TEXTURE(texSolidity, tcColor).r * getSolidityMultiplier(vSPBIndex);
			
		}else if(WithOutline){
			solidity = getOutlineSolidity(vSPBIndex);
			
		}else{
			solidity = getSolidityMultiplier(vSPBIndex);
		}
		
		if(SkinClipPlane){
			float skinClipDist = dot(vSkinClipCoord, vec3(pInstSkinClipPlaneNormal));
			
			float skinClipSolidity = getSkinClipPlaneBorder(vSPBIndex) > 0
				? smoothstep(pInstSkinClipPlaneNormal.w,
					pInstSkinClipPlaneNormal.w + getSkinClipPlaneBorder(vSPBIndex),
					skinClipDist)
				: smoothstep(pInstSkinClipPlaneNormal.w + getSkinClipPlaneBorder(vSPBIndex),
					pInstSkinClipPlaneNormal.w,
					skinClipDist);
			
			solidity *= mix(1, skinClipSolidity, getSkinClipPlane(vSPBIndex));
		}
	}
	
	vec3 emissivity;
	if(WithEmissivity){
		if(WithOutline){
			emissivity = getOutlineEmissivity(vSPBIndex);
			
		}else{
			emissivity = vec3(0.0);
			if(TextureEmissivity){
				emissivity += vec3(TEXTURE(texEmissivity, tcColor));
			}
			if(TextureRimEmissivity){
				if(getRimAngle(vSPBIndex) > 0.5){
					// deoglSkinTexture: getRimAngle(vSPBIndex) = angle > 0.001 ? 1 / (angle * pi/2) : 0
					// for "angle = 0.001 .. 1" we have "getRimAngle(vSPBIndex) = 0.637 .. 636.62". hence 0.5 as threshold
					// 
					// using "normal" is not giving the results one expects especially if close up.
					// instead the normal is dotted with the normalized fragment direction.
					emissivity += pow(TEXTURE(texRimEmissivity, tcEmissivity).rgb, vec3(getColorGamma(vSPBIndex)))
						* finalEmissivityIntensity(getRimEmissivityIntensity(vSPBIndex))
						* vec3(max(1.0 - pow(asin(abs(dot(fragmentDirection, normal)))
							* getRimAngle(vSPBIndex), getRimExponent(vSPBIndex)), 0.0));
				}
			}
		}
	}
	
	// discard fragments using masked solidity
	if(WithSolidity){
		if(solidity < (MaskedSolidity ? 0.35 : 0.001)){
			if(!WithEmissivity){
				discard;
			}
			
			//vec2 tcReliefMapped = vTCColor;
			//reliefMapping(tcReliefMapped, realNormal, vSPBIndex);
			//#define tcColor tcReliefMapped
			//#define tcEmissivity tcReliefMapped
			
			if(all(lessThan(emissivity, vec3(0.001)))){
				discard;
			}
			solidity = 0.0;
		}
	}
	
	// determine where the depth is coming from. this is different depending if projective depth is used or not
	float fragmentDepth;
	if(DepthOrthogonal || DepthDistance){
		fragmentDepth = gl_FragDepth;
		
	}else{
		fragmentDepth = gl_FragCoord.z;
	}
	
	// discard against previous depth
	if(DepthTest != DepthTestNone){
		float depthTestValue = sampleDepth(texDepthTest, ivec3(gl_FragCoord.xy, vLayer));
		bool condition;
		
		if(InverseDepth){
			if(DepthTest == DepthTestLarger){
				condition = fragmentDepth <= depthTestValue;
				
			}else{
				condition = fragmentDepth >= depthTestValue;
			}
			
		}else{
			if(DepthTest == DepthTestLarger){
				condition = fragmentDepth >= depthTestValue;
				
			}else{
				condition = fragmentDepth <= depthTestValue;
			}
		}
		
		if(condition){
			discard;
		}
	}
	
	// encode the output depth
	if(EncodeOutDepth){
		vec3 encoded = fract(packShift * vec3(fragmentDepth));
		outDepth = vec4(encoded - (encoded.yzz * packMask), 1.0);
	}
	
	// output constant value or color
	if(OutputMode == OutputModeConstant){
		outConstant = 1.0;
		
	}else if(OutputMode == OutputModeColor){
		color = pow(color, vec3(getColorGamma(vSPBIndex)));
		outColor = vec4(color, 1.0);
	}
}
