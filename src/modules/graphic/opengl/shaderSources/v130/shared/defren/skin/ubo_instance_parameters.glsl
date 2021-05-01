#ifdef SHARED_SPB

struct sInstanceParameters{
	mat4x3 pSPBMatrixModel;
	mat3 pSPBMatrixNormal;  // component
	mat3x2 pSPBMatrixTexCoord;  // component
	
	vec4 pSPBBillboardPosTransform; // multiply{X,Y}, offset{X,Y}
	bvec3 pSPBBillboardParams; // locked, spherical, sizeFixedToScreen
	vec4 pSPBSamplesParams; // particle: scaleU, offsetU, scaleV, offsetV
	
	vec2 pSPBHeightTerrainMaskTCTransform; // height-map: multiply
	ivec2 pSPBHeightTerrainMaskSelector; // height-map: arrayLayer, componentIndex
	
	vec2 pSPBVariationSeed; // component
	float pSPBBurstFactor; // particle
	int pSPBRibbonSheetCount; // particle: ribbon or beam
	
	float pSPBPropFieldParams; // prop-field: bendFactor
	bool pSPBDoubleSided; // component
	float pSPBEnvMapFade; // component
	// padding 1 float
	
	/*
	vec4 pSPBTCTransformColor;
	vec4 pSPBTCTransformNormal;
	vec4 pSPBTCTransformReflectivity;
	vec4 pSPBTCTransformEmissivity;
	vec4 pSPBTCTransformRefractionDistort;
	vec4 pSPBTCTransformAO;
	*/
	
	vec3 pSPBInstColorTint; // color.tint
	float pSPBInstColorGamma; // color.gamma
	
	float pSPBInstColorSolidityMultiplier; // color.solidity.multiplier
	float pSPBInstTransparencyMultiplier; // transparency.multiplier
	float pSPBInstSolidityMultiplier; // solidity.multiplier
	float pSPBInstAOSolidityMultiplier; // ambient.occlusion.solidity.multiplier
	
	vec2 pSPBInstHeightRemap; // height.scale, height.offset
	float pSPBInstNormalStrength; // normal.strength
	float pSPBInstNormalSolidityMultiplier; // normal.solidity.multiplier
	
	vec2 pSPBInstRoughnessRemap; // roughness.remap.* (x=scale, y=offset)
	float pSPBInstRoughnessGamma; // roughness.gamma
	float pSPBInstRoughnessSolidityMultiplier; // roughness.solidity.multiplier
	
	vec3 pSPBInstEmissivityIntensity; // emissivity.intensity
	float pSPBInstRefractionDistortStrength; // refraction.distort.strength
	
	vec3 pSPBInstEnvRoomEmissivityIntensity; // environmentroom.emissivity.intensity
	float pSPBInstReflectivitySolidityMultiplier; // reflectivity.solidity.multiplier
	
	vec3 pSPBInstEnvRoomOffset; // environmentroom.offset
	float pSPBInstReflectivityMultiplier; // reflectivity.multiplier
	
	vec2 pSPBInstEnvRoomSize; // environmentroom.size
	vec2 pSPBInstVariationEnableScale; // variation.u, variation.v
	
	vec3 pSPBInstRimEmissivityIntensity; // rim.emissivity.intensity
	float pSPBInstRimAngle; // rim.angle
	float pSPBInstRimExponent; // rim.exponent
	
	vec3 pSPBInstOutlineColor;
	float pSPBInstOutlineThickness;
	vec3 pSPBInstOutlineColorTint;
	float pSPBInstOutlineSolidity;
	vec3 pSPBInstOutlineEmissivity;
	vec3 pSPBInstOutlineEmissivityTint;
	
	#ifdef SHARED_SPB_PADDING
	vec4 _pSPBPadding[ SHARED_SPB_PADDING ];
	#endif
};

// with UBO the size has to be a constant (the definition is complicated but basically
// it boils down to anything but constants working reliably). SSBOs on the other hand
// can be of any size
#ifdef SHARED_SPB_USE_SSBO
	#ifdef ANDROID
		UBOLAYOUT_BIND(0) readonly buffer InstanceParametersSSBO
	#else
		UBOLAYOUT readonly buffer InstanceParametersSSBO
	#endif
#else
	UBOLAYOUT uniform InstanceParameters
#endif
{
	#ifdef SHARED_SPB_ARRAY_SIZE
	sInstanceParameters pSharedSPB[ SHARED_SPB_ARRAY_SIZE ];
	#else
	sInstanceParameters pSharedSPB[];
	#endif
};



#else  // SHARED_SPB

