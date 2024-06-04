struct sTextureParameters{
	// static values for texture properties typically using a texture
	vec4 valueColorTransparency; // color, transparency
	vec4 valueNormal; // normal
	vec4 valueReflectivityRoughness; // reflectivity, roughness
	vec2 valueRefractionDistort; // refraction.distort
	float valueSolidity; // solidity
	float valueAO; // ambient.occlusion
	
	// texture property values not using textures
	vec3 texColorTint; // color.tint
	float texColorGamma; // color.gamma
	
	float texColorSolidityMultiplier; // color.solidity.multiplier
	float texAOSolidityMultiplier; // ambient.occlusion.solidity.multiplier
	float texSolidityMultiplier; // solidity.multiplier
	float texAbsorptionRange; // absorption.range
	
	vec2 texHeightRemap; // height.scale, height.offset
	float texNormalStrength; // normal.strength
	float texNormalSolidityMultiplier; // normal.solidity.multiplier
	
	vec2 texRoughnessRemap; // roughness.remap.* (x=scale, y=offset)
	float texRoughnessGamma; // roughness.gamma
	float texRoughnessSolidityMultiplier; // roughness.solidity.multiplier
	
	vec2 texEnvRoomSize; // environmentroom.size
	float texRefractionDistortStrength; // refraction.distort.strength
	float texReflectivitySolidityMultiplier; // reflectivity.solidity.multiplier
	
	vec3 texEnvRoomOffset; // environmentroom.offset
	float texTransparencyMultiplier; // transparency.multiplier
	
	vec3 texEnvRoomEmissivityIntensity; // environmentroom.emissivity.intensity
	float texThickness; // thickness
	
	vec3 texEmissivityIntensity; // emissivity.intensity
	float texReflectivityMultiplier; // reflectivity.multiplier
	
	vec2 texVariationEnableScale; // variation.u, variation.v
	int texParticleSheetCount; // particle.sheets
	float texRimAngle; // rim.angle
	
	vec3 texRimEmissivityIntensity; // rim.emissivity.intensity
	float texRimExponent; // rim.exponent
	
	vec3 texOutlineColor;
	float texOutlineThickness;
	vec3 texOutlineColorTint;
	float texOutlineSolidity;
	vec3 texOutlineEmissivity;
	vec3 texOutlineEmissivityTint;
	
	bool texEmissivityCameraAdapted;
	float texSkinClipPlane; // clip.plane
	float texSkinClipPlaneBorder; // clip.plane.border
	
	#ifdef SHARED_SPB_TEXTURE_PADDING
	vec4 padding[ SHARED_SPB_TEXTURE_PADDING ];
	#endif
};

// with UBO the size has to be a constant (the definition is complicated but basically
// it boils down to anything but constants working reliably). SSBOs on the other hand
// can be of any size
#ifdef SHARED_SPB_USE_SSBO
	#ifdef OS_ANDROID
		UBOLAYOUT_BIND(2) readonly buffer TextureParametersSSBO
	#else
		UBOLAYOUT readonly buffer TextureParametersSSBO
	#endif
#else
	UBOLAYOUT uniform TextureParameters
#endif
{
	#ifdef SHARED_SPB_TEXTURE_ARRAY_SIZE
	sTextureParameters pSharedSPBTexture[ SHARED_SPB_TEXTURE_ARRAY_SIZE ];
	#else
	sTextureParameters pSharedSPBTexture[];
	#endif
};
