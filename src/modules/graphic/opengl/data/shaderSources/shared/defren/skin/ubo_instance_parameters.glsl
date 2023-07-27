#ifdef SHARED_SPB

struct sInstanceParameters{
	mat4x3 matrixModel;
	mat3 matrixNormal;  // component
	mat3x2 matrixTexCoord;  // component
	
	vec4 billboardPosTransform; // multiply{X,Y}, offset{X,Y}
	bvec3 billboardParams; // locked, spherical, sizeFixedToScreen
	vec4 samplesParams; // particle: scaleU, offsetU, scaleV, offsetV
	
	vec2 heightTerrainMaskTCTransform; // height-map: multiply
	ivec2 heightTerrainMaskSelector; // height-map: arrayLayer, componentIndex
	
	vec2 variationSeed; // component
	float burstFactor; // particle
	int ribbonSheetCount; // particle: ribbon or beam
	
	float propFieldParams; // prop-field: bendFactor
	bool doubleSided; // component
	float envMapFade; // component
	int indexSPBTexParams; // shared parameter block index for texture parameters
	
	/*
	vec4 tcTransformColor;
	vec4 tcTransformNormal;
	vec4 tcTransformReflectivity;
	vec4 tcTransformEmissivity;
	vec4 tcTransformRefractionDistort;
	vec4 tcTransformAO;
	*/
	
	vec3 instColorTint; // color.tint
	float instColorGamma; // color.gamma
	
	float instColorSolidityMultiplier; // color.solidity.multiplier
	float instTransparencyMultiplier; // transparency.multiplier
	float instSolidityMultiplier; // solidity.multiplier
	float instAOSolidityMultiplier; // ambient.occlusion.solidity.multiplier
	
	vec2 instHeightRemap; // height.scale, height.offset
	float instNormalStrength; // normal.strength
	float instNormalSolidityMultiplier; // normal.solidity.multiplier
	
	vec2 instRoughnessRemap; // roughness.remap.* (x=scale, y=offset)
	float instRoughnessGamma; // roughness.gamma
	float instRoughnessSolidityMultiplier; // roughness.solidity.multiplier
	
	vec3 instEmissivityIntensity; // emissivity.intensity
	float instRefractionDistortStrength; // refraction.distort.strength
	
	vec3 instEnvRoomEmissivityIntensity; // environmentroom.emissivity.intensity
	float instReflectivitySolidityMultiplier; // reflectivity.solidity.multiplier
	
	vec3 instEnvRoomOffset; // environmentroom.offset
	float instReflectivityMultiplier; // reflectivity.multiplier
	
	vec2 instEnvRoomSize; // environmentroom.size
	vec2 instVariationEnableScale; // variation.u, variation.v
	
	vec3 instRimEmissivityIntensity; // rim.emissivity.intensity
	float instRimAngle; // rim.angle
	float instRimExponent; // rim.exponent
	
	vec3 instOutlineColor;
	float instOutlineThickness;
	
	vec3 instOutlineColorTint;
	float instOutlineSolidity;
	
	vec3 instOutlineEmissivity;
	vec3 instOutlineEmissivityTint;
	
	vec4 instClipPlaneNormal; // clip.plane : xyz=normal, w=distance
	float instClipPlane; // clip.plane
	float instClipPlaneBorder; // clip.plane.border
	
	#ifdef SHARED_SPB_PADDING
	vec4 padding[ SHARED_SPB_PADDING ];
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
	
	int pIndexSPBTexParams; // shared parameter block index for texture parameters
	
	// texture coordinate transformations used for dynamic textures only
	/*
	vec4 pTCTransformColor;
	vec4 pTCTransformNormal;
	vec4 pTCTransformReflectivity;
	vec4 pTCTransformEmissivity;
	vec4 pTCTransformRefractionDistort;
	vec4 pTCTransformAO;
	*/
	
	// per texture dynamic texture properties not using textures
	vec3 pInstColorTint; // color.tint
	float pInstColorGamma; // color.gamma
	
	float pInstColorSolidityMultiplier; // color.solidity.multiplier
	float pInstTransparencyMultiplier; // transparency.multiplier
	float pInstSolidityMultiplier; // solidity.multiplier
	float pInstAOSolidityMultiplier; // ambient.occlusion.solidity.multiplier
	
	vec2 pInstHeightRemap; // height.scale, height.offset
	float pInstNormalStrength; // normal.strength
	float pInstNormalSolidityMultiplier; // normal.solidity.multiplier
	
	vec2 pInstRoughnessRemap; // roughness.remap.* (x=scale, y=offset)
	float pInstRoughnessGamma; // roughness.gamma
	float pInstRoughnessSolidityMultiplier; // roughness.solidity.multiplier
	
	float pInstRefractionDistortStrength; // refraction.distort.strength
	float pInstReflectivitySolidityMultiplier; // reflectivity.solidity.multiplier
	
	vec3 pInstEmissivityIntensity; // emissivity.intensity
	
	vec2 pInstEnvRoomSize; // environmentroom.size
	
	vec3 pInstEnvRoomOffset; // environmentroom.offset
	
	vec3 pInstEnvRoomEmissivityIntensity; // environmentroom.emissivity.intensity
	
	vec2 pInstVariationEnableScale; // variation.u, variation.v
	float pInstReflectivityMultiplier; // reflectivity.multiplier
	
	vec3 pInstRimEmissivityIntensity; // rim.emissivity.intensity
	float pInstRimAngle; // rim.angle
	float pInstRimExponent; // rim.exponent
	
	vec3 pInstOutlineColor;
	
	vec3 pInstOutlineColorTint;
	float pInstOutlineThickness;
	
	vec3 pInstOutlineEmissivity;
	float pInstOutlineSolidity;
	
	vec3 pInstOutlineEmissivityTint;
	
	vec4 pInstSkinClipPlaneNormal; // clip.plane : xyz=normal, w=distance
	float pInstSkinClipPlane; // clip.plane
	float pInstSkinClipPlaneBorder; // clip.plane.border
};

#ifdef PARTICLE
	const bool pDoubleSided = true;
	#ifdef WITH_VARIATIONS
		const vec2 pVariationSeed = vec2( 0.0 );
	#endif
#endif

#endif  // SHARED_SPB