UBOLAYOUT uniform InstanceParameters{
	#ifdef PARTICLE
		mat4x3 pMatrixModel;
		vec4 pSamplesParams; // samplesScaleU, samplesOffsetU, samplesScaleV, samplesOffsetV
		float pBurstFactor;
		#if defined PARTICLE_RIBBON || defined PARTICLE_BEAM
			int pRibbonSheetCount;
		#endif
		
		#define pDoubleSided true
		#ifdef WITH_VARIATIONS
			#define pVariationSeed vec2(0)
		#endif
		
	#else  // PARTICLE
		// component properties. the same for all textures in a component
		mat4x3 pMatrixModel;
		mat3 pMatrixNormal;
		
		// per texture properties. potentially different for each texture in a component
		mat3x2 pMatrixTexCoord;
		#ifdef PROP_FIELD
			float pPropFieldParams; // r=bendFactor
		#endif
		#ifdef HEIGHT_MAP
			vec2 pHeightTerrainMaskTCTransform; // multiply
			ivec2 pHeightTerrainMaskSelector; // arrayLayer, componentIndex
		#endif
		bool pDoubleSided;
		float pEnvMapFade;
		#ifdef WITH_VARIATIONS
			vec2 pVariationSeed;
		#endif
		
		// billboard, imposter
		vec4 pBillboardPosTransform; // multiplyX, multiplyY, offsetX, offsetY
		bvec3 pBillboardParams; // locked, spherical, sizeFixedToScreen
	#endif  // PARTICLE
	
	// texture coordinate transformations used for dynamic textures only
	/*
	vec4 pTCTransformColor;
	#ifdef TEXTURE_NORMAL
		vec4 pTCTransformNormal;
	#endif
	#ifdef TEXTURE_REFLECTIVITY
		vec4 pTCTransformReflectivity;
	#endif
	#ifdef TEXTURE_EMISSIVITY
		vec4 pTCTransformEmissivity;
	#endif
	#ifdef TEXTURE_REFRACTION_DISTORT
		vec4 pTCTransformRefractionDistort;
	#endif
	#ifdef TEXTURE_AO
		vec4 pTCTransformAO;
	#endif
	*/
	
	// per texture dynamic texture properties not using textures
	#ifdef DYNAMIC_COLOR_TINT
		vec3 pInstColorTint; // color.tint
	#endif
	#ifdef DYNAMIC_COLOR_GAMMA
		float pInstColorGamma; // color.gamma
	#endif
	#ifdef DYNAMIC_COLOR_SOLIDITY_MULTIPLIER
		float pInstColorSolidityMultiplier; // color.solidity.multiplier
	#endif
	#ifdef DYNAMIC_TRANSPARENCY_MULTIPLIER
		float pInstTransparencyMultiplier; // transparency.multiplier
	#endif
	#ifdef DYNAMIC_SOLIDITY_MULTIPLIER
		float pInstSolidityMultiplier; // solidity.multiplier
	#endif
	#ifdef DYNAMIC_AO_SOLIDITY_MULTIPLIER
		float pInstAOSolidityMultiplier; // ambient.occlusion.solidity.multiplier
	#endif
	#ifdef DYNAMIC_HEIGHT_REMAP
		vec2 pInstHeightRemap; // height.scale, height.offset
	#endif
	#ifdef DYNAMIC_NORMAL_STRENGTH
		float pInstNormalStrength; // normal.strength
	#endif
	#ifdef DYNAMIC_NORMAL_SOLIDITY_MULTIPLIER
		float pInstNormalSolidityMultiplier; // normal.solidity.multiplier
	#endif
	#ifdef DYNAMIC_ROUGHNESS_REMAP
		vec2 pInstRoughnessRemap; // roughness.remap.* (x=scale, y=offset)
	#endif
	#ifdef DYNAMIC_ROUGHNESS_GAMMA
		float pInstRoughnessGamma; // roughness.gamma
	#endif
	#ifdef DYNAMIC_ROUGHNESS_SOLIDITY_MULTIPLIER
		float pInstRoughnessSolidityMultiplier; // roughness.solidity.multiplier
	#endif
	#ifdef DYNAMIC_REFRACTION_DISTORT_STRENGTH
		float pInstRefractionDistortStrength; // refraction.distort.strength
	#endif
	#ifdef DYNAMIC_REFLECTIVITY_SOLIDITY_MULTIPLIER
		float pInstReflectivitySolidityMultiplier; // reflectivity.solidity.multiplier
	#endif
	#ifdef DYNAMIC_EMISSIVITY_INTENSITY
		vec3 pInstEmissivityIntensity; // emissivity.intensity
	#endif
	#ifdef DYNAMIC_ENVROOM_SIZE
		vec2 pInstEnvRoomSize; // environmentroom.size
	#endif
	#ifdef DYNAMIC_ENVROOM_OFFSET
		vec3 pInstEnvRoomOffset; // environmentroom.offset
	#endif
	#ifdef DYNAMIC_ENVROOM_EMISSIVITY_INTENSITY
		vec3 pInstEnvRoomEmissivityIntensity; // environmentroom.emissivity.intensity
	#endif
	#ifdef DYNAMIC_VARIATION
		vec2 pInstVariationEnableScale; // variation.u, variation.v
	#endif
	#ifdef DYNAMIC_REFLECTIVITY_MULTIPLIER
		float pInstReflectivityMultiplier; // reflectivity.multiplier
	#endif
	#ifdef DYNAMIC_RIM_EMISSIVITY_INTENSITY
		vec3 pInstRimEmissivityIntensity; // rim.emissivity.intensity
	#endif
	#ifdef DYNAMIC_RIM_ANGLE
		vec3 pInstRimAngle; // rim.angle
	#endif
	#ifdef DYNAMIC_RIM_EXPONENT
		vec3 pInstRimExponent; // rim.exponent
	#endif
	#ifdef DYNAMIC_OUTLINE_COLOR
		vec3 pInstOutlineColor;
	#endif
	#ifdef DYNAMIC_OUTLINE_COLOR_TINT
		vec3 pInstOutlineColorTint;
	#endif
	#ifdef DYNAMIC_OUTLINE_THICKNESS
		float pInstOutlineThickness;
	#endif
	#ifdef DYNAMIC_OUTLINE_SOLIDITY
		float pInstOutlineSolidity;
	#endif
	#ifdef DYNAMIC_OUTLINE_EMISSIVITY
		vec3 pInstOutlineEmissivity;
	#endif
	#ifdef DYNAMIC_OUTLINE_EMISSIVITY_TINT
		vec3 pInstOutlineEmissivityTint;
	#endif
};

#endif  // SHARED_SPB
