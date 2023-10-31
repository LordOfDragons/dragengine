#ifdef PARTICLE
	#define pMatrixModel pSharedSPB[ spbIndex ].matrixModel
	#define pSamplesParams pSharedSPB[ spbIndex ].samplesParams
	#define pBurstFactor pSharedSPB[ spbIndex ].burstFactor
	#if defined PARTICLE_RIBBON || defined PARTICLE_BEAM
		#define pRibbonSheetCount pSharedSPB[ spbIndex ].ribbonSheetCount
	#endif
	
#else  // PARTICLE
	// component properties. the same for all textures in a component
	#define pMatrixModel pSharedSPB[ spbIndex ].matrixModel
	#define pMatrixNormal pSharedSPB[ spbIndex ].matrixNormal
	
	// per texture properties. potentially different for each texture in a component
	#define pMatrixTexCoord pSharedSPB[ spbIndex ].matrixTexCoord
	#ifdef PROP_FIELD
		#define pPropFieldParams pSharedSPB[ spbIndex ].propFieldParams
	#endif
	#ifdef HEIGHT_MAP
		#define pHeightTerrainMaskTCTransform pSharedSPB[ spbIndex ].heightTerrainMaskTCTransform
		#define pHeightTerrainMaskSelector pSharedSPB[ spbIndex ].heightTerrainMaskSelector
	#endif
	#define pDoubleSided pSharedSPB[ spbIndex ].doubleSided
	#define pEnvMapFade pSharedSPB[ spbIndex ].envMapFade
	#ifdef WITH_VARIATIONS
		#define pVariationSeed pSharedSPB[ spbIndex ].variationSeed
	#endif
	
	// imposter
	#define pBillboardPosTransform pSharedSPB[ spbIndex ].billboardPosTransform
	#define pBillboardParams pSharedSPB[ spbIndex ].billboardParams
#endif  // PARTICLE

#define pIndexSPBTexParams pSharedSPB[ spbIndex ].indexSPBTexParams

#define pInstSkinClipPlaneNormal pSharedSPB[ spbIndex ].instSkinClipPlaneNormal

// texture coordinate transformations used for dynamic textures only
/*
#define pTCTransformColor pSharedSPB[ spbIndex ].tcTransformColor
#ifdef TEXTURE_NORMAL
	#define pTCTransformNormal pSharedSPB[ spbIndex ].tcTransformNormal
#endif
#ifdef TEXTURE_REFLECTIVITY
	#define pTCTransformReflectivity pSharedSPB[ spbIndex ].tcTransformReflectivity
#endif
#ifdef TEXTURE_EMISSIVITY
	#define pTCTransformEmissivity pSharedSPB[ spbIndex ].tcTransformEmissivity
#endif
#ifdef TEXTURE_REFRACTION_DISTORT
	#define pTCTransformRefractionDistort pSharedSPB[ spbIndex ].tcTransformRefractionDistort
#endif
#ifdef TEXTURE_AO
	#define pTCTransformAO pSharedSPB[ spbIndex ].tcTransformAO
#endif
*/

// per texture dynamic texture properties not using textures
#ifdef DYNAMIC_COLOR_TINT
	#define pInstColorTint pSharedSPB[ spbIndex ].instColorTint
#endif
#ifdef DYNAMIC_COLOR_GAMMA
	#define pInstColorGamma pSharedSPB[ spbIndex ].instColorGamma
#endif
#ifdef DYNAMIC_COLOR_SOLIDITY_MULTIPLIER
	#define pInstColorSolidityMultiplier pSharedSPB[ spbIndex ].instColorSolidityMultiplier
#endif
#ifdef DYNAMIC_TRANSPARENCY_MULTIPLIER
	#define pInstTransparencyMultiplier pSharedSPB[ spbIndex ].instTransparencyMultiplier
#endif
#ifdef DYNAMIC_SOLIDITY_MULTIPLIER
	#define pInstSolidityMultiplier pSharedSPB[ spbIndex ].instSolidityMultiplier
#endif
#ifdef DYNAMIC_AO_SOLIDITY_MULTIPLIER
	#define pInstAOSolidityMultiplier pSharedSPB[ spbIndex ].instAOSolidityMultiplier
#endif
#ifdef DYNAMIC_HEIGHT_REMAP
	#define pInstHeightRemap pSharedSPB[ spbIndex ].instHeightRemap
#endif
#ifdef DYNAMIC_NORMAL_STRENGTH
	#define pInstNormalStrength pSharedSPB[ spbIndex ].instNormalStrength
