/*
	<!-- shared/defren/skin/ubo_dynamic_parameters.glsl -->
	<define>DYNAMIC_AO_SOLIDITY_MULTIPLIER</define>
	<define>DYNAMIC_COLOR_GAMMA</define>
	<define>DYNAMIC_COLOR_SOLIDITY_MULTIPLIER</define>
	<define>DYNAMIC_COLOR_TINT</define>
	<define>DYNAMIC_EMISSIVITY_INTENSITY</define>
	<define>DYNAMIC_ENVROOM_EMISSIVITY_INTENSITY</define>
	<define>DYNAMIC_ENVROOM_OFFSET</define>
	<define>DYNAMIC_ENVROOM_SIZE</define>
	<define>DYNAMIC_ENVROOM_TINT</define>
	<define>DYNAMIC_HEIGHT_REMAP</define>
	<define>DYNAMIC_NORMAL_SOLIDITY_MULTIPLIER</define>
	<define>DYNAMIC_NORMAL_STRENGTH</define>
	<define>DYNAMIC_OUTLINE_COLOR_TINT</define>
	<define>DYNAMIC_OUTLINE_COLOR</define>
	<define>DYNAMIC_OUTLINE_EMISSIVITY_TINT</define>
	<define>DYNAMIC_OUTLINE_EMISSIVITY</define>
	<define>DYNAMIC_OUTLINE_SOLIDITY</define>
	<define>DYNAMIC_OUTLINE_THICKNESS</define>
	<define>DYNAMIC_REFLECTIVITY_MULTIPLIER</define>
	<define>DYNAMIC_REFLECTIVITY_SOLIDITY_MULTIPLIER</define>
	<define>DYNAMIC_REFRACTION_DISTORT_STRENGTH</define>
	<define>DYNAMIC_RIM_ANGLE</define>
	<define>DYNAMIC_RIM_EMISSIVITY_INTENSITY</define>
	<define>DYNAMIC_RIM_EXPONENT</define>
	<define>DYNAMIC_ROUGHNESS_GAMMA</define>
	<define>DYNAMIC_ROUGHNESS_REMAP</define>
	<define>DYNAMIC_ROUGHNESS_SOLIDITY_MULTIPLIER</define>
	<define>DYNAMIC_SKIN_CLIP_PLANE_BORDER</define>
	<define>DYNAMIC_SKIN_CLIP_PLANE</define>
	<define>DYNAMIC_SOLIDITY_MULTIPLIER</define>
	<define>DYNAMIC_TRANSPARENCY_MULTIPLIER</define>
	<define>DYNAMIC_VARIATION</define>
*/

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

#ifdef DYNAMIC_ENVROOM_TINT
	#define pEnvRoomTint pInstEnvRoomTint
#else
	#define pEnvRoomTint pTexEnvRoomTint
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

#ifdef DYNAMIC_RIM_EMISSIVITY_INTENSITY
	#define pRimEmissivityIntensity pInstRimEmissivityIntensity
#else
	#define pRimEmissivityIntensity pTexRimEmissivityIntensity
#endif

#ifdef DYNAMIC_RIM_ANGLE
	#define pRimAngle pInstRimAngle
#else
	#define pRimAngle pTexRimAngle
#endif

#ifdef DYNAMIC_RIM_EXPONENT
	#define pRimExponent pInstRimExponent
#else
	#define pRimExponent pTexRimExponent
#endif

#ifdef DYNAMIC_OUTLINE_COLOR
	#define pOutlineColor pInstOutlineColor
#else
	#define pOutlineColor pTexOutlineColor
#endif

#ifdef DYNAMIC_OUTLINE_COLOR_TINT
	#define pOutlineColorTint pInstOutlineColorTint
#else
	#define pOutlineColorTint pTexOutlineColorTint
#endif

#ifdef DYNAMIC_OUTLINE_THICKNESS
	#define pOutlineThickness pInstOutlineThickness
#else
	#define pOutlineThickness pTexOutlineThickness
#endif

#ifdef DYNAMIC_OUTLINE_SOLIDITY
	#define pOutlineSolidity pInstOutlineSolidity
#else
	#define pOutlineSolidity pTexOutlineSolidity
#endif

#ifdef DYNAMIC_OUTLINE_EMISSIVITY
	#define pOutlineEmissivity pInstOutlineEmissivity
#else
	#define pOutlineEmissivity pTexOutlineEmissivity
#endif

#ifdef DYNAMIC_OUTLINE_EMISSIVITY_TINT
	#define pOutlineEmissivityTint pInstOutlineEmissivityTint
#else
	#define pOutlineEmissivityTint pTexOutlineEmissivityTint
#endif

#define pEmissivityCameraAdapted pTexEmissivityCameraAdapted

#ifdef DYNAMIC_SKIN_CLIP_PLANE
	#define pSkinClipPlane pInstSkinClipPlane
#else
	#define pSkinClipPlane pTexSkinClipPlane
#endif

#ifdef DYNAMIC_SKIN_CLIP_PLANE_BORDER
	#define pSkinClipPlaneBorder pInstSkinClipPlaneBorder
#else
	#define pSkinClipPlaneBorder pTexSkinClipPlaneBorder
#endif
