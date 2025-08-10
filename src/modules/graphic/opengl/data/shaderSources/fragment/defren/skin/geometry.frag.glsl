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

// OutputMode == OutputModeMaterial
layout(location=0) out vec4 outMaterialDiffuse; // diffuse.r, diffuse.g, diffuse.b, transparency
layout(location=1) out vec4 outMaterialNormal; // normal.x, normal.y, normal.z, blend
layout(location=2) out vec4 outMaterialReflectivity; // reflectivity.r, reflectivity.g, reflectivity.b, blend
layout(location=3) out vec4 outMaterialRoughness; // roughness, n/a, n/a, blend
layout(location=4) out vec4 outMaterialAOSolidity; // ao, ssao, solidity, blend
layout(location=5) out vec4 outMaterialSubSurface; // subsurface.rgb, blend
layout(location=6) out vec4 outMaterialColor; // color.r, color.g, color.b, n/a

// OutputMode == OutputModeMaterialReduced
#define outMaterialReducedDiffuse outMaterialDiffuse
#define outMaterialReducedNormal outMaterialNormal
#define outMaterialReducedReflectivity outMaterialReflectivity
#define outMaterialReducedColor outMaterialRoughness

// OutputMode == OutputModeLuminance
#define outLuminanceLuminance outMaterialDiffuse.rgb // luminance, n/a, n/a
#define outLuminanceNormal outMaterialNormal.rgb // normal.x, normal.y, normal.z

// OutputMode == OutputModeColor || OutputMode == OutputModeDefault
#define outDefaultColor outMaterialDiffuse // color.r, color.g, color.b, n/a


// Constants
//////////////

const vec3 unpackDepth = vec3(1.0, 1.0 / 256.0, 1.0 / 65536.0);

const vec4 cemefac = vec4(0.5, 1.0, -0.1591549, -0.3183099); // 0.5, 1.0, -1/2pi, -1/pi

const vec4 colorTransparent = vec4(0.0, 0.0, 0.0, 1.0);

const vec3 lumiFactors = vec3(0.2125, 0.7154, 0.0721);
//const vec3 lumiFactors = vec3(0.3086, 0.6094, 0.0820); // nVidia


#include "shared/normal/texture.glsl"
#include "shared/normal/zero.glsl"

#include "shared/interface/skin/variation.glsl"
#include "shared/interface/skin/emissivity.glsl"

#include "shared/defren/skin/relief_mapping.glsl"
#include "shared/defren/skin/environment_room.glsl"
#include "shared/defren/skin/nonpbr_metalness.glsl"


// Main Function
//////////////////