#endif
#ifdef DYNAMIC_NORMAL_SOLIDITY_MULTIPLIER
	#define pInstNormalSolidityMultiplier pSharedSPB[ spbIndex ].instNormalSolidityMultiplier
#endif
#ifdef DYNAMIC_ROUGHNESS_REMAP
	#define pInstRoughnessRemap pSharedSPB[ spbIndex ].instRoughnessRemap
#endif
#ifdef DYNAMIC_ROUGHNESS_GAMMA
	#define pInstRoughnessGamma pSharedSPB[ spbIndex ].instRoughnessGamma
#endif
#ifdef DYNAMIC_ROUGHNESS_SOLIDITY_MULTIPLIER
	#define pInstRoughnessSolidityMultiplier pSharedSPB[ spbIndex ].instRoughnessSolidityMultiplier
#endif
#ifdef DYNAMIC_REFRACTION_DISTORT_STRENGTH
	#define pInstRefractionDistortStrength pSharedSPB[ spbIndex ].instRefractionDistortStrength
#endif
#ifdef DYNAMIC_REFLECTIVITY_SOLIDITY_MULTIPLIER
	#define pInstReflectivitySolidityMultiplier pSharedSPB[ spbIndex ].instReflectivitySolidityMultiplier
#endif
#ifdef DYNAMIC_EMISSIVITY_INTENSITY
	#define pInstEmissivityIntensity pSharedSPB[ spbIndex ].instEmissivityIntensity
#endif
#ifdef DYNAMIC_ENVROOM_SIZE
	#define pInstEnvRoomSize pSharedSPB[ spbIndex ].instEnvRoomSize
#endif
#ifdef DYNAMIC_ENVROOM_OFFSET
	#define pInstEnvRoomOffset pSharedSPB[ spbIndex ].instEnvRoomOffset
#endif
#ifdef DYNAMIC_ENVROOM_EMISSIVITY_INTENSITY
	#define pInstEnvRoomEmissivityIntensity pSharedSPB[ spbIndex ].instEnvRoomEmissivityIntensity
#endif
#ifdef DYNAMIC_VARIATION
	#define pInstVariationEnableScale pSharedSPB[ spbIndex ].instVariationEnableScale
#endif
#ifdef DYNAMIC_REFLECTIVITY_MULTIPLIER
	#define pInstReflectivityMultiplier pSharedSPB[ spbIndex ].instReflectivityMultiplier
#endif
#ifdef DYNAMIC_RIM_EMISSIVITY_INTENSITY
	#define pInstRimEmissivityIntensity pSharedSPB[ spbIndex ].instRimEmissivityIntensity
#endif
#ifdef DYNAMIC_RIM_ANGLE
	#define pInstRimAngle pSharedSPB[ spbIndex ].instRimAngle
#endif
#ifdef DYNAMIC_RIM_EXPONENT
	#define pInstRimExponent pSharedSPB[ spbIndex ].instRimExponent
#endif
#ifdef DYNAMIC_OUTLINE_COLOR
	#define pInstOutlineColor pSharedSPB[ spbIndex ].instOutlineColor
#endif
#ifdef DYNAMIC_OUTLINE_COLOR_TINT
	#define pInstOutlineColorTint pSharedSPB[ spbIndex ].instOutlineColorTint
#endif
#ifdef DYNAMIC_OUTLINE_THICKNESS
	#define pInstOutlineThickness pSharedSPB[ spbIndex ].instOutlineThickness
#endif
#ifdef DYNAMIC_OUTLINE_EMISSIVITY
	#define pInstOutlineEmissivity pSharedSPB[ spbIndex ].instOutlineEmissivity
#endif
#ifdef DYNAMIC_OUTLINE_EMISSIVITY_TINT
	#define pInstOutlineEmissivityTint pSharedSPB[ spbIndex ].instOutlineEmissivityTint
#endif
#ifdef DYNAMIC_OUTLINE_SOLIDITY
	#define pInstOutlineSolidity pSharedSPB[ spbIndex ].instOutlineSolidity
#endif
#ifdef DYNAMIC_SKIN_CLIP_PLANE
	#define pInstSkinClipPlane pSharedSPB[ spbIndex ].instSkinClipPlane
#endif
#ifdef DYNAMIC_SKIN_CLIP_PLANE_BORDER
	#define pInstSkinClipPlaneBorder pSharedSPB[ spbIndex ].instSkinClipPlaneBorder
#endif



// redirect flags if used
#if defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED
	#define pLayerVisibility spbFlags
#endif
