#ifdef DYNAMIC_COLOR_TINT
	#define pColorTint pInstColorTint
#else
	#define pColorTint pTexColorTint
#endif

#ifdef DYNAMIC_COLOR_GAMMA
	#define pColorGamma pInstColorGamma
#else
	#define pColorGamma pTexColorGamma
#endif

#ifdef DYNAMIC_COLOR_SOLIDITY_MULTIPLIER
	#define pColorSolidityMultiplier pInstColorSolidityMultiplier
#else
	#define pColorSolidityMultiplier pTexColorSolidityMultiplier
#endif

#ifdef DYNAMIC_AO_SOLIDITY_MULTIPLIER
	#define pAOSolidityMultiplier pInstAOSolidityMultiplier
#else
	#define pAOSolidityMultiplier pTexAOSolidityMultiplier
#endif

#ifdef DYNAMIC_TRANSPARENCY_MULTIPLIER
	#define pTransparencyMultiplier pInstTransparencyMultiplier
#else
	#define pTransparencyMultiplier pTexTransparencyMultiplier
#endif

#ifdef DYNAMIC_SOLIDITY_MULTIPLIER
	#define pSolidityMultiplier pInstSolidityMultiplier
#else
	#define pSolidityMultiplier pTexSolidityMultiplier
#endif

#ifdef DYNAMIC_HEIGHT_REMAP
	#define pHeightRemap pInstHeightRemap
#else
	#define pHeightRemap pTexHeightRemap
#endif

#ifdef DYNAMIC_NORMAL_STRENGTH
	#define pNormalStrength pInstNormalStrength
#else
	#define pNormalStrength pTexNormalStrength
#endif

#ifdef DYNAMIC_NORMAL_SOLIDITY_MULTIPLIER
	#define pNormalSolidityMultiplier pInstNormalSolidityMultiplier
#else
	#define pNormalSolidityMultiplier pTexNormalSolidityMultiplier
#endif

#ifdef DYNAMIC_ROUGHNESS_REMAP
	#define pRoughnessRemap pInstRoughnessRemap
#else
	#define pRoughnessRemap pTexRoughnessRemap
#endif

#ifdef DYNAMIC_ROUGHNESS_GAMMA
	#define pRoughnessGamma pInstRoughnessGamma
#else
	#define pRoughnessGamma pTexRoughnessGamma
#endif

#ifdef DYNAMIC_ROUGHNESS_SOLIDITY_MULTIPLIER
	#define pRoughnessSolidityMultiplier pInstRoughnessSolidityMultiplier
#else
	#define pRoughnessSolidityMultiplier pTexRoughnessSolidityMultiplier
#endif

#ifdef DYNAMIC_REFLECTIVITY_SOLIDITY_MULTIPLIER
	#define pReflectivitySolidityMultiplier pInstReflectivitySolidityMultiplier
#else
	#define pReflectivitySolidityMultiplier pTexReflectivitySolidityMultiplier
#endif

#ifdef DYNAMIC_REFRACTION_DISTORT_STRENGTH
	#define pRefractionDistortStrength pInstRefractionDistortStrength
#else
	#define pRefractionDistortStrength pTexRefractionDistortStrength
#endif

#ifdef DYNAMIC_EMISSIVITY_INTENSITY
	#define pEmissivityIntensity pInstEmissivityIntensity
#else
	#define pEmissivityIntensity pTexEmissivityIntensity
#endif

#ifdef DYNAMIC_ENVROOM_SIZE
	#define pEnvRoomSize pInstEnvRoomSize
#else
	#define pEnvRoomSize pTexEnvRoomSize
#endif

#ifdef DYNAMIC_ENVROOM_OFFSET
	#define pEnvRoomOffset pInstEnvRoomOffset
#else
	#define pEnvRoomOffset pTexEnvRoomOffset
#endif

#ifdef DYNAMIC_ENVROOM_EMISSIVITY_INTENSITY
	#define pEnvRoomEmissivityIntensity pInstEnvRoomEmissivityIntensity
#else
	#define pEnvRoomEmissivityIntensity pTexEnvRoomEmissivityIntensity
#endif

#ifdef DYNAMIC_VARIATION
	#define pVariationEnableScale pInstVariationEnableScale
#else
	#define pVariationEnableScale pTexVariationEnableScale
#endif

#ifdef DYNAMIC_REFLECTIVITY_MULTIPLIER
	#define pReflectivityMultiplier pInstReflectivityMultiplier
#else
	#define pReflectivityMultiplier pTexReflectivityMultiplier
#endif

#define pParticleSheetCount pTexParticleSheetCount