void main(void){
	// for height map quickly dicard fragments if the mask is 0
	if(GeometryMode == GeometryModeHeightMap && vHTMask < 0.001){
		discard;
	}
	
	// discard fragments beyond render range
	/*
	if(FadeOutRange && vFadeZ > pFadeRange.y){
		discard;
	}
	*/
	
	// calculate the screen space texture coordinates
	ivec2 tc = ivec2(gl_FragCoord.xy);
	
	// test against depth texture
	if(DepthTest != DepthTestNone){
		float depthTestValue = texelFetch(texDepthTest, ivec3(tc, vLayer), 0).r;
		bool condition;
		
		if(InverseDepth){
			if(DepthTest == DepthTestLarger){
				condition = gl_FragCoord.z < depthTestValue;
				
			}else{
				condition = gl_FragCoord.z > depthTestValue;
			}
			
		}else{
			if(DepthTest == DepthTestLarger){
				condition = gl_FragCoord.z > depthTestValue;
				
			}else{
				condition = gl_FragCoord.z < depthTestValue;
			}
		}
		
		if(condition){
			discard;
		}
	}
	
	
	
	// determine the correct normal. for back facing fragments the normal has to be flipped. care
	// has to be taken if the rendering is mirrored. in this case the front facing is exactly the
	// opposite of what we are really looking for. The xor operator does exactly this
	vec3 realNormal = mix(-vNormal, vNormal, vec3(pFlipCulling ^^ gl_FrontFacing)); // mix(if-false, if-true, condition)
	/*vec3 realNormal = vNormal;
	
	if(! gl_FrontFacing){
		realNormal = -realNormal;
	}*/
	
	// relief mapping. definition of macros has to be delied until here since undefine
	// symbols can lead to tricky situations resulting in compilers failing
	vec2 tcColor, tcColorTintMask, tcNormal, tcReflectivity, tcEmissivity,
		tcRefractionDistort, tcAO, tcAbsorption;
	
	if(WithTessellationShader){
		tcColor = vTCColor;
		tcColorTintMask = vTCColorTintMask;
		tcNormal = vTCNormal;
		tcReflectivity = vTCReflectivity;
		tcEmissivity = vTCEmissivity;
		tcRefractionDistort = vTCRefractionDistort;
		tcAO = vTCAO;
		tcAbsorption = tcColor;
		
	}else{
		vec2 tcReliefMapped = vTCColor;
		reliefMapping(tcReliefMapped, realNormal, vSPBIndex);
		
		tcColor = tcReliefMapped;
		tcColorTintMask = tcReliefMapped;
		tcNormal = tcReliefMapped;
		tcReflectivity = tcReliefMapped;
		tcEmissivity = tcReliefMapped;
		tcRefractionDistort = tcReliefMapped;
		tcAO = tcReliefMapped;
		tcAbsorption = tcReliefMapped;
	}
	
	
	
	// get texture properties from textures
	vec4 color = vec4(0.0, 0.0, 0.0, 1.0), normal;
	vec3 reflectivity, nonpbrAlbedo;
	vec2 distort;
	float solidity, ao, roughness, nonpbrMetalness;
	
	if(OutputMode != OutputModeLuminance){
		// non pbr
		if(TextureNonPbrAlbedo || TextureNonPbrMetalness){
			nonpbrAlbedo = vec3(0.0);
			if(TextureNonPbrAlbedo){
				nonpbrAlbedo = vec3(TEXTURE(texNonPbrAlbedo, tcColor));
			}
			
			nonpbrMetalness = 0.0;
			if(TextureNonPbrMetalness){
				nonpbrMetalness = TEXTURE(texNonPbrMetalness, tcReflectivity).r;
			}
		}
		
		
		// color
		if(TextureTransparency){
			color.a = TEXTURE(texTransparency, tcColor).r;
		}
		if(TextureNonPbrAlbedo || TextureNonPbrMetalness){
			color.rgb = nonpbrMetalnessToColor(nonpbrAlbedo, nonpbrMetalness);
			
		}else if(TextureColor){
			if(TextureTransparency){
				color.rgb = vec3(TEXTURE(texColor, tcColor));
				
			}else{
				color = TEXTURE(texColor, tcColor);
			}
			
		}else if(WithOutline){
			color.rgb = getOutlineColor(vSPBIndex) * getOutlineColorTint(vSPBIndex);
		}
		color.a *= getTransparencyMultiplier(vSPBIndex);
		
		// for height map adjust alpha value
		if(GeometryMode == GeometryModeHeightMap){
			color.a *= vHTMask;
		}
		
		
		// solidity
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
		
		if(MaskedSolidity){
			// ensure calculation of depth and geometry matches
			solidity = solidity < 0.35 ? 0.0 : solidity;
		}
		
		
		// normal
		if(TextureNormal){
			normal = TEXTURE(texNormal, tcNormal);
			normal.xyz = normal.rgb * vec3(1.9921569) + vec3(-0.9921722);
			
		}else{
			normal = vec4(realNormal, 0.0); // (0,0,1) => realNormal
		}
		
		
		// ambient occlusion
		if(TextureAO){
			ao = TEXTURE(texAO, tcAO).r;
			
		}else{
			ao = 1.0;
		}
		
		
		// reflectivity
		if(TextureNonPbrAlbedo || TextureNonPbrMetalness){
			reflectivity = nonpbrMetalnessToReflectivity(nonpbrAlbedo, nonpbrMetalness);
			
		}else if(TextureReflectivity){
			reflectivity = vec3(TEXTURE(texReflectivity, tcReflectivity))
				* vec3(getDynamicReflectivityMultiplier(vSPBIndex));
			
		}else{
			reflectivity = vec3(0.0);
		}
		
		
		// roughness
		if(TextureRoughness){
			roughness = TEXTURE(texRoughness, tcReflectivity).r;
			
		}else{
			roughness = 1.0;
		}
		
		
		// refraction distortion
		if(TextureRefractionDistort){
			distort = vec2(TEXTURE(texRefractionDistort, tcRefractionDistort)) * vec2(2.0) + vec2(-1.0);
			distort *= pScreenSpace.xy * vec2(getRefractionDistortStrength(vSPBIndex));
		}
	}
	
	
	
	// environment room replaces the diffuse component
	vec3 envRoomDir, envRoomColor;
	float envRoomMask = 1.0;
	
	if(TextureEnvRoom || TextureEnvRoomEmissivity){
		envRoomDir = calcEnvRoomDir(tcColor, realNormal, vSPBIndex);
		
		if(OutputMode != OutputModeLuminance){
			if(TextureEnvRoom){
				envRoomColor = vec3(textureLod(texEnvRoom, envRoomDir, 0.0)) * getEnvRoomTint(vSPBIndex);
				
			}else{
				envRoomColor = vec3(0.0);
			}
			
			if(TextureEnvRoomMask){
				envRoomMask = TEXTURE(texEnvRoomMask, tcColor).r;
				color.rgb = mix(color.rgb, envRoomColor, vec3(envRoomMask));
				
			}else{
				color.rgb = envRoomColor;
			}
		}
	}
	/*
	if(TextureEnvRoom || TextureEnvRoomEmissivity){
		if(AnyOutputModeMaterial){
			outMaterialDiffuse = vec4(0.0, 0.0, 0.0, 0.0);
			outMaterialNormal = vec4(normalZeroMaterialEnc, 0.0);
			outMaterialReflectivity = vec4(0.0, 0.0, 0.0, 0.0);
			outMaterialRoughness = vec4(0.0, 1.0, 1.0, 0.0);
		}
		return;
	}
	*/
	
	
	// color, ambient occlusion and masked solidity
	vec4 writeColor;
	
	if(OutputMode != OutputModeLuminance){
		if(!WithEmissivity){
			if(MaskedSolidity){
				if(solidity < 0.35){
					discard;
				}
				solidity = 1.0;
				
			// in the case of direct rendering values of 0 indicate pixels to not render.
			// this is a hack right now since there is no better way yet to handle this situation
			}else if(!AnyOutputModeMaterial){
				if(solidity < 0.001){
					discard;
				}
			}
		}
		
		color.rgb = pow(color.rgb, vec3(getColorGamma(vSPBIndex)));
		if(GeometryMode == GeometryModeParticle){
			color *= vParticleColor;
			
		}else if(TextureColorTintMask){
			color.rgb = mix(color.rgb, color.rgb * getColorTint(vSPBIndex),
				TEXTURE(texColorTintMask, tcColorTintMask).r);
			
		}else{
			color.rgb *= getColorTint(vSPBIndex);
		}
		
		color.a *= solidity;
		
		
		if(AnyOutputModeMaterial){
			vec4 writeDiffuse = color;
			if(SolidityMultiplier){
				writeDiffuse.a *= getColorSolidityMultiplier(vSPBIndex);
			}
			
			if(OutputMode == OutputModeMaterial){
				outMaterialDiffuse = writeDiffuse;
				
			}else if(OutputMode == OutputModeMaterialReduced){
				outMaterialReducedDiffuse = writeDiffuse;
			}
		}
		
		
		if(AmbientLightProbe){
			//writeColor = color * vec4(textureLod(texEnvMap, -vReflectDir, 12.0).rgb, 1.0);
			writeColor = color * vec4(pParticleLightHack, 1.0);
			
		}else{
			writeColor = color * pAmbient;
		}
		
		if(TextureRenderColor){
			writeColor.rgb *= vec3(color.a);
		}
		
		
		ao = pow(ao, getColorGamma(vSPBIndex)); // this is a hack and has to be replaced with a proper ambient.occlusion.gamma texture property
	}
	
	
	
	// normal and normal variance
	if(OutputMode != OutputModeLuminance){
		if(TextureNormal){
			normal.xyz = vTangent * vec3(normal.x) + vBitangent * vec3(normal.y) + realNormal * vec3(normal.z);
		}
		
		if(TPNormalStrength){
			// mix() is not an option since the texture property can be negative or larger than 1 for special effects
			normal.xyz = (normal.xyz - realNormal) * vec3(getNormalStrength(vSPBIndex)) + realNormal;
			normal.w *= abs(getNormalStrength(vSPBIndex));
		}
		
		// various hacks that should go away later on
		if(dot(normal.xyz, normal.xyz) < 1e-6){
			normal = vec4(0.0, 0.0, 1.0, 0.0);
		}
		
		// normalize is required for the later passes to work correctly
		normal.xyz = normalize(normal.xyz);
		
		if(AnyOutputModeMaterial){
			vec4 writeNormal;
			
			if(WithOutline){
				writeNormal = vec4(normalZeroMaterialEnc, color.a);
				
			}else{
				writeNormal = vec4(normalEncodeMaterial(normal.xyz), color.a);
				if(SolidityMultiplier){
					writeNormal.a *= getNormalStrengthMultiplier(vSPBIndex);
				}
			}
			
			if(OutputMode == OutputModeMaterial){
				outMaterialNormal = writeNormal;
				
			}else if(OutputMode == OutputModeMaterialReduced){
				outMaterialReducedNormal = writeNormal;
			}
		}
	}
	
	
	
	// reflection and refraction
	vec3 envMapDir, envMapReflectivity, fragmentDirection;
	
	if(!WithOutline && OutputMode != OutputModeLuminance){
		if(AnyOutputModeMaterial || TextureEnvMap){
			reflectivity.rgb = pow(reflectivity.rgb, vec3(getColorGamma(vSPBIndex)));
			roughness = pow(clamp(roughness, 0.0, 1.0), getRoughnessGamma(vSPBIndex));
			if(TPRoughnessRemap){
				roughness = clamp(roughness * getRoughnessRemap(vSPBIndex).x + getRoughnessRemap(vSPBIndex).y, 0.0, 1.0);
			}
			if(UseNormalRoughnessCorrection){
				roughness = min(roughness + normal.w * pNorRoughCorrStrength, 1.0); // apply normal variance as roughness increase
			}
		}
		
		if(TextureEnvMap || TextureRimEmissivity){
			fragmentDirection = normalize(vReflectDir);
		}
		
		if(TextureEnvMap){
			float reflectDot = min(abs(dot(-fragmentDirection, normal.xyz)), 1.0);
			envMapDir = pMatrixEnvMap * vec3(reflect(fragmentDirection, normal.xyz));
			
			//float fresnelReduction = smoothstep(0.5, 1.0, 1.0 - roughness);
			//float fresnelReduction = 1.0 / (1.0 + roughness * 3.0); // CoD version... no less than 25% reflectivity at fully grazing angle
			//float fresnelReduction = 1.0 - roughness;
			
			// pow(5) fits betters but pow(4) is cheaper:
			//float fresnelReduction = pow(1.0 - roughness, 5.0);
		//	vec3 fresnelReduction = vec3(pow(1.0 - roughness, 5.0) * 0.9524 + 0.0476);
			vec3 fresnelReduction = mix(reflectivity.rgb, vec3(1.0), pow(1.0 - roughness, 5.0));
		//	vec3 fresnelReduction = mix(mix(vec3(0.056), vec3(1.0), reflectivity.rgb), vec3(1.0), pow(1.0 - roughness, 5.0));
			
			// reduce reflectivity depending on AO and roughness angles
			//float aoAngle = 1.0 - acos(1.0 - ao) * 0.636620; // * 2/pi
			//fresnelReduction *=  1.0 - smoothstep(aoAngle - roughness, aoAngle + roughness, reflectDot);
			// WARNING! the mix function is very fragile and passing under 0 or over 1 by even just a small
			//          amount results in NaN values and other nasty stuff. clamping costs some cycles
			//          but it prevents the mix function from smuggling NaN values into the image
			fresnelReduction *= vec3(clamp(
				(acos(1.0 - ao) + roughness * 1.5707963 - acos(reflectDot) + 0.01)
					/ max(roughness * 3.14159265, 0.01),
				0.0, 1.0));
			vec3 fresnelFactor = vec3(clamp(pow(1.0 - reflectDot, 5.0), 0.0, 1.0));
			envMapReflectivity = mix(reflectivity.rgb, vec3(1.0), vec3(fresnelFactor))
				* fresnelReduction * vec3(solidity);
// 			fresnelFactor *= fresnelReduction;
// 			
// 			vec4 fresnelFactorWithAlpha = vec4(fresnelFactor, dot(fresnelFactor, vec3(1.0 / 3.0))) * vec4(solidity);
// 			color = mix(color, colorTransparent, fresnelFactorWithAlpha);
// 			if(TextureRenderColor){
//				writeColor = mix(writeColor, colorTransparent, fresnelFactorWithAlpha);
// 			}
			
			if(GeometryMode == GeometryModeParticle){
				envMapReflectivity *= vParticleColor.a;
			}
		}
	}
	
	if(TextureRenderColor){
		vec4 renderColor;
		if(TextureRefractionDistort && !WithOutline){
			renderColor = textureLod(texRenderColor, vec3(clamp(
				gl_FragCoord.xy * pScreenSpace.zw + distort, pViewport.xy, pViewport.zw), vLayer), 0.0);
			
		}else{
			renderColor = texelFetch(texRenderColor, ivec3(tc, vLayer), 0);
		}
		
		writeColor.rgb = mix(vec3(renderColor), vec3(writeColor), color.a);
		writeColor.a = min(writeColor.a + renderColor.a, 1.0);
			// vec3(color.a) is a fake solution for true transmission(distanceThroughMaterial).rgb
		//writeColor = mix(writeColor, vec4(0.0, 0.0, 0.0, 1.0), vec4(fresnelFactor * fresnelReduction));
	}
	
	
	if(OutputMode == OutputModeColor || OutputMode == OutputModeDefault){
		// for direct rendering apply normals as pre-baked lighting
		writeColor.rgb *= vec3(0.25 - clamp(normal.z, -1.0, 0.0) * 0.75);
		
		// for direct rendering the transparency and solidity has to be premultiplied to
		// allow emissivity to properly apply. this is due to use of blend mode (1,1-alpha):
		//   f = d*(1-a) + (s*a + e)*1
		writeColor.rgb *= vec3(writeColor.a * solidity);
			// this does not work correctly with particles having solid appearance in
			// volumetric-mode
	}
	
	if(!WithOutline && OutputMode != OutputModeLuminance && TextureEnvMap){
		if(SkinReflections || pSkinDoesReflections){
			float envMapLodLevel = log2(1.0 + pEnvMapLodLevel * roughness);
			vec3 reflectedColor;
			
			envMapDir = normalize(envMapDir);
			
			if(TextureEnvMapEqui){
				vec2 tcEnvMap = cemefac.xy + cemefac.zw * vec2(
					atan(envMapDir.x, envMapDir.z), acos(envMapDir.y));
				
				reflectedColor = textureLod(texEnvMap, tcEnvMap, envMapLodLevel).rgb;
				if(TextureEnvMapFade && GeometryMode != GeometryModeParticle){
					reflectedColor = mix(textureLod(texEnvMapFade, tcEnvMap, envMapLodLevel).rgb,
						reflectedColor, vec3(pEnvMapFade));
				}
				
			}else{
				reflectedColor = textureLod(texEnvMapCube, envMapDir, envMapLodLevel).rgb;
				if(TextureEnvMapFade && GeometryMode != GeometryModeParticle){
					reflectedColor = mix(textureLod(texEnvMapFadeCube, envMapDir, envMapLodLevel).rgb,
						reflectedColor, vec3(pEnvMapFade));
				}
			}
			
			// this should simulate to some degree a prefiltered environment map (cosine filter, phong lobe).
			//reflectedColor *= vec3((cos(roughness * 3.1416) * 0.5 + 0.5) * 0.38 + 0.62);
			// this version is based on a normalized situation and should be somewhat better
		//	reflectedColor *= vec3(0.2 + 0.8 / (1.0 + 50.0 * roughness * roughness));
			
			writeColor.rgb += reflectedColor * envMapReflectivity;
		}
	}
	
	if(AnyOutputModeMaterial){
		vec4 writeReflectivity, writeRoughness, writeAOSolidity;
		
		if(WithOutline){
			writeReflectivity = vec4(0.0, 0.0, 0.0, color.a);
			writeRoughness = vec4(1.0, 1.0, 1.0, color.a);
			writeAOSolidity = vec4(1.0, 1.0, solidity, color.a);
			
		}else{
			writeReflectivity = vec4(reflectivity.rgb, color.a);
			writeRoughness = vec4(roughness, 1.0, 1.0, color.a);
			writeAOSolidity = vec4(ao, 1.0, solidity, color.a);
			
			if(SolidityMultiplier){
				writeReflectivity.a *= getReflectivitySolidityMultiplier(vSPBIndex);
				writeRoughness.a *= getRoughnessSolidityMultiplier(vSPBIndex);
			}
		}
		
		if(OutputMode == OutputModeMaterial){
			outMaterialReflectivity = writeReflectivity;
			outMaterialRoughness = writeRoughness;
			outMaterialAOSolidity = writeAOSolidity;
			
		}else if(OutputMode == OutputModeMaterialReduced){
			outMaterialReducedReflectivity = writeReflectivity;
		}
	}
	
	
	
	// absorption and sub-surf scattering
	if(AnyOutputModeMaterial){
		if(WithOutline){
			outMaterialSubSurface = vec4(0.0, 0.0, 0.0, color.a);
			
		}else{
			outMaterialSubSurface = vec4(vec3(pTexAbsorptionRange), color.a);
			if(TextureAbsorption){
				// hack, needs pAbsorptionGamma
				outMaterialSubSurface.rgb *= pow(vec3(TEXTURE(texAbsorption, tcAbsorption)),
					vec3(getColorGamma(vSPBIndex)));
			}
		}
	}
	
	
	
	// emissivity
	if(WithOutline){
		writeColor.rgb += getOutlineEmissivity(vSPBIndex) * getOutlineEmissivityTint(vSPBIndex);
		
	}else{
		if(TextureEmissivity){
			vec3 scale;
			if(GeometryMode == GeometryModeParticle){
				scale = vParticleColor.rgb * finalEmissivityIntensity(vParticleEmissivity);
				
			}else{
				scale = vec3(finalEmissivityIntensity(getEmissivityIntensity(vSPBIndex)));
			}
			writeColor.rgb += pow(vec3(TEXTURE(texEmissivity, tcEmissivity)),
				vec3(getColorGamma(vSPBIndex))) * scale;
		}
		
		if(TextureEnvRoomEmissivity){
			writeColor.rgb += vec3(textureLod(texEnvRoomEmissivity, envRoomDir, 0.0))
				* finalEmissivityIntensity(getEnvRoomEmissivityIntensity(vSPBIndex))
				* vec3(envRoomMask);
		}
		
		if(TextureRimEmissivity){
			if(getRimAngle(vSPBIndex) > 0.5){
				// deoglSkinTexture: getRimAngle(vSPBIndex) = angle > 0.001 ? 1 / (angle * pi/2) : 0
				// for "angle = 0.001 .. 1" we have "getRimAngle(vSPBIndex) = 0.637 .. 636.62". hence 0.5 as threshold
				// 
				// using "normal" is not giving the results one expects especially if close up.
				// instead the normal is dotted with the normalized fragment direction.
				writeColor.rgb += pow(vec3(TEXTURE(texRimEmissivity, tcEmissivity)),
					vec3(getColorGamma(vSPBIndex)))
						* finalEmissivityIntensity(getRimEmissivityIntensity(vSPBIndex))
						* vec3(max(1.0 - pow(asin(abs(dot(fragmentDirection, normal.xyz)))
						* getRimAngle(vSPBIndex), getRimExponent(vSPBIndex)), 0.0));
			}
		}
	}
	
	
	// writeColor.rgb = vec3(vHTMask);
	
	
	
	// fade fragment near render range
	/* produces artifacts with lighting since alpha-blend does not work in this case
	if(FadeOutRange){
		writeColor.a *= 1.0 - clamp(vFadeZ, pFadeRange.x, pFadeRange.y) * pFadeRange.z;
	}
	*/
	
	
	if(OutputMode == OutputModeMaterial){
		outMaterialColor = writeColor;
		
	}else if(OutputMode == OutputModeMaterialReduced){
		outMaterialReducedColor = writeColor;
		
	}else if(OutputMode == OutputModeColor || OutputMode == OutputModeDefault){
		outDefaultColor = writeColor;
		
	}else if(OutputMode == OutputModeLuminance){
		outLuminanceLuminance = vec3(dot(vec3(writeColor), lumiFactors));
		outLuminanceNormal = realNormal;
	}
}
