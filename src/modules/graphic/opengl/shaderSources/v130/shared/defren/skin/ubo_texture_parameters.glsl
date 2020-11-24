UBOLAYOUT uniform TextureParameters{
	// static values for texture properties typically using a texture
	vec4 pValueColorTransparency; // color, transparency
	vec4 pValueNormal; // normal
	vec4 pValueReflectivityRoughness; // reflectivity, roughness
	vec2 pValueRefractionDistort; // refraction.distort
	float pValueSolidity; // solidity
	float pValueAO; // ambient.occlusion
	
	// texture property values not using textures
	vec3 pTexColorTint; // color.tint
	float pTexColorGamma; // color.gamma
	
	float pTexColorSolidityMultiplier; // color.solidity.multiplier
	float pTexAOSolidityMultiplier; // ambient.occlusion.solidity.multiplier
	float pTexSolidityMultiplier; // solidity.multiplier
	float pTexAbsorptionRange; // absorption.range
	
	vec2 pTexHeightRemap; // height.scale, height.offset
	float pTexNormalStrength; // normal.strength
	float pTexNormalSolidityMultiplier; // normal.solidity.multiplier
	
	vec2 pTexRoughnessRemap; // roughness.remap.* (x=scale, y=offset)
	float pTexRoughnessGamma; // roughness.gamma
	float pTexRoughnessSolidityMultiplier; // roughness.solidity.multiplier
	
	vec2 pTexEnvRoomSize; // environmentroom.size
	float pTexRefractionDistortStrength; // refraction.distort.strength
	float pTexReflectivitySolidityMultiplier; // reflectivity.solidity.multiplier
	
	vec3 pTexEnvRoomOffset; // environmentroom.offset
	float pTexTransparencyMultiplier; // transparency.multiplier
	
	vec3 pTexEnvRoomEmissivityIntensity; // environmentroom.emissivity.intensity
	float pTexThickness; // thickness
	
	vec3 pTexEmissivityIntensity; // emissivity.intensity
	float pTexReflectivityMultiplier; // reflectivity.multiplier
	
	vec2 pTexVariationEnableScale; // variation.u, variation.v
	int pTexParticleSheetCount; // particle.sheets
	float pTexRimAngle; // rim.angle
	
	vec3 pTexRimEmissivityIntensity; // rim.emissivity.intensity
	float pTexRimExponent; // rim.exponent
	
	vec3 pTexOutlineColor;
	float pTexOutlineThickness;
	vec3 pTexOutlineColorTint;
	float pTexOutlineSolidity;
	vec3 pTexOutlineEmissivity;
	vec3 pTexOutlineEmissivityTint;
	
	bool pTexEmissivityCameraAdapted;
};

#ifdef BUG_UBO_DIRECT_LINK_DEAD_LOOP
	#define pValueColorTransparency vec4(pValueColorTransparency)
	#define pValueNormal vec4(pValueNormal)
	#define pValueReflectivityRoughness vec4(pValueReflectivityRoughness)
	#define pValueRefractionDistort vec2(pValueRefractionDistort)
	#define pValueSolidity float(pValueSolidity)
	#define pValueAO float(pValueAO)
	
	#define pTexColorTint vec3(pTexColorTint)
	#define pTexColorGamma float(pTexColorGamma)
	
	#define pTexColorSolidityMultiplier float(pTexColorSolidityMultiplier)
	#define pTexAOSolidityMultiplier float(pTexAOSolidityMultiplier)
	#define pTexSolidityMultiplier float(pTexSolidityMultiplier)
	#define pTexAbsorptionRange float(pTexAbsorptionRange)
	
	#define pTexHeightRemap vec2(pTexHeightRemap)
	#define pTexNormalStrength float(pTexNormalStrength)
	#define pTexNormalSolidityMultiplier float(pTexNormalSolidityMultiplier)
	
	#define pTexRoughnessRemap vec2(pTexRoughnessRemap)
	#define pTexRoughnessGamma float(pTexRoughnessGamma)
	#define pTexRoughnessSolidityMultiplier float(pTexRoughnessSolidityMultiplier)
	
	#define pTexEnvRoomSize vec2(pTexEnvRoomSize)
	#define pTexRefractionDistortStrength float(pTexRefractionDistortStrength)
	#define pTexReflectivitySolidityMultiplier float(pTexReflectivitySolidityMultiplier)
	
	#define pTexEnvRoomOffset vec3(pTexEnvRoomOffset)
	#define pTexTransparencyMultiplier float(pTexTransparencyMultiplier)
	
	#define pTexEnvRoomEmissivityIntensity vec3(pTexEnvRoomEmissivityIntensity)
	#define pTexThickness float(pTexThickness)
	
	#define pTexEmissivityIntensity vec3(pTexEmissivityIntensity)
	#define pTexReflectivityMultiplier float(pTexReflectivityMultiplier)
	
	#define pTexVariationEnableScale vec2(pTexVariationEnableScale)
	#define pTexParticleSheetCount int(pTexParticleSheetCount)
	#define pTexRimAngle float(pTexRimAngle)
	
	#define pTexRimEmissivityIntensity vec3(pTexRimEmissivityIntensity)
	#define pTexRimExponent float(pTexRimExponent)
	
	#define pTexOutlineColor vec3(pTexOutlineColor)
	#define pTexOutlineThickness float(pTexOutlineThickness)
	#define pTexOutlineColorTint vec3(pTexOutlineColorTint)
	#define pTexOutlineSolidity float(pTexOutlineSolidity)
	#define pTexOutlineEmissivity vec3(pTexOutlineEmissivity)
	#define pTexOutlineEmissivityTint vec3(pTexOutlineEmissivityTint)
	
	#define pTexEmissivityCameraAdapted bool(pTexEmissivityCameraAdapted)
#endif
