#ifdef PARTICLE
	#define pMatrixModel pSharedSPB[ spbIndex ].pSPBMatrixModel
	#define pSamplesParams pSharedSPB[ spbIndex ].pSPBSamplesParams
	#define pBurstFactor pSharedSPB[ spbIndex ].pSPBBurstFactor
	#if defined PARTICLE_RIBBON || defined PARTICLE_BEAM
		#define pRibbonSheetCount pSharedSPB[ spbIndex ].pSPBRibbonSheetCount
	#endif
	
#else  // PARTICLE
	// component properties. the same for all textures in a component
	#define pMatrixModel pSharedSPB[ spbIndex ].pSPBMatrixModel
	#define pMatrixNormal pSharedSPB[ spbIndex ].pSPBMatrixNormal
	
	// per texture properties. potentially different for each texture in a component
	#define pMatrixTexCoord pSharedSPB[ spbIndex ].pSPBMatrixTexCoord
	#ifdef PROP_FIELD
		#define pPropFieldParams pSharedSPB[ spbIndex ].pSPBPropFieldParams
	#endif
	#ifdef HEIGHT_MAP
		#define pHeightTerrainMaskTCTransform pSharedSPB[ spbIndex ].pSPBHeightTerrainMaskTCTransform
		#define pHeightTerrainMaskSelector pSharedSPB[ spbIndex ].pSPBHeightTerrainMaskSelector
	#endif
	#define pDoubleSided pSharedSPB[ spbIndex ].pSPBDoubleSided
	#define pEnvMapFade pSharedSPB[ spbIndex ].pSPBEnvMapFade
	#ifdef WITH_VARIATIONS
		#define pVariationSeed pSharedSPB[ spbIndex ].pSPBVariationSeed
	#endif
	
	// imposter
	#define pBillboardPosTransform pSharedSPB[ spbIndex ].pSPBBillboardPosTransform
	#define pBillboardParams pSharedSPB[ spbIndex ].pSPBBillboardParams
#endif  // PARTICLE

// texture coordinate transformations used for dynamic textures only
/*
#define pTCTransformColor pSharedSPB[ spbIndex ].pSPBTCTransformColor
#ifdef TEXTURE_NORMAL
	#define pTCTransformNormal pSharedSPB[ spbIndex ].pSPBTCTransformNormal
#endif
#ifdef TEXTURE_REFLECTIVITY
	#define pTCTransformReflectivity pSharedSPB[ spbIndex ].pSPBTCTransformReflectivity
#endif
#ifdef TEXTURE_EMISSIVITY
	#define pTCTransformEmissivity pSharedSPB[ spbIndex ].pSPBTCTransformEmissivity
#endif
#ifdef TEXTURE_REFRACTION_DISTORT
	#define pTCTransformRefractionDistort pSharedSPB[ spbIndex ].pSPBTCTransformRefractionDistort
#endif
#ifdef TEXTURE_AO
	#define pTCTransformAO pSharedSPB[ spbIndex ].pSPBTCTransformAO
#endif
*/

// per texture dynamic texture properties not using textures
#ifdef DYNAMIC_COLOR_TINT
	#define pInstColorTint pSharedSPB[ spbIndex ].pSPBInstColorTint
#endif
#ifdef DYNAMIC_COLOR_GAMMA
	#define pInstColorGamma pSharedSPB[ spbIndex ].pSPBInstColorGamma
#endif
#ifdef DYNAMIC_COLOR_SOLIDITY_MULTIPLIER
	#define pInstColorSolidityMultiplier pSharedSPB[ spbIndex ].pSPBInstColorSolidityMultiplier
#endif
#ifdef DYNAMIC_TRANSPARENCY_MULTIPLIER
	#define pInstTransparencyMultiplier pSharedSPB[ spbIndex ].pSPBInstTransparencyMultiplier
#endif
#ifdef DYNAMIC_SOLIDITY_MULTIPLIER
	#define pInstSolidityMultiplier pSharedSPB[ spbIndex ].pSPBInstSolidityMultiplier
#endif
#ifdef DYNAMIC_AO_SOLIDITY_MULTIPLIER
	#define pInstAOSolidityMultiplier pSharedSPB[ spbIndex ].pSPBInstAOSolidityMultiplier
#endif
#ifdef DYNAMIC_HEIGHT_REMAP
	#define pInstHeightRemap pSharedSPB[ spbIndex ].pSPBInstHeightRemap
#endif
#ifdef DYNAMIC_NORMAL_STRENGTH
	#define pInstNormalStrength pSharedSPB[ spbIndex ].pSPBInstNormalStrength
#endif
#ifdef DYNAMIC_NORMAL_SOLIDITY_MULTIPLIER
	#define pInstNormalSolidityMultiplier pSharedSPB[ spbIndex ].pSPBInstNormalSolidityMultiplier
#endif
#ifdef DYNAMIC_ROUGHNESS_REMAP
	#define pInstRoughnessRemap pSharedSPB[ spbIndex ].pSPBInstRoughnessRemap
#endif
#ifdef DYNAMIC_ROUGHNESS_GAMMA
	#define pInstRoughnessGamma pSharedSPB[ spbIndex ].pSPBInstRoughnessGamma
#endif
#ifdef DYNAMIC_ROUGHNESS_SOLIDITY_MULTIPLIER
	#define pInstRoughnessSolidityMultiplier pSharedSPB[ spbIndex ].pSPBInstRoughnessSolidityMultiplier
#endif
#ifdef DYNAMIC_REFRACTION_DISTORT_STRENGTH
	#define pInstRefractionDistortStrength pSharedSPB[ spbIndex ].pSPBInstRefractionDistortStrength
#endif
#ifdef DYNAMIC_REFLECTIVITY_SOLIDITY_MULTIPLIER
	#define pInstReflectivitySolidityMultiplier pSharedSPB[ spbIndex ].pSPBInstReflectivitySolidityMultiplier
#endif
#ifdef DYNAMIC_EMISSIVITY_INTENSITY
	#define pInstEmissivityIntensity pSharedSPB[ spbIndex ].pSPBInstEmissivityIntensity
#endif
#ifdef DYNAMIC_ENVROOM_SIZE
	#define pInstEnvRoomSize pSharedSPB[ spbIndex ].pSPBInstEnvRoomSize
#endif
#ifdef DYNAMIC_ENVROOM_OFFSET
	#define pInstEnvRoomOffset pSharedSPB[ spbIndex ].pSPBInstEnvRoomOffset
#endif
#ifdef DYNAMIC_ENVROOM_EMISSIVITY_INTENSITY
	#define pInstEnvRoomEmissivityIntensity pSharedSPB[ spbIndex ].pSPBInstEnvRoomEmissivityIntensity
#endif
#ifdef DYNAMIC_VARIATION
	#define pInstVariationEnableScale pSharedSPB[ spbIndex ].pSPBInstVariationEnableScale
#endif
#ifdef DYNAMIC_REFLECTIVITY_MULTIPLIER
	#define pInstReflectivityMultiplier pSharedSPB[ spbIndex ].pSPBInstReflectivityMultiplier
#endif



// redirect flags if used
#ifdef GS_RENDER_CUBE
	#define pCubeFaceVisible spbFlags
#endif
