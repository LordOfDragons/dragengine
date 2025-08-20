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

vec3 getColorTint(int spbIndex){
	return DynamicColorTint ? pInstColorTint : pTexColorTint;
}

float getColorGamma(int spbIndex){
	return DynamicColorGamma ? pInstColorGamma : pTexColorGamma;
}

float getColorSolidityMultiplier(int spbIndex){
	return DynamicColorSolidityMultiplier ? pInstColorSolidityMultiplier : pTexColorSolidityMultiplier;
}

float getAOSolidityMultiplier(int spbIndex){
	return DynamicAOSolidityMultiplier ? pInstAOSolidityMultiplier : pTexAOSolidityMultiplier;
}

float getTransparencyMultiplier(int spbIndex){
	return DynamicTransparencyMultiplier ? pInstTransparencyMultiplier : pTexTransparencyMultiplier;
}

float getSolidityMultiplier(int spbIndex){
	return DynamicSolidityMultiplier ? pInstSolidityMultiplier : pTexSolidityMultiplier;
}

vec2 getHeightRemap(int spbIndex){
	return DynamicHeightRemap ? pInstHeightRemap : pTexHeightRemap;
}

float getNormalStrength(int spbIndex){
	return DynamicNormalStrength ? pInstNormalStrength : pTexNormalStrength;
}

float getNormalStrengthMultiplier(int spbIndex){
	return DynamicNormalStrengthMultiplier ? pInstNormalSolidityMultiplier : pTexNormalSolidityMultiplier;
}

vec2 getRoughnessRemap(int spbIndex){
	return DynamicRoughnessRemap ? pInstRoughnessRemap : pTexRoughnessRemap;
}

float getRoughnessGamma(int spbIndex){
	return DynamicRoughnessGamma ? pInstRoughnessGamma : pTexRoughnessGamma;
}

float getRoughnessSolidityMultiplier(int spbIndex){
	return DynamicRoughnessSolidityMultiplier ? pInstRoughnessSolidityMultiplier : pTexRoughnessSolidityMultiplier;
}

float getReflectivitySolidityMultiplier(int spbIndex){
	return DynamicReflectivitySolidityMultiplier ? pInstReflectivitySolidityMultiplier : pTexReflectivitySolidityMultiplier;
}

float getDynamicReflectivityMultiplier(int spbIndex){
	return DynamicReflectivityMultiplier ? pInstReflectivityMultiplier : pTexReflectivityMultiplier;
}

float getRefractionDistortStrength(int spbIndex){
	return DynamicRefractionDistortStrength ? pInstRefractionDistortStrength : pTexRefractionDistortStrength;
}

vec3 getEmissivityIntensity(int spbIndex){
	return DynamicEmissivityIntensity ? pInstEmissivityIntensity : pTexEmissivityIntensity;
}

vec3 getEnvRoomTint(int spbIndex){
	return DynamicEnvRoomTint ? pInstEnvRoomTint : pTexEnvRoomTint;
}

vec2 getEnvRoomSize(int spbIndex){
	return DynamicEnvRoomSize ? pInstEnvRoomSize : pTexEnvRoomSize;
}

vec3 getEnvRoomOffset(int spbIndex){
	return DynamicEnvRoomOffset ? pInstEnvRoomOffset : pTexEnvRoomOffset;
}

vec3 getEnvRoomEmissivityIntensity(int spbIndex){
	return DynamicEnvRoomEmissivityIntensity ? pInstEnvRoomEmissivityIntensity : pTexEnvRoomEmissivityIntensity;
}

vec2 getVariationEnableScale(int spbIndex){
	return DynamicVariation ? pInstVariationEnableScale : pTexVariationEnableScale;
}

vec3 getRimEmissivityIntensity(int spbIndex){
	return DynamicRimEmissivityIntensity ? pInstRimEmissivityIntensity : pTexRimEmissivityIntensity;
}

float getRimAngle(int spbIndex){
	return DynamicRimAngle ? pInstRimAngle : pTexRimAngle;
}

float getRimExponent(int spbIndex){
	return DynamicRimExponent ? pInstRimExponent : pTexRimExponent;
}

vec3 getOutlineColor(int spbIndex){
	return DynamicOutlineColor ? pInstOutlineColor : pTexOutlineColor;
}

vec3 getOutlineColorTint(int spbIndex){
	return DynamicOutlineColorTint ? pInstOutlineColorTint : pTexOutlineColorTint;
}

float getOutlineThickness(int spbIndex){
	return DynamicOutlineThickness ? pInstOutlineThickness : pTexOutlineThickness;
}

float getOutlineSolidity(int spbIndex){
	return DynamicOutlineSolidity ? pInstOutlineSolidity : pTexOutlineSolidity;
}

vec3 getOutlineEmissivity(int spbIndex){
	return DynamicOutlineEmissivity ? pInstOutlineEmissivity : pTexOutlineEmissivity;
}

vec3 getOutlineEmissivityTint(int spbIndex){
	return DynamicOutlineEmissivityTint ? pInstOutlineEmissivityTint : pTexOutlineEmissivityTint;
}

float getSkinClipPlane(int spbIndex){
	return DynamicSkinClipPlane ? pInstSkinClipPlane : pTexSkinClipPlane;
}

float getSkinClipPlaneBorder(int spbIndex){
	return DynamicSkinClipPlaneBorder ? pInstSkinClipPlaneBorder : pTexSkinClipPlaneBorder;
}

int getParticleSheetCount(int spbIndex){
	return pTexParticleSheetCount;
}

bool getEmissivityCameraAdapted(int spbIndex){
	return pTexEmissivityCameraAdapted;
}

