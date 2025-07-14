/*
	<!-- shared/defren/skin/ubo_instance_parameters.glsl -->
	<shaderStorageBlock name='InstanceParametersSSBO' binding='0'/>
	<uniformBlock name='InstanceParameters' binding='2'/>
	
	<define>SHARED_SPB</define>
	
	<!-- system only
	<define>SHARED_SPB_PADDING</define>
	<define>SHARED_SPB_USE_SSBO</define>
	<define>SHARED_SPB_ARRAY_SIZE</define>
	-->
*/

/*
affects:
C: component
B: billboard
P: particle
PF: prop field
H: height map
*/

#ifdef SHARED_SPB

struct sInstanceParameters{
	mat4x3 matrixModel;
	mat3 matrixNormal; // C
	mat3x2 matrixTexCoord; // C
	
	vec4 billboardPosTransform; // B: multiply{X,Y}, offset{X,Y}
	bvec3 billboardParams; // B: locked, spherical, sizeFixedToScreen
	vec4 samplesParams; // P: scaleU, offsetU, scaleV, offsetV
	
	vec2 heightTerrainMaskTCTransform; // H: multiply
	ivec2 heightTerrainMaskSelector; // H: arrayLayer, componentIndex
	
	vec2 variationSeed; // C
	float burstFactor; // P
	int ribbonSheetCount; // P: ribbon or beam
	
	float propFieldParams; // PF: bendFactor
	bool doubleSided; // C
	float envMapFade; // C
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
	
	vec3 instEnvRoomTint; // environmentroom.tint
	
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
	
	vec4 instSkinClipPlaneNormal; // clip.plane : xyz=normal, w=distance
	float instSkinClipPlane; // clip.plane
	float instSkinClipPlaneBorder; // clip.plane.border
	
	#ifdef SHARED_SPB_PADDING
	vec4 padding[ SHARED_SPB_PADDING ];
	#endif
};

// with UBO the size has to be a constant (the definition is complicated but basically
// it boils down to anything but constants working reliably). SSBOs on the other hand
// can be of any size
#ifdef SHARED_SPB_USE_SSBO
	UBOLAYOUT_BIND(0) readonly buffer InstanceParametersSSBO
#else
	UBOLAYOUT_BIND(2) uniform InstanceParameters
#endif
{
	#ifdef SHARED_SPB_ARRAY_SIZE
	sInstanceParameters pSharedSPB[ SHARED_SPB_ARRAY_SIZE ];
	#else
	sInstanceParameters pSharedSPB[];
	#endif
};



#else // SHARED_SPB

UBOLAYOUT_BIND(2) uniform InstanceParameters{
	mat4x3 pMatrixModel;
	mat3 pMatrixNormal; // C
	mat3x2 pMatrixTexCoord; // C
	
	vec4 pBillboardPosTransform; // B: multiply{X,Y}, offset{X,Y}
	bvec3 pBillboardParams; // B: locked, spherical, sizeFixedToScreen
	vec4 pSamplesParams; // P: scaleU, offsetU, scaleV, offsetV
	
	vec2 pHeightTerrainMaskTCTransform; // H: multiply
	ivec2 pHeightTerrainMaskSelector; // H: arrayLayer, componentIndex
	
	vec2 pVariationSeed; // C
	float pBurstFactor; // P
	int pRibbonSheetCount; // P: ribbon or beam
	
	float pPropFieldParams; // PF: bendFactor
	bool pDoubleSided; // C
	float pEnvMapFade; // C
	int pIndexSPBTexParams; // shared parameter block index for texture parameters
	
	/*
	vec4 pTCTransformColor;
	vec4 pTCTransformNormal;
	vec4 pTCTransformReflectivity;
	vec4 pTCTransformEmissivity;
	vec4 pTCTransformRefractionDistort;
	vec4 pTCTransformAO;
	*/
	
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
	
	vec3 pInstEnvRoomTint; // environmentroom.tint
	
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

#endif // SHARED_SPB
