// Constants
//////////////

const float epsilon = 0.0001;

const float shadowThreshold = -0.0174524; // cos of 1 radians
const float shadowThresholdInv = 1.0 / 0.0174524;

const vec3 ambientLightFactor = vec3(0.25, 0.5, 0.25);


// Shadow casting
///////////////////

#define SHADOW_NOISE_TEXTURE texNoise
#include "shared/defren/light/shadow/shadow.glsl"
// #include "shared/defren/light/shadow/shadowScreenSpace.glsl"

const float defaultShadowThickness = 100.0; // just something large enough


// Lighting
/////////////

struct sFRFragment{
	vec3 position;
	vec4 diffuse;
	vec3 normal;
	vec3 reflectivity;
	float roughness;
	float ao;
	vec3 ambientColor;
};


// Light fragment for single light
vec3 frLighting(const in sFRLight light, const in sFRFragment fragment){
	// light constants
	bool isSky = light.type == LightModeSky;
	bool isPoint = light.type == LightModePoint;
	bool isSpot = light.type == LightModeSpot;
	bool isProjector = light.type == LightModeProjector;
	bool isSpotAny = isSpot || isProjector;
	bool isPointSpotAny = isPoint || isSpotAny;
	
	// calculate light direction and distance
	vec3 lightDir;
	float dist;
	
	if(isSky){
		lightDir = light.instance.lightView[vLayer];
		
	}else{
		lightDir = light.instance.lightPosition[vLayer] - fragment.position;
		dist = length(lightDir);
		
		// discard if pre-lit (length = 0) or outside the light range
		if(dist == 0.0 || dist > light.light.range){
			return vec3(0);
		}
		lightDir /= dist;
		
		// discard fragments outside the spot cone
		// NOTE this does not work anymore since spot cone can be squashed now
		/*
		if(isSpot){
			float spotFactor = dot(light.instance.lightView[vLayer], -lightDir) - light.light.spotBase;
			if(spotFactor <= 0.0){
				return vec3(0);
			}
		}
		*/
	}
	
	// shadow constants
	bool hasTexShadow1Solid = light.hasTextureShadow.x;
	bool hasTexShadow2Solid = light.hasTextureShadow.y;
	bool hasShadows = hasTexShadow1Solid || hasTexShadow2Solid;
	bool castShadows = hasShadows && GeometryMode != GeometryModeParticle;
	int arrayIndex = light.arrayIndex;
	int arrayIndexSolid1 = arrayIndex * 2;
	int arrayIndexSolid2 = arrayIndex * 2 + 1;
	
	// calculate shadow position if required
	float thicknessShadowScale;
	vec4 shapos1, shapos2;
	vec2 projectorConeRadius;
	float spotConeRadius;
	
	if(isSky){
		if(castShadows){
			if(fragment.position.z < light.instance.layerBorder.x){
				shapos1 = (light.instance.shadowMatrix1[vLayer] * vec4(fragment.position, 1)).stqp; // s(x),t(y),layer,distance(z)
				shapos1.p = 0.0; // layer 0
				if(pSSSSSEnabled){
					thicknessShadowScale = light.instance.shadowDepthTransform.z;
				}
				
			}else if(fragment.position.z < light.instance.layerBorder.y){
				shapos1 = (light.instance.shadowMatrix2[vLayer] * vec4(fragment.position, 1)).stqp; // s(x),t(y),layer,distance(z)
				shapos1.p = 1.0; // layer 1
				if(pSSSSSEnabled){
					thicknessShadowScale = light.instance.shadowDepthTransform.w;
				}
				
			}else if(fragment.position.z < light.instance.layerBorder.z){
				shapos1 = (light.instance.shadowMatrix3[vLayer] * vec4(fragment.position, 1)).stqp; // s(x),t(y),layer,distance(z)
				shapos1.p = 2.0; // layer 2
				if(pSSSSSEnabled){
					thicknessShadowScale = light.instance.shadowDepthTransform2.z;
				}
				
			}else{
				shapos1 = (light.instance.shadowMatrix4[vLayer] * vec4(fragment.position, 1)).stqp; // s(x),t(y),layer,distance(z)
				shapos1.p = 3.0; // layer 3
				if(pSSSSSEnabled){
					thicknessShadowScale = light.instance.shadowDepthTransform2.w;
				}
			}
			
			shapos2 = shapos1;
		}
		
	/*}else if(isParticle){
		shapos1 = vec4(position - vParticleLightPosition, 1);*/
		
	}else{ // point, spot or projector
		float shadowClamp = light.instance.lightParams.y;
		
		shapos1 = light.instance.shadowMatrix1[vLayer] * vec4(fragment.position, 1);
		
		if(isPoint){
			vec3 absshapos1 = abs(vec3(shapos1));
			float shaZ1 = max(max(absshapos1.x, absshapos1.y), max(absshapos1.z, shadowClamp));
			shapos1.q = (shaZ1 * light.instance.shadowDepthTransform.x + light.instance.shadowDepthTransform.y) / shaZ1;
			
			shapos2 = shapos1;
			
		}else{
			shapos1.pq = max(shapos1.pq, vec2(shadowClamp));
			shapos1.stp /= shapos1.q;
			
			if(isSpot){
				// discard fragments outside the spot cone. since spot cones can now be squashed
				// the shadow matrix texture coordinates have to be used to get a correct result.
				// this boils down to a circle test in the range 0..1 which is a simple check if
				// the point is more than 0.5 units away from the center. this is always correct
				// the same no matter how squashed the cone is
				spotConeRadius = length(shapos1.st - vec2(0.5));
				if(spotConeRadius > 0.5){
					return vec3(0);
				}
				
			}else{
				// discard fragments outide the projector frustum
				projectorConeRadius = abs(shapos1.st - vec2(0.5));
				if(any(greaterThan(projectorConeRadius, vec2(0.5)))){
					return vec3(0);
				}
			}
			
			shapos1.q = shapos1.p;
			shapos1.p = float(arrayIndexSolid1);
			
			shapos2 = shapos1;
			shapos2.p = float(arrayIndexSolid2);
		}
	}
	
	// normal
	vec3 normal = fragment.normal;
	if(dot(normal, normal) < 0.0001){
		normal = lightDir; // 0-normal means always point towards light source
	}
	
	// calculate the sss thickness from the shadow map if existing
	/*
	float largestAbsorptionRadius, shadowThickness;
	vec3 absorptionRadius;
	
	if(pSSSSSEnabled){
		absorptionRadius = texelFetch(texSubSurface, tcArray, 0).rgb;
		largestAbsorptionRadius = max(max(absorptionRadius.x, absorptionRadius.y), absorptionRadius.z);
		shadowThickness = defaultShadowThickness;
		
		if(castShadows){
			if(hasTexShadow1Solid){
				if(isPoint){
					shadowThickness = shadowCubeArrayThickness(texLightDepth1Cube,
						shapos1, arrayIndexSolid1, light.instance.shadowDepthTransform.zw);
					
				}else{
					shadowThickness = shadowMapArrayThickness(texLightDepth1Array, thicknessShadowScale, shapos1);
				}
			}
			
			if(hasTexShadow2Solid){
				if(isPoint){
					shadowThickness = shadowCubeArrayThickness(texLightDepth2Cube,
						shapos2, arrayIndexSolid2, light.instance.shadowDepthTransform.zw);
					
				}else{
					shadowThickness = shadowMapArrayThickness(texLightDepth2Array, thicknessShadowScale, shapos2);
				}
			}
		}
	}
	*/
	
	// calculate shadow color of required
	// NOTE the clamp() call is unfortunately mandatory. small imprecisions in GPU calculation
	//      can cause dot() to output a value which is slightly outside the range totally
	//      upsetting the upcoming calculationsTextureShadow1Solid
	float dotval = clamp(dot(normal, lightDir), -1.0, 1.0);
	
	/*
	float absorptionDot = max(-dotval, 0.0); // pSSSSSEnabled
	if(pSSSSSEnabled){
		float v = abs(dotval);
		v *= v;
		v *= v; // pow(1-abs(dot), 4)
		absorptionDot *= v;
	}
	*/
	
	vec3 fullShadowColor = vec3(1.0), shadowColor = vec3(1.0);
	
	if(castShadows){
		float shadow = dotval > 0.0 ? 1.0 : 0.0;
		
		if(shadow >= epsilon){
			if(hasTexShadow1Solid){
				float shadow2;
				if(isSky){
					shadow2 = evalShadowMapArray(texFRShadowSky, light.instance.shadow1Solid, shapos1, light.pcfMode);
					
				}else if(isPoint){
					shadow2 = evalShadowCubeArray(texFRShadowPoint,
						arrayIndexSolid1, light.instance.shadow1Solid, shapos1, light.pcfMode);
					
				}else{
					shadow2 = evalShadowMapArray(texFRShadowSpot, light.instance.shadow1Solid, shapos1, light.pcfMode);
				}
				shadow = min(shadow, shadow2);
			}
			
			if(hasTexShadow2Solid){
				float shadow2;
				if(isSky){
					shadow2 = evalShadowMapArray(texFRShadowSky, light.instance.shadow2Solid, shapos2, light.pcfMode);
					
				}else if(Shadow2Mode == ShadowModeCube){
					shadow2 = evalShadowCubeArray(texFRShadowPoint,
						arrayIndexSolid2, light.instance.shadow2Solid, shapos2, light.pcfMode);
					
				}else{
					shadow2 = evalShadowMapArray(texFRShadowSpot, light.instance.shadow2Solid, shapos2, light.pcfMode);
				}
				shadow = min(shadow, shadow2);
			}
			
			/*
			// screen space shadow castin is not working in forward rendering since this would
			// requires sampling from the depth texture which is used for depth testing. this
			// can cause problems with gpu drivers. another approach is calculating screen space
			// shadows in a separate pass like deoglRenderLights::RenderAO() but the problem is
			// that screen space shadows are different depending on the light direction. for this
			// reason screen space shadows are disabled in forward rendering
			if(shadow > epsilon && dotval > 0.0){
				shadow *= screenSpaceShadow(texDepth, position, lightDir);
			}
			*/
			
			if(isSky){
				// temporary until sky light is improved. required since ambientRatio is
				// not defined for sky light which would result in strange errors if this not
				// initialized value is used
				fullShadowColor = vec3(shadow); // required for specular reflection
				
			}else{
				fullShadowColor = vec3(shadow); // required for specular reflection
				shadow = mix(light.light.ambientRatio, 1.0, shadow);
			}
		}
		
		// discard the fragment if it is fully in shadows and there is no ambient light
		if(!isSky && shadow < epsilon){
			bool discardFragment = true;
			/*if(WithSubsurface){
				discardFragment = shadowThickness > largestAbsorptionRadius;
			}*/
			if(discardFragment){
				return vec3(0);
			}
		}
		
		shadowColor = vec3(shadow);
	}
	
	// for calculation information see light.frag.glsl
	float ao = fragment.ao;
	float ssao = 1.0;
	
	// merge the texture-ao with the ssao. use the minimum of the two to avoid over-occluding
	// if both are used. the result is stored in aoSolidity.g . this way aoSolidity.r contains
	// the pure texture-ao and aoSolidity.gb the combined ao
	//ssao = texelFetch(texAOSolidity, tc, 0).g;
	//ao = min(ao, ssao);
	
	// specular term
	float ap = 426.0 * (1.0 - fragment.roughness) / (90.0 * fragment.roughness * fragment.roughness + fragment.roughness + 0.001) + 1.0;
	vec3 halfDir = normalize(lightDir - normalize(fragment.position));
	float specNormTerm = (ap + 2.0) / 8.0;
	float specPowTerm = pow(clamp(dot(normal, halfDir), 0.0, 0.99), ap);
	vec3 specFresnelTerm = mix(fragment.reflectivity, vec3(1.0), vec3(pow(clamp(1.0 - dot(lightDir, halfDir), 0.0, 1.0), 5.0)));
	
	dotval = max(dotval, 0.0);
	
	if(castShadows){
		specFresnelTerm *= fullShadowColor;
	}
	vec3 finalColorSurface = clamp(vec3(specNormTerm * specPowTerm) * specFresnelTerm, vec3(0.0), vec3(1.0));
	
	// light color taking into account light color, light image and shadow. attenuation is handled separately
	vec3 lightColor = light.light.color;
	
	/*
	if(TextureLightColor == TextureLightColor2D){
		vec2 ltc = shapos1.st;
		ltc.y = 1.0 - ltc.y;
		RCTLCR(lightColor *= pow(texture(texColor, ltc).rgb, vec3(light.light.imageGamma)))
		
	}else if(TextureLightColor == TextureLightColorCube){
		vec3 ltc = normalize(light.instance.lightImageOmniMatrix[vLayer] * vec4(position, 1.0));
		RCTLCC(lightColor *= pow(texture(texColorCubemap, ltc).rgb, vec3(light.light.imageGamma)))
		
	}else if(TextureLightColor == TextureLightColorEquirect){
		vec2 ltc = equirectFromNormal(normalize(light.instance.lightImageOmniMatrix[vLayer] * vec4(position, 1.0)));
		RCTLCE(lightColor *= pow(texture(texColorEquirect, ltc).rgb, vec3(light.light.imageGamma)))
	}
	*/
	
	// WithSubsurface
	vec3 absorptionLightColor = lightColor;
	/*if(LightMode == LightModeParticle){
		lightColor *= vec3(dotval);
		
	}else{*/
		lightColor *= vec3(mix(light.light.ambientRatio, 1.0, dotval));
	//}
	if(castShadows){
		lightColor *= shadowColor;
	}
	
	vec3 finalColorAmbient;
	
	lightColor *= vec3(clamp(((acos(1.0 - ao) - acos(dotval)) * pAOSelfShadow.y) + 1.0, pAOSelfShadow.x, 1.0));
	if(isSky){
		finalColorAmbient = light.light.colorAmbient * vec3(ssao);
	}
	
	// distance and spot attenuation
	if(!isSky){
		/*if(LightMode == LightModeParticle){
			dist /= vParticleLightRange; // map all ranges into the 1m range so the same attenuation parameters can be used
		}*/
		float attenuation = max(light.light.dampingCoefficient
			/ (1.0 + light.light.attenuationCoefficient * dist * dist)
			+ light.light.dampingThreshold, 0.0);
		
		if(isSpot){
			attenuation *= pow(clamp(spotConeRadius * light.light.spotFactor
				+ light.light.spotBase, 0.001, 1.0), float(light.light.spotExponent));
			
		}else if(isProjector){
			vec2 spotConeAtten = pow(clamp(
				projectorConeRadius * vec2(light.light.spotFactor) + vec2(light.light.spotBase),
				vec2(0.001), vec2(1.0)), vec2(light.light.spotExponent));
			attenuation *= spotConeAtten.x;
			attenuation *= spotConeAtten.y;
		}
		
		lightColor *= vec3(attenuation);
		if(isSky){
			finalColorAmbient *= vec3(attenuation);
		}
		/*if(WithSubsurface){
			absorptionLightColor *= vec3(attenuation);
		}*/
	}
	
	// final light contribution
	finalColorSurface *= lightColor;
	
	vec3 finalColorSubSurface = lightColor;
	if(isSky){
		finalColorSubSurface += finalColorAmbient;
	}
	
	//outLuminance = vec4(vec3(dot(finalColorSubSurface + finalColorSurface, lumiFactors)), diffuse.a);
	
	/*
	if(WithSubsurface){
		outColor = vec4(finalColorSurface, diffuse.a);
		outSubSurface = vec4(finalColorSubSurface, diffuse.a);
		
		vec3 scatDist = vec3(shadowThickness) / max(absorptionRadius, vec3(0.001));
		vec3 scatAdd = exp(scatDist * vec3(-5.0)) * vec3(absorptionDot) * absorptionLightColor;
		outSubSurface.rgb += scatAdd * step(vec3(0.001), absorptionRadius);
		
	}else{
		outColor = vec4(finalColorSubSurface * diffuse.rgb + finalColorSurface, diffuse.a);
	}
	*/
	return finalColorSubSurface * fragment.diffuse.rgb + finalColorSurface;
}


// Light fragment using all lights
vec3 frLighting(const in sFRFragment fragment){
	vec3 color = fragment.diffuse.rgb * fragment.ambientColor;
	int i;
	for(i=0; i<pFRLightCount; i++){
		color += frLighting(pFRLights[i], fragment);
	}
	return color;
}
