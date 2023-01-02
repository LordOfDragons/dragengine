/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglSkinShader.h"
#include "deoglSkinShaderManager.h"
#include "../deoglSkinTexture.h"
#include "../channel/deoglSkinChannel.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../envmap/deoglEnvironmentMap.h"
#include "../../extensions/deoglExtensions.h"
#include "../../rendering/deoglRenderReflection.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTBufferObject.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../renderthread/deoglRTDefaultTextures.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../shaders/deoglShaderCompiled.h"
#include "../../shaders/deoglShaderLanguage.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderSources.h"
#include "../../shaders/paramblock/deoglSPBParameter.h"
#include "../../shaders/paramblock/deoglSPBlockMemory.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../../shaders/paramblock/shared/deoglSharedSPBElement.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../texture/texunitsconfig/deoglTexUnitConfig.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

static const char *vTextureTargetNames[ deoglSkinShader::ETT_COUNT ] = {
	"texColor", // ettColor
	"texColorTintMask", // ettColorTintMask
	"texTransparency", // ettTransparency
	"texSolidity", // ettSolidity
	"texNormal", // ettNormal
	"texHeight", // ettHeight
	"texReflectivity", // ettReflectivity
	"texRoughness", // ettRoughness
	"texEnvMap", // ettEnvMap
	"texEnvMapFade", // ettEnvMapFade
	"texEmissivity", // ettEmissivity
	"texRenderColor", // ettRenderColor
	"texRefractionDistort", // ettRefractionDistort
	"texAO", // ettAO
	"texEnvRoom", // ettEnvRoom
	"texEnvRoomMask", // ettEnvRoomMask
	"texEnvRoomEmissivity", // ettEnvRoomEmissivity
	"texAbsorption", // ettAbsorption
	"texRimEmissivity", // ettRimEmissivity
	"texNonPbrAlbedo", // ettNonPbrAlbedo
	"texNonPbrMetalness", // ettNonPbrMetalness
	"texDepth", // ettDepth
	"texDepthTest", // ettDepthTest
	"texSamples", // ettSamples
	"texSubInstance1", // ettSubInstance1
	"texSubInstance2", // ettSubInstance2
	"texHeightMapMask", // ettHeightMapMask
	"texXRayDepth" // ettXRayDepth
};

static const char *vTextureUniformTargetNames[ deoglSkinShader::ETUT_COUNT ] = {
	"pValueColorTransparency", // etutValueColorTransparency
	"pValueNormal", // etutValueNormal
	"pValueReflectivityRoughness", // etutValueReflectivityRoughness
	"pValueRefractionDistort", // etutValueRefractionDistort
	"pValueSolidity", // etutValueSolidity
	"pValueAO", // etutValueAO
	
	"pTexColorTint", // etutTexColorTint
	"pTexColorGamma", // etutTexColorGamma
	
	"pTexColorSolidityMultiplier", // etutTexColorSolidityMultiplier
	"pTexAOSolidityMultiplier", // etutTexAOSolidityMultiplier
	"pTexSolidityMultiplier", // etutTexSolidityMultiplier
	"pTexAbsorptionRange", // etutTexAbsorptionRange
	
	"pTexHeightRemap", // etutTexHeightRemap
	"pTexNormalStrength", // etutTexNormalStrength
	"pTexNormalSolidityMultiplier", // etutTexNormalSolidityMultiplier
	
	"pTexRoughnessRemap", // etutTexRoughnessRemap
	"pTexRoughnessGamma", // etutTexRoughnessGamma
	"pTexRoughnessSolidityMultiplier", // etutTexRoughnessSolidityMultiplier
	
	"pTexEnvRoomSize", // etutTexEnvRoomSize
	"pTexRefractionDistortStrength", // etutTexRefractionDistortStrength
	"pTexReflectivitySolidityMultiplier", // etutTexReflectivitySolidityMultiplier
		
	"pTexEnvRoomOffset", // etutTexEnvRoomOffset
	"pTexTransparencyMultiplier", // etutTexTransparencyMultiplier

	"pTexEnvRoomEmissivityIntensity", // etutTexEnvRoomEmissivityIntensity
	"pTexThickness", // etutTexThichness
	
	"pTexEmissivityIntensity", // etutTexEmissivityIntensity
	"pTexReflectivityMultiplier", // etutReflectivityMultiplier
	
	"pTexVariationEnableScale", // etutTexVariationEnableScale
	"pTexParticleSheetCount", // etutTexParticleSheetCount
	"pTexRimAngle", // etutTexRimAngle
	
	"pTexRimEmissivityIntensity", // etutTexRimEmissivityIntensity
	"pTexRimExponent", // etutTexRimExponent
	
	"pTexOutlineColor", // etutTexOutlineColor
	"pTexOutlineThickness", // etutTexOutlineThickness
	"pTexOutlineColorTint", // etutTexOutlineColorTint
	"pTexOutlineSolidity", // etutTexOutlineSolidity
	"pTexOutlineEmissivity", // etutTexOutlineEmissivity
	"pTexOutlineEmissivityTint", // etutTexOutlineEmissivityTint
	
	"pTexEmissivityCameraAdapted", // etutTexEmissivityCameraAdapted
};

static const char *vInstanceUniformTargetNames[ deoglSkinShader::EIUT_COUNT ] = {
	"pMatrixModel", // eiutMatrixModel
	"pMatrixNormal", // eiutMatrixNormal
	
	"pMatrixTexCoord", // eiutMatrixTexCoord
	"pPropFieldParams", // eiutPropFieldParams
	"pHeightTerrainMaskTCTransform", // eiutHeightTerrainMaskTCTransform
	"pHeightTerrainMaskSelector", // eiutHeightTerrainMaskSelector
	"pDoubleSided", // eiutMatrixTexCoord
	"pEnvMapFade", // eiutEnvMapFade
	"pVariationSeed", // eiutVariationSeed
	"pBillboardPosTransform", // eiutBillboardPosTransform
	"pBillboardParams", // eiutBillboardParams
	
	"pSamplesParams", // eiutSamplesParams
	"pBurstFactor", // eiutBurstFactor
	"pRibbonSheetCount", // eiutRibbonSheetCount
	
	"pIndexSPBTexParams", // eiutIndexSPBTexParams
	
	"pTCTransformColor", // eiutTCTransformColor
	"pTCTransformNormal", // eiutTCTransformNormal
	"pTCTransformReflectivity", // eiutTCTransformReflectivity
	"pTCTransformEmissivity", // eiutTCTransformEmissivity
	"pTCTransformRefractionDistort", // eiutTCTransformRefractionDistort
	"pTCTransformAO", // eiutTCTransformAO
	
	"pInstColorTint", // eiutInstColorTint
	"pInstColorGamma", // eiutInstColorGamma
	"pInstColorSolidityMultiplier", // eiutInstColorSolidityMultiplier
	"pInstTransparencyMultiplier", // eiutInstTransparencyMultiplier
	"pInstSolidityMultiplier", // eiutInstSolidityMultiplier
	"pInstAOSolidityMultiplier", // eiutInstAOSolidityMultiplier
	"pInstHeightRemap", // eiutInstHeightRemap
	"pInstNormalStrength", // eiutInstNormalStrength
	"pInstNormalSolidityMultiplier", // eiutInstNormalSolidityMultiplier
	"pInstRoughnessRemap", // eiutInstRoughnessRemap
	"pInstRoughnessGamma", // eiutInstRoughnessGamma
	"pInstRoughnessSolidityMultiplier", // eiutInstRoughnessSolidityMultiplier
	"pInstRefractionDistortStrength", // pInstRefractionDistortStrength
	"pInstReflectivitySolidityMultiplier", // eiutInstReflectivitySolidityMultiplier
	"pInstEmissivityIntensity", // eiutInstEmissivityIntensity
	"pInstEnvRoomSize", // eiutInstEnvRoomSize
	"pInstEnvRoomOffset", // eiutInstEnvRoomOffset
	"pInstEnvRoomEmissivityIntensity", // eiutInstEnvRoomEmissivityIntensity
	"pInstVariationEnableScale", // eiutInstVariationEnableScale
	"pInstReflectivityMultiplier", // eiutReflectivityMultiplier
	"pInstRimEmissivityIntensity", // eiutInstRimEmissivityIntensity
	"pInstRimAngle", // eiutInstRimAngle
	"pInstRimExponent", // eiutInstRimExponent
	"pInstOutlineColor", // eiutInstOutlineColor
	"pInstOutlineThickness", // eiutInstOutlineThickness
	"pInstOutlineColorTint", // eiutInstOutlineColorTint
	"pInstOutlineEmissivity", // eiutInstOutlineEmissivity
	"pInstOutlineSolidity", // eiutInstOutlineSolidity
	"pInstOutlineEmissivityTint", // eiutInstOutlineEmissivityTint
};



struct sSPBParameterDefinition{
	deoglSPBParameter::eValueTypes dataType;
	int componentCount;
	int vectorCount;
	int arrayCount;
};

static const sSPBParameterDefinition vTextureSPBParamDefs[ deoglSkinShader::ETUT_COUNT ] = {
	{ deoglSPBParameter::evtFloat, 4, 1, 1 }, // etutValueColorTransparency ( vec4 )
	{ deoglSPBParameter::evtFloat, 4, 1, 1 }, // etutValueNormal ( vec4 )
	{ deoglSPBParameter::evtFloat, 4, 1, 1 }, // etutValueReflectivityRoughness ( vec4 )
	{ deoglSPBParameter::evtFloat, 2, 1, 1 }, // etutValueRefractionDistort ( vec2 )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // etutValueSolidity ( float )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // etutValueAO ( float )
	
	{ deoglSPBParameter::evtFloat, 3, 1, 1 }, // etutTexColorTint ( vec3 )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // etutTexColorGamma ( float )
	
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // etutTexColorSolidityMultiplier ( float )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // etutTexAOSolidityMultiplier ( float )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // etutTexSolidityMultiplier ( float )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // etutTexAbsorptionRange ( float )
	
	{ deoglSPBParameter::evtFloat, 2, 1, 1 }, // etutTexHeightRemap ( vec2 )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // etutTexNormalStrength ( float )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // etutTexNormalSolidityMultiplier ( float )
	
	{ deoglSPBParameter::evtFloat, 2, 1, 1 }, // etutTexRoughnessRemap ( vec2 )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // etutTexRoughnessGamma ( float )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // etutTexRoughnessSolidityMultiplier ( float )
	
	{ deoglSPBParameter::evtFloat, 2, 1, 1 }, // etutTexEnvRoomSize ( vec2 )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // etutTexRefractionDistortStrength ( float )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // etutTexReflectivitySolidityMultiplier ( float )
	
	{ deoglSPBParameter::evtFloat, 3, 1, 1 }, // etutTexEnvRoomOffset ( vec3 )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // etutTexTransparencyMultiplier ( float )
	
	{ deoglSPBParameter::evtFloat, 3, 1, 1 }, // etutTexEnvRoomEmissivityIntensity ( vec3 )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // etutTexThickness ( float )
	
	{ deoglSPBParameter::evtFloat, 3, 1, 1 }, // etutTexEmissivityIntensity ( vec3 )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // etutReflectivityMultiplier ( float )
	
	{ deoglSPBParameter::evtFloat, 2, 1, 1 }, // etutTexVariationEnableScale ( vec2 )
	{ deoglSPBParameter::evtInt, 1, 1, 1 }, // etutTexParticleSheetCount ( int )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // etutTexRimAngle ( float )
	
	{ deoglSPBParameter::evtFloat, 3, 1, 1 }, // etutTexRimEmissivityIntensity ( vec3 )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // etutTexRimExponent ( float )
	
	{ deoglSPBParameter::evtFloat, 3, 1, 1 }, // etutTexOutlineColor ( vec3 )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // etutTexOutlineThickness ( float )
	
	{ deoglSPBParameter::evtFloat, 3, 1, 1 }, // etutTexOutlineColorTint ( vec3 )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // etutTexOutlineSolidity ( float )
	
	{ deoglSPBParameter::evtFloat, 3, 1, 1 }, // etutTexOutlineEmissivity ( vec3 )
	{ deoglSPBParameter::evtFloat, 3, 1, 1 }, // etutTexOutlineEmissivityTint ( vec3 )
	
	{ deoglSPBParameter::evtBool, 1, 1, 1 }  // etutTexEmissivityCameraAdapted ( bool )
};

static const sSPBParameterDefinition vInstanceSPBParamDefs[ deoglSkinShader::EIUT_COUNT ] = {
	{ deoglSPBParameter::evtFloat, 4, 3, 1 }, // eiutMatrixModel ( mat4x3 )
	{ deoglSPBParameter::evtFloat, 3, 3, 1 }, // eiutMatrixNormal ( mat3 )
	
	{ deoglSPBParameter::evtFloat, 3, 2, 1 }, // eiutMatrixTexCoord ( mat3x2 )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // eiutPropFieldParams ( float )
	{ deoglSPBParameter::evtFloat, 2, 1, 1 }, // eiutHeightTerrainMaskTCTransform ( vec2 )
	{ deoglSPBParameter::evtInt, 2, 1, 1 }, // eiutHeightTerrainMaskSelector ( ivec2 )
	{ deoglSPBParameter::evtBool, 1, 1, 1 }, // eiutDoubleSided ( bool )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // eiutEnvMapFade ( float )
	{ deoglSPBParameter::evtFloat, 2, 1, 1 }, // eiutVariationSeed ( vec2 )
	{ deoglSPBParameter::evtFloat, 4, 1, 1 }, // eiutBillboardPosTransform ( vec4 )
	{ deoglSPBParameter::evtBool, 3, 1, 1 }, // eiutBillboardParams ( bvec3 )
	
	{ deoglSPBParameter::evtFloat, 4, 1, 1 }, // eiutSamplesParams ( vec4 )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // eiutBurstFactor ( float )
	{ deoglSPBParameter::evtInt, 1, 1, 1 }, // eiutRibbonSheetCount ( int )
	
	{ deoglSPBParameter::evtInt, 1, 1, 1 }, // eiutIndexSPBTexParams ( int )
	
	{ deoglSPBParameter::evtFloat, 4, 1, 1 }, // eiutTCTransformColor ( vec4 )
	{ deoglSPBParameter::evtFloat, 4, 1, 1 }, // eiutTCTransformNormal ( vec4 )
	{ deoglSPBParameter::evtFloat, 4, 1, 1 }, // eiutTCTransformReflectivity ( vec4 )
	{ deoglSPBParameter::evtFloat, 4, 1, 1 }, // eiutTCTransformEmissivity ( vec4 )
	{ deoglSPBParameter::evtFloat, 4, 1, 1 }, // eiutTCTransformRefractionDistort ( vec4 )
	{ deoglSPBParameter::evtFloat, 4, 1, 1 }, // eiutTCTransformAO ( vec4 )
	
	{ deoglSPBParameter::evtFloat, 3, 1, 1 }, // eiutInstColorTint ( vec3 )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // eiutInstColorGamma ( float )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // eiutInstColorSolidityMultiplier ( float )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // eiutInstTransparencyMultiplier ( float )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // eiutInstSolidityMultiplier ( float )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // eiutInstAOSolidityMultiplier ( float )
	{ deoglSPBParameter::evtFloat, 2, 1, 1 }, // eiutInstHeightRemap ( vec2 )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // eiutInstNormalStrength ( float )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // eiutInstNormalSolidityMultiplier ( float )
	{ deoglSPBParameter::evtFloat, 2, 1, 1 }, // eiutInstRoughnessRemap ( vec2 )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // eiutInstRoughnessGamma ( float )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // eiutInstRoughnessSolidityMultiplier ( float )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // eiutInstRefractionDistortStrength ( float )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // eiutInstReflectivitySolidityMultiplier ( float )
	{ deoglSPBParameter::evtFloat, 3, 1, 1 }, // eiutInstEmissivityIntensity ( float )
	{ deoglSPBParameter::evtFloat, 2, 1, 1 }, // eiutInstEnvRoomSize ( vec2 )
	{ deoglSPBParameter::evtFloat, 3, 1, 1 }, // eiutInstEnvRoomOffset ( vec3 )
	{ deoglSPBParameter::evtFloat, 3, 1, 1 }, // eiutInstEnvRoomEmissivityIntensity ( vec3 )
	{ deoglSPBParameter::evtFloat, 2, 1, 1 }, // eiutInstVariationEnableScale ( vec2 )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // eiutInstReflectivityMultiplier ( float )
	{ deoglSPBParameter::evtFloat, 3, 1, 1 }, // eiutInstRimEmissivityIntensity ( vec3 )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // eiutInstRimAngle ( float )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // eiutInstRimExponent ( float )
	{ deoglSPBParameter::evtFloat, 3, 1, 1 }, // eiutInstOutlineColor ( vec3 )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // eiutInstOutlineThickness ( float )
	{ deoglSPBParameter::evtFloat, 3, 1, 1 }, // eiutInstOutlineColorTint ( vec3 )
	{ deoglSPBParameter::evtFloat, 3, 1, 1 }, // eiutInstOutlineEmissivity ( vec3 )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // eiutInstOutlineSolidity ( float )
	{ deoglSPBParameter::evtFloat, 3, 1, 1 }  // eiutInstOutlineEmissivityTint ( vec3 )
};

static const int vUBOInstParamMapCount = 44;
static const deoglSkinShader::eInstanceUniformTargets vUBOInstParamMap[ vUBOInstParamMapCount ] = {
	deoglSkinShader::eiutMatrixModel, // eiutMatrixModel ( mat4x3 )
	deoglSkinShader::eiutMatrixNormal, // eiutMatrixNormal ( mat3 )
	deoglSkinShader::eiutMatrixTexCoord, // eiutMatrixTexCoord ( mat3x2 )
	
	deoglSkinShader::eiutBillboardPosTransform, // eiutBillboardPosTransform ( vec4 )
	deoglSkinShader::eiutBillboardParams, // eiutBillboardParams( bvec3 )
	deoglSkinShader::eiutSamplesParams, // eiutSamplesParams ( vec4 )
	
	deoglSkinShader::eiutHeightTerrainMaskTCTransform, // eiutHeightTerrainMaskTCTransform ( vec2 )
	deoglSkinShader::eiutHeightTerrainMaskSelector, // eiutHeightTerrainMaskSelector ( ivec2 )
	
	deoglSkinShader::eiutVariationSeed, // eiutVariationSeed ( vec2 )
	deoglSkinShader::eiutBurstFactor, // eiutBurstFactor ( float )
	deoglSkinShader::eiutRibbonSheetCount, // eiutRibbonSheetCount ( int )
	
	deoglSkinShader::eiutPropFieldParams, // eiutPropFieldParams ( float )
	deoglSkinShader::eiutDoubleSided, // eiutDoubleSided ( bool )
	deoglSkinShader::eiutEnvMapFade, // eiutEnvMapFade ( float )
	deoglSkinShader::eiutIndexSPBTexParams, // eiutIndexSPBTexParams ( int )
	
	/*
	deoglSkinShader::eiutTCTransformColor, // eiutTCTransformColor ( vec4 )
	deoglSkinShader::eiutTCTransformNormal, // eiutTCTransformNormal ( vec4 )
	deoglSkinShader::eiutTCTransformReflectivity, // eiutTCTransformReflectivity ( vec4 )
	deoglSkinShader::eiutTCTransformEmissivity, // eiutTCTransformEmissivity ( vec4 )
	deoglSkinShader::eiutTCTransformRefractionDistort, // eiutTCTransformRefractionDistort ( vec4 )
	deoglSkinShader::eiutTCTransformAO, // eiutTCTransformAO ( vec4 )
	*/
	
	deoglSkinShader::eiutInstColorTint, // eiutInstColorTint ( vec3 )
	deoglSkinShader::eiutInstColorGamma, // eiutInstColorGamma ( float )
	
	deoglSkinShader::eiutInstColorSolidityMultiplier, // eiutInstColorSolidityMultiplier ( float )
	deoglSkinShader::eiutInstTransparencyMultiplier, // eiutInstTransparencyMultiplier ( float )
	deoglSkinShader::eiutInstSolidityMultiplier, // eiutInstSolidityMultiplier ( float )
	deoglSkinShader::eiutInstAOSolidityMultiplier, // eiutInstAOSolidityMultiplier ( float )
	
	deoglSkinShader::eiutInstHeightRemap, // eiutInstHeightRemap ( vec2 )
	deoglSkinShader::eiutInstNormalStrength, // eiutInstNormalStrength ( float )
	deoglSkinShader::eiutInstNormalSolidityMultiplier, // eiutInstNormalSolidityMultiplier ( float )
	
	deoglSkinShader::eiutInstRoughnessRemap, // eiutInstRoughnessRemap ( vec2 )
	deoglSkinShader::eiutInstRoughnessGamma, // eiutInstRoughnessGamma ( float )
	deoglSkinShader::eiutInstRoughnessSolidityMultiplier, // eiutInstRoughnessSolidityMultiplier ( float )
	
	deoglSkinShader::eiutInstEmissivityIntensity, // eiutInstEmissivityIntensity ( vec3 )
	deoglSkinShader::eiutInstRefractionDistortStrength, // eiutInstRefractionDistortStrength ( float )
	
	deoglSkinShader::eiutInstEnvRoomEmissivityIntensity, // eiutInstEnvRoomEmissivityIntensity ( vec3 )
	deoglSkinShader::eiutInstReflectivitySolidityMultiplier, // eiutInstReflectivitySolidityMultiplier ( float )
	
	deoglSkinShader::eiutInstEnvRoomOffset, // eiutInstEnvRoomOffset ( vec3 )
	deoglSkinShader::eiutInstReflectivityMultiplier, // eiutInstReflectivityMultiplier ( float )
	
	deoglSkinShader::eiutInstEnvRoomSize, // eiutInstEnvRoomSize ( vec2 )
	deoglSkinShader::eiutInstVariationEnableScale, // eiutInstVariationEnableScale ( vec2 )
	
	deoglSkinShader::eiutInstRimEmissivityIntensity, // eiutInstRimEmissivityIntensity ( vec3 )
	deoglSkinShader::eiutInstRimAngle, // eiutInstRimAngle ( float )
	deoglSkinShader::eiutInstRimExponent, // eiutInstRimExponent ( float )
	
	deoglSkinShader::eiutInstOutlineColor, // eiutInstOutlineColor ( vec3 )
	deoglSkinShader::eiutInstOutlineThickness, // eiutInstOutlineThickness ( float )
	
	deoglSkinShader::eiutInstOutlineColorTint, // eiutInstOutlineColorTint ( vec3 )
	deoglSkinShader::eiutInstOutlineSolidity, // eiutInstOutlineSolidity ( float )
	
	deoglSkinShader::eiutInstOutlineEmissivity, // eiutInstOutlineEmissivity ( vec3 )
	deoglSkinShader::eiutInstOutlineEmissivityTint // eiutInstOutlineEmissivityTint ( vec3 )
};



// Class deoglSkinShader
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinShader::deoglSkinShader( deoglRenderThread &renderThread, const deoglSkinShaderConfig &config ) :
pRenderThread( renderThread ),

pConfig( config ),
pUsedTextureTargetCount( 0 ),
pUsedInstanceUniformTargetCount( 0 ),
pTargetSPBInstanceIndexBase( -1 ),
pTargetDrawIDOffset( -1 ),
pSources( NULL ),
pShader( NULL )
{
	int i;
	
	for( i=0; i<ETT_COUNT; i++ ){
		pTextureTargets[ i ] = -1;
	}
	for( i=0; i<EIUT_COUNT; i++ ){
		pInstanceUniformTargets[ i ] = -1;
	}
}

deoglSkinShader::~deoglSkinShader(){
	if( pShader ){
		delete pShader;
	}
	if( pSources ){
		delete pSources;
	}
}



// Management
///////////////

int deoglSkinShader::GetTextureTarget( deoglSkinShader::eTextureTargets target ) const{
	return pTextureTargets[ target ];
}

void deoglSkinShader::SetTextureTarget( deoglSkinShader::eTextureTargets target, int index ){
	if( index < -1 ){
		DETHROW( deeInvalidParam );
	}
	pTextureTargets[ target ] = index;
}

void deoglSkinShader::SetUsedTextureTargetCount( int usedTextureTargetCount ){
	if( usedTextureTargetCount < 0 ){
		DETHROW( deeInvalidParam );
	}
	pUsedTextureTargetCount = usedTextureTargetCount;
}



int deoglSkinShader::GetInstanceUniformTarget( deoglSkinShader::eInstanceUniformTargets target ) const{
	return pInstanceUniformTargets[ target ];
}

void deoglSkinShader::SetInstanceUniformTarget( deoglSkinShader::eInstanceUniformTargets target, int index ){
	if( index < -1 ){
		DETHROW( deeInvalidParam );
	}
	pInstanceUniformTargets[ target ] = index;
}



void deoglSkinShader::PrepareShader(){
	if( ! pShader ){
		GenerateShader();
	}
}



deoglSPBlockUBO *deoglSkinShader::CreateSPBRender( deoglRenderThread &renderThread ){
	if( ! pglUniformBlockBinding ){
		DETHROW( deeInvalidParam );
	}
	
	// this shader parameter block will not be optimzed. the layout is always the
	// same no matter what configuration is used for skins. this is also why this
	// method is a static method not an regular method
	
	deoglSPBlockUBO *spb = nullptr;
	
	try{
		spb = new deoglSPBlockUBO( renderThread );
		spb->SetRowMajor( ! renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Broken() );
		spb->SetParameterCount( ERUT_COUNT );
		
		spb->GetParameterAt( erutAmbient ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 1 ); // vec4
		spb->GetParameterAt( erutMatrixV ).SetAll( deoglSPBParameter::evtFloat, 4, 3, 6 ); // mat4x3
		spb->GetParameterAt( erutMatrixP ).SetAll( deoglSPBParameter::evtFloat, 4, 4, 2 ); // mat4
		spb->GetParameterAt( erutMatrixVP ).SetAll( deoglSPBParameter::evtFloat, 4, 4, 6 ); // mat4
		spb->GetParameterAt( erutMatrixVn ).SetAll( deoglSPBParameter::evtFloat, 3, 3, 6 ); // mat3
		spb->GetParameterAt( erutMatrixEnvMap ).SetAll( deoglSPBParameter::evtFloat, 3, 3, 1 ); // mat3
		spb->GetParameterAt( erutMatrixSkyBody ).SetAll( deoglSPBParameter::evtFloat, 4, 4, 6 ); // mat4
		spb->GetParameterAt( erutDepthToPosition ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 2 ); // vec4
		spb->GetParameterAt( erutDepthToPosition2 ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 2 ); // vec2
		
		spb->GetParameterAt( erutDepthTransform ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
		spb->GetParameterAt( erutEnvMapLodLevel ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
		spb->GetParameterAt( erutNorRoughCorrStrength ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
		
		spb->GetParameterAt( erutSkinDoesReflections ).SetAll( deoglSPBParameter::evtBool, 1, 1, 1 ); // bool
		spb->GetParameterAt( erutFlipCulling ).SetAll( deoglSPBParameter::evtBool, 1, 1, 1 ); // bool
		spb->GetParameterAt( erutClearDepthValue ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
		
		spb->GetParameterAt( erutViewport ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 1 ); // vec4
		spb->GetParameterAt( erutClipPlane ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 2 ); // vec4
		spb->GetParameterAt( erutScreenSpace ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 1 ); // vec4
		
		spb->GetParameterAt( erutRenderSize ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
		
		spb->GetParameterAt( erutMipMapParams ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 1 ); // vec4
		
		spb->GetParameterAt( erutDepthOffset ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 4 ); // vec4
		
		spb->GetParameterAt( erutParticleLightHack ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
		spb->GetParameterAt( erutBillboardZScale ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
		
		spb->GetParameterAt( erutFadeRange ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
		
		spb->GetParameterAt( erutCameraStereoMatrix ).SetAll( deoglSPBParameter::evtFloat, 4, 3, 1 ); // mat4x3
		
		spb->GetParameterAt( erutCameraRange ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
		spb->GetParameterAt( erutCameraAdaptedIntensity ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
		
		spb->GetParameterAt( erutDepthSampleOffset ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
		
		spb->GetParameterAt( erutFSScreenCoordToTexCoord ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 1 ); // vec4
		spb->GetParameterAt( erutFSTexCoordToScreenCoord ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 1 ); // vec4
		
		spb->GetParameterAt( erutSSAOParams1 ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 1 ); // vec4
		spb->GetParameterAt( erutSSAOParams2 ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 1 ); // vec4
		spb->GetParameterAt( erutSSAOParams3 ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
		
		spb->GetParameterAt( erutSSSSSParams1 ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 1 ); // vec4
		spb->GetParameterAt( erutSSSSSParams2 ).SetAll( deoglSPBParameter::evtInt, 2, 1, 1 ); // ivec2
		
		spb->GetParameterAt( erutSSRParams1 ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 1 ); // vec4
		spb->GetParameterAt( erutSSRParams2 ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 1 ); // vec4
		spb->GetParameterAt( erutSSRParams3 ).SetAll( deoglSPBParameter::evtInt, 4, 1, 1 ); // ivec4
		
		spb->GetParameterAt( erutAOSelfShadow ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
		spb->GetParameterAt( erutLumFragCoordScale ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
		
		spb->GetParameterAt( erutGIRayMatrix ).SetAll( deoglSPBParameter::evtFloat, 4, 3, 1 ); // mat4x3
		spb->GetParameterAt( erutGIRayMatrixNormal ).SetAll( deoglSPBParameter::evtFloat, 3, 3, 1 ); // mat3
		
		spb->GetParameterAt( erutGIHighestCascade ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
		
		spb->GetParameterAt( erutToneMapSceneKey ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
		spb->GetParameterAt( erutToneMapAdaption ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
		spb->GetParameterAt( erutToneMapBloom ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
		
		spb->GetParameterAt( erutDebugDepthTransform ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
		
		spb->GetParameterAt( erutConditions1 ).SetAll( deoglSPBParameter::evtBool, 4, 1, 1 ); // bvec4
		
		spb->MapToStd140();
		spb->SetBindingPoint( deoglSkinShader::eubRenderParameters );
		
	}catch( const deException & ){
		if( spb ){
			spb->FreeReference();
		}
		throw;
	}
	
	return spb;
}

deoglSPBlockUBO *deoglSkinShader::CreateSPBOccMap( deoglRenderThread &renderThread ){
	if( ! pglUniformBlockBinding ){
		DETHROW( deeInvalidParam );
	}
	
	deoglSPBlockUBO *ompb = NULL;
	
	try{
		ompb = new deoglSPBlockUBO( renderThread );
		ompb->SetRowMajor( ! renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Broken() );
		ompb->SetParameterCount( 4 );
		ompb->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtFloat, 4, 4, 6 ); // mat4 pMatrixVP[ 6 ]
		ompb->GetParameterAt( 1 ).SetAll( deoglSPBParameter::evtFloat, 4, 3, 6 ); // mat4x3 pMatrixV[ 6 ]
		ompb->GetParameterAt( 2 ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 6 ); // vec4 pTransformZ[ 6 ]
		ompb->GetParameterAt( 3 ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2 pZToDepth
		
		ompb->MapToStd140();
		ompb->SetBindingPoint( 0 );//deoglSkinShader::eubRenderParameters );
		
	}catch( const deException & ){
		if( ompb ){
			ompb->FreeReference();
		}
		throw;
	}
	
	return ompb;
}

deoglSPBlockUBO *deoglSkinShader::CreateSPBSpecial( deoglRenderThread &renderThread ){
	if( ! pglUniformBlockBinding ){
		DETHROW( deeInvalidParam );
	}
	
	deoglSPBlockUBO *spb = NULL;
	
	try{
		spb = new deoglSPBlockUBO( renderThread );
		spb->SetRowMajor( ! renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Broken() );
		spb->SetParameterCount( 1 );
		
		spb->GetParameterAt( esutLayerVisibility ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int pLayerVisibility
		
		spb->MapToStd140();
		spb->SetBindingPoint( deoglSkinShader::eubSpecialParameters );
		
	}catch( const deException & ){
		if( spb ){
			spb->FreeReference();
		}
		throw;
	}
	
	return spb;
}



deoglSPBlockUBO *deoglSkinShader::CreateSPBTexParam( deoglRenderThread &renderThread ){
	if( ! pglUniformBlockBinding ){
		DETHROW( deeInvalidParam );
	}
	
	deoglSPBlockUBO *spb = NULL;
	
	try{
		spb = new deoglSPBlockUBO( renderThread );
		spb->SetRowMajor( renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
		spb->SetParameterCount( ETUT_COUNT );
		
		int i;
		for( i=0; i<ETUT_COUNT; i++ ){
			spb->GetParameterAt( i ).SetAll( vTextureSPBParamDefs[ i ].dataType,
				vTextureSPBParamDefs[ i ].componentCount, vTextureSPBParamDefs[ i ].vectorCount, 1 );
		}
		
		spb->MapToStd140();
		spb->SetBindingPoint( deoglSkinShader::eubTextureParameters );
		
	}catch( const deException & ){
		if( spb ){
			spb->FreeReference();
		}
		throw;
	}
	
	return spb;
}

deoglSPBlockUBO *deoglSkinShader::CreateSPBInstParam() const{
	if( ! pglUniformBlockBinding ){
		DETHROW( deeInvalidParam );
	}
	
	deoglSPBlockUBO *spb = NULL;
	int i;
	
	try{
		spb = new deoglSPBlockUBO( pRenderThread );
		spb->SetRowMajor( ! pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Broken() );
		spb->SetParameterCount( pUsedInstanceUniformTargetCount );
		
		for( i=0; i<EIUT_COUNT; i++ ){
			const int target = pInstanceUniformTargets[ i ];
			if( target != -1 ){
				spb->GetParameterAt( target ).SetAll( vInstanceSPBParamDefs[ i ].dataType,
					vInstanceSPBParamDefs[ i ].componentCount,
					vInstanceSPBParamDefs[ i ].vectorCount, 1 );
			}
		}
		
		spb->MapToStd140();
		spb->SetBindingPoint( deoglSkinShader::eubInstanceParameters );
		
	}catch( const deException & ){
		if( spb ){
			spb->FreeReference();
		}
		throw;
	}
	
	return spb;
}

deoglSPBlockUBO *deoglSkinShader::CreateLayoutSkinInstanceUBO( deoglRenderThread &renderThread ){
	deoglSPBlockUBO *ubo = NULL;
	int i;
	
	try{
		ubo = new deoglSPBlockUBO( renderThread );
		ubo->SetRowMajor( renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
		ubo->SetParameterCount( vUBOInstParamMapCount );
		
		for( i=0; i<vUBOInstParamMapCount; i++ ){
			const sSPBParameterDefinition &pdef = vInstanceSPBParamDefs[ vUBOInstParamMap[ i ] ];
			ubo->GetParameterAt( i ).SetAll( pdef.dataType, pdef.componentCount, pdef.vectorCount, 1 );
		}
		
		ubo->MapToStd140();
		ubo->SetBindingPoint( eubInstanceParameters );
		
	}catch( const deException & ){
		if( ubo ){
			ubo->FreeReference();
		}
		throw;
	}
	
	return ubo;
}

deoglSPBlockSSBO *deoglSkinShader::CreateLayoutSkinInstanceSSBO( deoglRenderThread &renderThread ){
	deoglSPBlockSSBO *ssbo = NULL;
	int i;
	
	try{
		ssbo = new deoglSPBlockSSBO( renderThread );
		ssbo->SetRowMajor( renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
		ssbo->SetParameterCount( vUBOInstParamMapCount );
		
		for( i=0; i<vUBOInstParamMapCount; i++ ){
			const sSPBParameterDefinition &pdef = vInstanceSPBParamDefs[ vUBOInstParamMap[ i ] ];
			ssbo->GetParameterAt( i ).SetAll( pdef.dataType, pdef.componentCount, pdef.vectorCount, 1 );
		}
		
		ssbo->MapToStd140();
		ssbo->SetBindingPoint( essboInstanceParameters );
		
	}catch( const deException & ){
		if( ssbo ){
			ssbo->FreeReference();
		}
		throw;
	}
	
	return ssbo;
}

deoglSPBlockUBO *deoglSkinShader::CreateLayoutSkinTextureUBO( deoglRenderThread &renderThread ){
	deoglSPBlockUBO * const spb = new deoglSPBlockUBO( renderThread );
	int i;
	
	try{
		spb->SetRowMajor( renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
		spb->SetParameterCount( ETUT_COUNT );
		
		for( i=0; i<ETUT_COUNT; i++ ){
			spb->GetParameterAt( i ).SetAll( vTextureSPBParamDefs[ i ].dataType,
				vTextureSPBParamDefs[ i ].componentCount, vTextureSPBParamDefs[ i ].vectorCount, 1 );
		}
		
		spb->MapToStd140();
		spb->SetBindingPoint( eubTextureParameters );
		
	}catch( const deException & ){
		if( spb ){
			spb->FreeReference();
		}
		throw;
	}
	
	return spb;
}

deoglSPBlockSSBO *deoglSkinShader::CreateLayoutSkinTextureSSBO( deoglRenderThread &renderThread ){
	deoglSPBlockSSBO * const spb = new deoglSPBlockSSBO( renderThread );
	int i;
	
	try{
		spb->SetRowMajor( renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
		spb->SetParameterCount( ETUT_COUNT );
		
		for( i=0; i<ETUT_COUNT; i++ ){
			spb->GetParameterAt( i ).SetAll( vTextureSPBParamDefs[ i ].dataType,
				vTextureSPBParamDefs[ i ].componentCount, vTextureSPBParamDefs[ i ].vectorCount, 1 );
		}
		
		spb->MapToStd140();
		spb->SetBindingPoint( essboTextureParameters );
		
	}catch( const deException & ){
		if( spb ){
			spb->FreeReference();
		}
		throw;
	}
	
	return spb;
}



void deoglSkinShader::SetTexParamsInInstParamSPB( deoglShaderParameterBlock &paramBlock,
const deoglSkinTexture &skinTexture ) const{
	SetTexParamsInInstParamSPB( paramBlock, 0, skinTexture );
}

void deoglSkinShader::SetTexParamsInInstParamSPB( deoglShaderParameterBlock &paramBlock,
int element, const deoglSkinTexture &skinTexture ) const{
	if( pInstanceUniformTargets[ eiutIndexSPBTexParams ] != -1 ){
		const deoglSharedSPBElement * const spbElement = skinTexture.GetSharedSPBElement();
		paramBlock.SetParameterDataInt( pInstanceUniformTargets[ eiutIndexSPBTexParams ],
			element, spbElement ? spbElement->GetIndex() : 0 );
	}
}

void deoglSkinShader::SetDynTexParamsInInstParamSPB( deoglShaderParameterBlock &paramBlock,
const deoglSkinTexture &skinTexture, deoglSkinState *skinState, deoglRDynamicSkin *dynamicSkin ) const{
	SetDynTexParamsInInstParamSPB( paramBlock, 0, skinTexture, skinState, dynamicSkin );
}

void deoglSkinShader::SetDynTexParamsInInstParamSPB( deoglShaderParameterBlock &paramBlock,
int element, const deoglSkinTexture &skinTexture, deoglSkinState *skinState,
deoglRDynamicSkin *dynamicSkin ) const{
	/*
	if( pInstanceUniformTargets[ eiutTCTransformColor ] != -1 ){
		paramBlock.SetParameterDataVec4( pInstanceUniformTargets[ eiutTCTransformColor ],
			element, 1.0f, 1.0f, 0.0f, 0.0f );
	}
	if( pInstanceUniformTargets[ eiutTCTransformNormal ] != -1 ){
		paramBlock.SetParameterDataVec4( pInstanceUniformTargets[ eiutTCTransformNormal ],
			element, 1.0f, 1.0f, 0.0f, 0.0f );
	}
	if( pInstanceUniformTargets[ eiutTCTransformReflectivity ] != -1 ){
		paramBlock.SetParameterDataVec4( pInstanceUniformTargets[ eiutTCTransformReflectivity ],
			element, 1.0f, 1.0f, 0.0f, 0.0f );
	}
	if( pInstanceUniformTargets[ eiutTCTransformEmissivity ] != -1 ){
		paramBlock.SetParameterDataVec4( pInstanceUniformTargets[ eiutTCTransformEmissivity ],
			element, 1.0f, 1.0f, 0.0f, 0.0f );
	}
	if( pInstanceUniformTargets[ eiutTCTransformRefractionDistort ] != -1 ){
		paramBlock.SetParameterDataVec4( pInstanceUniformTargets[ eiutTCTransformRefractionDistort ],
			element, 1.0f, 1.0f, 0.0f, 0.0f );
	}
	if( pInstanceUniformTargets[ eiutTCTransformSolidity ] != -1 ){
		paramBlock.SetParameterDataVec4( pInstanceUniformTargets[ eiutTCTransformSolidity ],
			element, 1.0f, 1.0f, 0.0f, 0.0f );
	}
	if( pInstanceUniformTargets[ eiutTCTransformAO ] != -1 ){
		paramBlock.SetParameterDataVec4( pInstanceUniformTargets[ eiutTCTransformAO ],
			element, 1.0f, 1.0f, 0.0f, 0.0f );
	}
	*/
	
	if( pInstanceUniformTargets[ eiutInstColorTint ] != -1 ){
		const deoglSkinTextureProperty &property = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empColorTint );
		decColor colorTint( property.ResolveColor( skinState, dynamicSkin, skinTexture.GetColorTint() ) );
		
		colorTint.r = powf( decMath::max( colorTint.r, 0.0f ), 2.2f );
		colorTint.g = powf( decMath::max( colorTint.g, 0.0f ), 2.2f );
		colorTint.b = powf( decMath::max( colorTint.b, 0.0f ), 2.2f );
		colorTint.a = 1.0f;
		
		paramBlock.SetParameterDataVec3( pInstanceUniformTargets[ eiutInstColorTint ], element, colorTint );
	}
	
	if( pInstanceUniformTargets[ eiutInstColorGamma ] != -1 ){
		const deoglSkinTextureProperty &property = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empColorGamma );
		const float gamma = property.ResolveAsFloat( skinState, dynamicSkin, skinTexture.GetColorGamma() );
		
		paramBlock.SetParameterDataFloat( pInstanceUniformTargets[ eiutInstColorGamma ],
			element, decMath::max( gamma, 0.0f ) );
	}
	
	if( pInstanceUniformTargets[ eiutInstColorSolidityMultiplier ] != -1 ){
		const deoglSkinTextureProperty &property = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empColorSolidityMultiplier );
		const float weight = property.ResolveAsFloat( skinState, dynamicSkin,
			skinTexture.GetColorSolidityMultiplier() );
		
		paramBlock.SetParameterDataFloat( pInstanceUniformTargets[ eiutInstColorSolidityMultiplier ],
			element, decMath::clamp( weight, 0.0f, 1.0f ) );
	}
	
	if( pInstanceUniformTargets[ eiutInstAOSolidityMultiplier ] != -1 ){
		const deoglSkinTextureProperty &property = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empAmbientOcclusionSolidityMultiplier );
		const float weight = property.ResolveAsFloat( skinState, dynamicSkin,
			skinTexture.GetAmbientOcclusionSolidityMultiplier() );
		
		paramBlock.SetParameterDataFloat( pInstanceUniformTargets[ eiutInstAOSolidityMultiplier ],
			element, decMath::clamp( weight, 0.0f, 1.0f ) );
	}
	
	if( pInstanceUniformTargets[ eiutInstTransparencyMultiplier ] != -1 ){
		const deoglSkinTextureProperty &property = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empTransparencyMultiplier );
		const float weight = property.ResolveAsFloat( skinState, dynamicSkin,
			skinTexture.GetTransparencyMultiplier() );
		
		paramBlock.SetParameterDataFloat( pInstanceUniformTargets[ eiutInstTransparencyMultiplier ],
			element, decMath::clamp( weight, 0.0f, 1.0f ) );
	}
	
	if( pInstanceUniformTargets[ eiutInstSolidityMultiplier ] != -1 ){
		const deoglSkinTextureProperty &property = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empSolidityMultiplier );
		const float weight = property.ResolveAsFloat( skinState, dynamicSkin,
			skinTexture.GetSolidityMultiplier() );
		
		paramBlock.SetParameterDataFloat( pInstanceUniformTargets[ eiutInstSolidityMultiplier ],
			element, decMath::clamp( weight, 0.0f, 1.0f ) );
	}
	
	if( pInstanceUniformTargets[ eiutInstHeightRemap ] != -1 ){
		const deoglSkinTextureProperty &propertyScale = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empHeightScale );
		const deoglSkinTextureProperty &propertyOffset = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empHeightOffset );
		const float scale = propertyScale.ResolveAsFloat( skinState, dynamicSkin,
			skinTexture.GetHeightScale() );
		const float offset = propertyOffset.ResolveAsFloat( skinState, dynamicSkin,
			skinTexture.GetHeightOffset() );
		
		// (texHeight - 0.5 + offset) * scale = texHeight*scale + (offset-0.5)*scale
		paramBlock.SetParameterDataVec2( pInstanceUniformTargets[ eiutInstHeightRemap ],
			element, scale, ( offset - 0.5f ) * scale );
	}
	
	if( pInstanceUniformTargets[ eiutInstNormalStrength ] != -1 ){
		const deoglSkinTextureProperty &property = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empNormalStrength );
		const float strength = property.ResolveAsFloat( skinState, dynamicSkin,
			skinTexture.GetNormalStrength() );
		
		paramBlock.SetParameterDataFloat( pInstanceUniformTargets[ eiutInstNormalStrength ],
			element, strength );
	}
	
	if( pInstanceUniformTargets[ eiutInstNormalSolidityMultiplier ] != -1 ){
		const deoglSkinTextureProperty &property = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empNormalSolidityMultiplier );
		const float weight = property.ResolveAsFloat( skinState, dynamicSkin,
			skinTexture.GetNormalSolidityMultiplier() );
		
		paramBlock.SetParameterDataFloat( pInstanceUniformTargets[ eiutInstNormalSolidityMultiplier ],
			element, decMath::clamp( weight, 0.0f, 1.0f ) );
	}
	
	if( pInstanceUniformTargets[ eiutInstRoughnessRemap ] != -1 ){
		const deoglSkinTextureProperty &propertyLower = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empRoughnessRemapLower );
		const deoglSkinTextureProperty &propertyUpper = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empRoughnessRemapUpper );
		const float lower = decMath::clamp( propertyLower.ResolveAsFloat( skinState, dynamicSkin,
			skinTexture.GetRoughnessRemapLower() ), 0.0f, 1.0f );
		const float upper = decMath::clamp( propertyUpper.ResolveAsFloat( skinState, dynamicSkin,
			skinTexture.GetRoughnessRemapUpper() ), 0.0f, 1.0f );
		
		paramBlock.SetParameterDataVec2( pInstanceUniformTargets[ eiutInstRoughnessRemap ],
			element, upper - lower, lower );
	}
	
	if( pInstanceUniformTargets[ eiutInstRoughnessGamma ] != -1 ){
		const deoglSkinTextureProperty &property = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empRoughnessGamma );
		const float gamma = property.ResolveAsFloat( skinState, dynamicSkin,
			skinTexture.GetRoughnessGamma() );
		
		paramBlock.SetParameterDataFloat( pInstanceUniformTargets[ eiutInstRoughnessGamma ],
			element, decMath::max( gamma, 0.0f ) );
	}
	
	if( pInstanceUniformTargets[ eiutInstRoughnessSolidityMultiplier ] != -1 ){
		const deoglSkinTextureProperty &property = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empRoughnessSolidityMultiplier );
		const float weight = property.ResolveAsFloat( skinState, dynamicSkin,
			skinTexture.GetRoughnessSolidityMultiplier() );
		
		paramBlock.SetParameterDataFloat( pInstanceUniformTargets[ eiutInstRoughnessSolidityMultiplier ],
			element, decMath::clamp( weight, 0.0f, 1.0f ) );
	}
	
	if( pInstanceUniformTargets[ eiutInstRefractionDistortStrength ] != -1 ){
		const deoglSkinTextureProperty &property = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empRefractDistortStrength );
		const float strength = property.ResolveAsFloat( skinState, dynamicSkin,
			skinTexture.GetRefractDistortStrength() );
		
		paramBlock.SetParameterDataFloat( pInstanceUniformTargets[ eiutInstRefractionDistortStrength ],
			element, decMath::max( strength, 0.0f ) );
	}
	
	if( pInstanceUniformTargets[ eiutInstReflectivitySolidityMultiplier ] != -1 ){
		const deoglSkinTextureProperty &property = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empReflectivitySolidityMultiplier );
		const float weight = property.ResolveAsFloat( skinState, dynamicSkin,
			skinTexture.GetReflectivitySolidityMultiplier() );
		
		paramBlock.SetParameterDataFloat( pInstanceUniformTargets[ eiutInstReflectivitySolidityMultiplier ],
			element, decMath::clamp( weight, 0.0f, 1.0f ) );
	}
	
	if( pInstanceUniformTargets[ eiutInstEmissivityIntensity ] != -1 ){
		const deoglSkinTextureProperty &propertyTint = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empEmissivityTint );
		decColor tint( propertyTint.ResolveColor( skinState, dynamicSkin,
			skinTexture.GetEmissivityTint() ) );
		const deoglSkinTextureProperty &property = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empEmissivityIntensity );
		const float intensity = property.ResolveAsFloat( skinState, dynamicSkin,
			skinTexture.GetEmissivityIntensity() );
		
		tint.r = powf( decMath::max( tint.r, 0.0f ), 2.2f );
		tint.g = powf( decMath::max( tint.g, 0.0f ), 2.2f );
		tint.b = powf( decMath::max( tint.b, 0.0f ), 2.2f );
		
		paramBlock.SetParameterDataVec3( pInstanceUniformTargets[ eiutInstEmissivityIntensity ],
			element, tint * decMath::max( intensity, 0.0f ) );
	}
	
	if( pInstanceUniformTargets[ eiutInstEnvRoomSize ] != -1 ){
		const deoglSkinTextureProperty &property = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empEnvironmentRoomSize );
		const decVector2 size = property.ResolveVector2( skinState, dynamicSkin,
			skinTexture.GetEnvironmentRoomSize() ).Largest( decVector2() );
		
		paramBlock.SetParameterDataVec2( pInstanceUniformTargets[ eiutInstEnvRoomSize ],
			element, 1.0f / size.x, -1.0f / size.y );
	}
	
	if( pInstanceUniformTargets[ eiutInstEnvRoomOffset ] != -1 ){
		const deoglSkinTextureProperty &property = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empEnvironmentRoomOffset );
		const decVector offset( property.ResolveVector( skinState, dynamicSkin,
			skinTexture.GetEnvironmentRoomOffset() ) );
		
		paramBlock.SetParameterDataVec3( pInstanceUniformTargets[ eiutInstEnvRoomOffset ],
			element, offset );
	}
	
	if( pInstanceUniformTargets[ eiutInstEnvRoomEmissivityIntensity ] != -1 ){
		const deoglSkinTextureProperty &propertyTint = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empEnvironmentRoomEmissivityTint );
		decColor tint( propertyTint.ResolveColor( skinState, dynamicSkin,
			skinTexture.GetEnvironmentRoomEmissivityTint() ) );
		const deoglSkinTextureProperty &property = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empEnvironmentRoomEmissivityIntensity );
		const float intensity = property.ResolveAsFloat( skinState, dynamicSkin,
			skinTexture.GetEnvironmentRoomEmissivityIntensity() );
		
		tint.r = powf( decMath::max( tint.r, 0.0f ), 2.2f );
		tint.g = powf( decMath::max( tint.g, 0.0f ), 2.2f );
		tint.b = powf( decMath::max( tint.b, 0.0f ), 2.2f );
		
		paramBlock.SetParameterDataVec3( pInstanceUniformTargets[ eiutInstEnvRoomEmissivityIntensity ],
			element, tint * decMath::max( intensity, 0.0f ) );
	}
	
	if( pInstanceUniformTargets[ eiutInstVariationEnableScale ] != -1 ){
		const deoglSkinTextureProperty &propertyU = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empVariationU );
		const deoglSkinTextureProperty &propertyV = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empVariationV );
		const bool scaleU = propertyU.ResolveAsBool( skinState, dynamicSkin,
			skinTexture.GetVariationU() );
		const bool scaleV = propertyV.ResolveAsBool( skinState, dynamicSkin,
			skinTexture.GetVariationV() );
		
		paramBlock.SetParameterDataVec2( pInstanceUniformTargets[ eiutInstVariationEnableScale ],
			element, scaleU ? 1.0f : 0.0f, scaleV ? 1.0f : 0.0f );
	}
	
	if( pInstanceUniformTargets[ eiutInstReflectivityMultiplier ] != -1 ){
		const deoglSkinTextureProperty &property = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empReflectivityMultiplier );
		const float multiplier = property.ResolveAsFloat( skinState, dynamicSkin,
			skinTexture.GetReflectivityMultiplier() );
		
		paramBlock.SetParameterDataFloat( pInstanceUniformTargets[ eiutInstReflectivityMultiplier ],
			element, decMath::clamp( multiplier, 0.0f, 1.0f ) );
	}
	
	if( pInstanceUniformTargets[ eiutInstRimEmissivityIntensity ] != -1 ){
		const deoglSkinTextureProperty &propertyTint = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empRimEmissivityTint );
		decColor tint( propertyTint.ResolveColor( skinState, dynamicSkin,
			skinTexture.GetRimEmissivityTint() ) );
		const deoglSkinTextureProperty &property = skinTexture
			.GetMaterialPropertyAt( deoglSkinTexture::empRimEmissivityIntensity );
		const float intensity = property.ResolveAsFloat( skinState, dynamicSkin,
			skinTexture.GetRimEmissivityIntensity() );
		
		tint.r = powf( decMath::max( tint.r, 0.0f ), 2.2f );
		tint.g = powf( decMath::max( tint.g, 0.0f ), 2.2f );
		tint.b = powf( decMath::max( tint.b, 0.0f ), 2.2f );
		
		paramBlock.SetParameterDataVec3( pInstanceUniformTargets[ eiutInstRimEmissivityIntensity ],
			element, tint * decMath::max( intensity, 0.0f ) );
	}
	
	if( pInstanceUniformTargets[ eiutInstRimAngle ] != -1 ){
		const deoglSkinTextureProperty &property = skinTexture.
			GetMaterialPropertyAt( deoglSkinTexture::empRimAngle );
		const float angle = property.ResolveAsFloat( skinState, dynamicSkin,
			skinTexture.GetRimAngle() );
		
		paramBlock.SetParameterDataFloat( pInstanceUniformTargets[ eiutInstRimAngle ],
			element, angle > 0.001f ? 1.0f / ( angle * HALF_PI ) : 0.0f );
	}
	
	if( pInstanceUniformTargets[ eiutInstRimExponent ] != -1 ){
		const deoglSkinTextureProperty &property = skinTexture.
			GetMaterialPropertyAt( deoglSkinTexture::empRimExponent );
		const float exponent = property.ResolveAsFloat( skinState, dynamicSkin,
			skinTexture.GetRimExponent() );
		
		paramBlock.SetParameterDataFloat( pInstanceUniformTargets[ eiutInstRimExponent ],
			element, decMath::max( exponent, 0.001f ) );
	}
	
	if( pInstanceUniformTargets[ eiutInstOutlineColor ] != -1 ){
		const deoglSkinTextureProperty &property = skinTexture.
			GetMaterialPropertyAt( deoglSkinTexture::empOutlineColor );
		decColor color( property.ResolveColor( skinState, dynamicSkin, skinTexture.GetOutlineColor() ) );
		
		color.r = powf( decMath::max( color.r, 0.0f ), 2.2f );
		color.g = powf( decMath::max( color.g, 0.0f ), 2.2f );
		color.b = powf( decMath::max( color.b, 0.0f ), 2.2f );
		color.a = 1.0f;
		
		paramBlock.SetParameterDataVec3( pInstanceUniformTargets[ eiutInstOutlineColor ], element, color );
	}
	
	if( pInstanceUniformTargets[ eiutInstOutlineColorTint ] != -1 ){
		const deoglSkinTextureProperty &property = skinTexture.
			GetMaterialPropertyAt( deoglSkinTexture::empOutlineColorTint );
		decColor color( property.ResolveColor( skinState, dynamicSkin, skinTexture.GetOutlineColorTint() ) );
		
		color.r = powf( decMath::max( color.r, 0.0f ), 2.2f );
		color.g = powf( decMath::max( color.g, 0.0f ), 2.2f );
		color.b = powf( decMath::max( color.b, 0.0f ), 2.2f );
		color.a = 1.0f;
		
		paramBlock.SetParameterDataVec3( pInstanceUniformTargets[ eiutInstOutlineColorTint ], element, color );
	}
	
	if( pInstanceUniformTargets[ eiutInstOutlineThickness ] != -1 ){
		const deoglSkinTextureProperty &property = skinTexture.
			GetMaterialPropertyAt( deoglSkinTexture::empOutlineThickness );
		const float thickness = property.ResolveAsFloat( skinState, dynamicSkin,
			skinTexture.GetOutlineThickness() );
		
		paramBlock.SetParameterDataFloat( pInstanceUniformTargets[ eiutInstOutlineThickness ],
			element, decMath::max( thickness, 0.0f ) );
	}
	
	if( pInstanceUniformTargets[ eiutInstOutlineSolidity ] != -1 ){
		const deoglSkinTextureProperty &property = skinTexture.
			GetMaterialPropertyAt( deoglSkinTexture::empOutlineSolidity );
		const float solidity = property.ResolveAsFloat( skinState, dynamicSkin,
			skinTexture.GetOutlineSolidity() );
		
		paramBlock.SetParameterDataFloat( pInstanceUniformTargets[ eiutInstOutlineSolidity ],
			element, decMath::clamp( solidity, 0.0f, 1.0f ) );
	}
	
	if( pInstanceUniformTargets[ eiutInstOutlineEmissivity ] != -1 ){
		const deoglSkinTextureProperty &propertyColor = skinTexture.
			GetMaterialPropertyAt( deoglSkinTexture::empOutlineEmissivity );
		decColor emissivity( propertyColor.ResolveColor( skinState, dynamicSkin,
			skinTexture.GetOutlineEmissivity() ) );
		const deoglSkinTextureProperty &propertyIntensity = skinTexture.
			GetMaterialPropertyAt( deoglSkinTexture::empOutlineEmissivityIntensity );
		const float intensity = propertyIntensity.ResolveAsFloat( skinState, dynamicSkin,
			skinTexture.GetOutlineEmissivityIntensity() );
		
		emissivity.r = powf( decMath::max( emissivity.r, 0.0f ), 2.2f );
		emissivity.g = powf( decMath::max( emissivity.g, 0.0f ), 2.2f );
		emissivity.b = powf( decMath::max( emissivity.b, 0.0f ), 2.2f );
		
		paramBlock.SetParameterDataVec3( pInstanceUniformTargets[ eiutInstOutlineEmissivity ],
			element, emissivity * decMath::max( intensity, 0.0f ) );
	}
	
	if( pInstanceUniformTargets[ eiutInstOutlineEmissivityTint ] != -1 ){
		const deoglSkinTextureProperty &property = skinTexture.
			GetMaterialPropertyAt( deoglSkinTexture::empOutlineEmissivityTint );
		decColor tint( property.ResolveColor( skinState, dynamicSkin,
			skinTexture.GetOutlineEmissivityTint() ) );
		
		tint.r = powf( decMath::max( tint.r, 0.0f ), 2.2f );
		tint.g = powf( decMath::max( tint.g, 0.0f ), 2.2f );
		tint.b = powf( decMath::max( tint.b, 0.0f ), 2.2f );
		
		paramBlock.SetParameterDataVec3( pInstanceUniformTargets[ eiutInstOutlineEmissivityTint ], element, tint );
	}
}

void deoglSkinShader::SetTUCCommon( deoglTexUnitConfig *units, const deoglSkinTexture &skinTexture,
deoglSkinState *skinState, deoglRDynamicSkin *dynamicSkin ){
	if( ! units ){
		DETHROW( deeInvalidParam );
	}
	
	if( pUsedTextureTargetCount == 0 ){
		return;
	}
	
	// texture properties
	if( pTextureTargets[ ettColor ] != -1 ){
		if( pConfig.GetVariations() ){
			units[ pTextureTargets[ ettColor ] ].EnableArrayTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectColor, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetColorArray() );
			
		}else{
			units[ pTextureTargets[ ettColor ] ].EnableTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectColor, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetColor() );
		}
	}
	
	if( pTextureTargets[ ettColorTintMask ] != -1 ){
		if( pConfig.GetVariations() ){
			units[ pTextureTargets[ ettColorTintMask ] ].EnableArrayTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectColorTintMask, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetSolidityArray() );
			
		}else{
			units[ pTextureTargets[ ettColorTintMask ] ].EnableTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectColorTintMask, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetSolidity() );
		}
	}
	
	if( pTextureTargets[ ettTransparency ] != -1 ){
		if( pConfig.GetVariations() ){
			units[ pTextureTargets[ ettTransparency ] ].EnableArrayTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectTransparency, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetTransparencyArray() );
			
		}else{
			units[ pTextureTargets[ ettTransparency ] ].EnableTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectTransparency, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetTransparency() );
		}
	}
	
	if( pTextureTargets[ ettSolidity ] != -1 ){
		if( pConfig.GetVariations() ){
			units[ pTextureTargets[ ettSolidity ] ].EnableArrayTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectSolidity, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetSolidityArray() );
			
		}else{
			units[ pTextureTargets[ ettSolidity ] ].EnableTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectSolidity, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetSolidity() );
		}
	}
	
	if( pTextureTargets[ ettNormal ] != -1 ){
		if( pConfig.GetVariations() ){
			units[ pTextureTargets[ ettNormal ] ].EnableArrayTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectNormal, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetNormalArray() );
			
		}else{
			units[ pTextureTargets[ ettNormal ] ].EnableTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectNormal, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetNormal() );
		}
	}
	
	if( pTextureTargets[ ettHeight ] != -1 ){
		if( pConfig.GetVariations() ){
			units[ pTextureTargets[ ettHeight ] ].EnableArrayTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectHeight, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetHeightArray() );
			
		}else{
			units[ pTextureTargets[ ettHeight ] ].EnableTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectHeight, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetHeight() );
		}
	}
	
	if( pTextureTargets[ ettReflectivity ] != -1 ){
		if( pConfig.GetVariations() ){
			units[ pTextureTargets[ ettReflectivity ] ].EnableArrayTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectReflectivity, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetReflectivityArray() );
			
		}else{
			units[ pTextureTargets[ ettReflectivity ] ].EnableTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectReflectivity, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetReflectivity() );
		}
	}
	
	if( pTextureTargets[ ettRoughness ] != -1 ){
		if( pConfig.GetVariations() ){
			units[ pTextureTargets[ ettRoughness ] ].EnableArrayTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectRoughness, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetRoughnessArray() );
			
		}else{
			units[ pTextureTargets[ ettRoughness ] ].EnableTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectRoughness, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetRoughness() );
		}
	}
	
	if( pTextureTargets[ ettEnvMap ] != -1 ){
		units[ pTextureTargets[ ettEnvMap ] ].EnableCubeMapFromChannel( pRenderThread,
			skinTexture, deoglSkinChannel::ectEnvironmentMap, skinState, dynamicSkin, NULL );
		
		if( pTextureTargets[ ettEnvMapFade ] != -1 ){
			units[ pTextureTargets[ ettEnvMapFade ] ].SetFrom( units[ pTextureTargets[ ettEnvMap ] ] );
		}
	}
	
	if( pTextureTargets[ ettEmissivity ] != -1 ){
		if( pConfig.GetVariations() ){
			units[ pTextureTargets[ ettEmissivity ] ].EnableArrayTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectEmissivity, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetEmissivityArray() );
			
		}else{
			units[ pTextureTargets[ ettEmissivity ] ].EnableTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectEmissivity, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetEmissivity() );
		}
	}
	
	if( pTextureTargets[ ettRenderColor ] != -1 ){
		units[ pTextureTargets[ ettRenderColor ] ].EnableSpecial( deoglTexUnitConfig::estPrevColor,
			pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampNearest ) );
	}
	
	if( pTextureTargets[ ettRefractionDistort ] != -1 ){
		if( pConfig.GetVariations() ){
			units[ pTextureTargets[ ettRefractionDistort ] ].EnableArrayTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectRefractDistort, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetNormalArray() );
			
		}else{
			units[ pTextureTargets[ ettRefractionDistort ] ].EnableTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectRefractDistort, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetNormal() );
		}
	}
	
	if( pTextureTargets[ ettAO ] != -1 ){
		if( pConfig.GetVariations() ){
			units[ pTextureTargets[ ettAO ] ].EnableArrayTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectAO, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetAOArray() );
			
		}else{
			units[ pTextureTargets[ ettAO ] ].EnableTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectAO, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetAO() );
		}
	}
	
	if( pTextureTargets[ ettEnvRoom ] != -1 ){
		units[ pTextureTargets[ ettEnvRoom ] ].EnableCubeMapFromChannel( pRenderThread,
			skinTexture, deoglSkinChannel::ectEnvironmentRoom, skinState, dynamicSkin,
			pRenderThread.GetDefaultTextures().GetEnvMap() );
	}
	
	if( pTextureTargets[ ettEnvRoomMask ] != -1 ){
		if( pConfig.GetVariations() ){
			units[ pTextureTargets[ ettEnvRoomMask ] ].EnableArrayTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectEnvironmentRoomMask, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetAOArray() );
			
		}else{
			units[ pTextureTargets[ ettEnvRoomMask ] ].EnableTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectEnvironmentRoomMask, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetAO() );
		}
	}
	
	if( pTextureTargets[ ettEnvRoomEmissivity ] != -1 ){
		units[ pTextureTargets[ ettEnvRoomEmissivity ] ].EnableCubeMapFromChannel( pRenderThread,
			skinTexture, deoglSkinChannel::ectEnvironmentRoomEmissivity, skinState, dynamicSkin,
			pRenderThread.GetDefaultTextures().GetEnvMap() );
	}
	
	if( pTextureTargets[ ettAbsorption ] != -1 ){
		if( pConfig.GetVariations() ){
			units[ pTextureTargets[ ettAbsorption ] ].EnableArrayTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectAbsorption, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetEnvRoomMaskArray() );
			
		}else{
			units[ pTextureTargets[ ettAbsorption ] ].EnableTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectAbsorption, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetEnvRoomMask() );
		}
	}
	
	if( pTextureTargets[ ettRimEmissivity ] != -1 ){
		if( pConfig.GetVariations() ){
			units[ pTextureTargets[ ettRimEmissivity ] ].EnableArrayTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectRimEmissivity, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetEmissivityArray() );
			
		}else{
			units[ pTextureTargets[ ettRimEmissivity ] ].EnableTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectRimEmissivity, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetEmissivity() );
		}
	}
	
	if( pTextureTargets[ ettNonPbrAlbedo ] != -1 ){
		if( pConfig.GetVariations() ){
			units[ pTextureTargets[ ettNonPbrAlbedo ] ].EnableArrayTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectNonPbrAlbedo, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetColorArray() );
			
		}else{
			units[ pTextureTargets[ ettNonPbrAlbedo ] ].EnableTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectNonPbrAlbedo, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetColor() );
		}
	}
	
	if( pTextureTargets[ ettNonPbrMetalness ] != -1 ){
		if( pConfig.GetVariations() ){
			units[ pTextureTargets[ ettNonPbrMetalness ] ].EnableArrayTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectNonPbrMetalness, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetNonPbrMetalnessArray() );
			
		}else{
			units[ pTextureTargets[ ettNonPbrMetalness ] ].EnableTextureFromChannel( pRenderThread,
				skinTexture, deoglSkinChannel::ectNonPbrMetalness, skinState, dynamicSkin,
				pRenderThread.GetDefaultTextures().GetNonPbrMetalness() );
		}
	}
	
	// depth buffers
	if( pTextureTargets[ ettDepth ] != -1 ){
		units[ pTextureTargets[ ettDepth ] ].EnableSpecial( deoglTexUnitConfig::estPrevDepth,
			pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampNearest ) );
	}
	
	if( pTextureTargets[ ettDepthTest ] != -1 ){
		units[ pTextureTargets[ ettDepthTest ] ].EnableSpecial( deoglTexUnitConfig::estPrevDepth,
			pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampNearest ) );
	}
	
	if( pTextureTargets[ ettXRayDepth ] != -1 ){
		units[ pTextureTargets[ ettXRayDepth ] ].EnableSpecial( deoglTexUnitConfig::estXRayDepth,
			pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampNearest ) );
	}
}

int deoglSkinShader::REFLECTION_TEST_MODE = 2; // 0=oldVersion 1=ownPassReflection 2=singleBlenderEnvMap

void deoglSkinShader::SetTUCPerObjectEnvMap( deoglTexUnitConfig *units,
deoglEnvironmentMap *envmapSky, deoglEnvironmentMap *envmap, deoglEnvironmentMap *envmapFade ){
	if( ! units ){
		DETHROW( deeInvalidParam );
	}
	
	if( pUsedTextureTargetCount == 0 ){
		return;
	}
	
	if( pTextureTargets[ ettEnvMap ] == -1 ){
		return;
	}
	if( ! units[ pTextureTargets[ ettEnvMap ] ].IsDisabled() ){
		return;
	}
	
	const bool useEquiMap = pConfig.GetTextureEnvMapEqui();
	deoglTexUnitConfig &unitEnvMap = units[ pTextureTargets[ ettEnvMap ] ];
	
	if( REFLECTION_TEST_MODE == 0 ){
		// use provided env-map (see below)
		
	}else if( REFLECTION_TEST_MODE == 2 ){
		if( pRenderThread.GetConfiguration().GetEnvMapMethod() == deoglConfiguration::eemmSingle ){
			if( useEquiMap ){
				unitEnvMap.EnableTexture( pRenderThread.GetRenderers().GetReflection().GetEnvMapEqui(),
					pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscRepeatLinearMipMap ) );
				
			}else{
				unitEnvMap.EnableCubeMap( pRenderThread.GetRenderers().GetReflection().GetEnvMap(),
					pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampLinearMipMap ) ); // nearest
			}
			
			// fade
			if( pTextureTargets[ ettEnvMapFade ] == -1 ){
				return;
			}
			
			deoglTexUnitConfig &unitEnvMapFade = units[ pTextureTargets[ ettEnvMapFade ] ];
			
			if( useEquiMap ){
				unitEnvMapFade.EnableTexture( pRenderThread.GetRenderers().GetReflection().GetEnvMapEqui(),
					pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscRepeatLinearMipMap ) );
				
			}else{
				unitEnvMapFade.EnableCubeMap( pRenderThread.GetRenderers().GetReflection().GetEnvMap(),
					pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampLinearMipMap ) ); // nearest
			}
			
			return;
			
		}else{
			if( useEquiMap ){
				unitEnvMap.EnableSpecial( deoglTexUnitConfig::estDirectEnvMapActive,
					pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscRepeatLinearMipMap ) );
				
			}else{
				unitEnvMap.EnableSpecial( deoglTexUnitConfig::estDirectEnvMapActive,
					pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscRepeatLinearMipMap ) ); // nearest
			}
			
			// fade
			if( pTextureTargets[ ettEnvMapFade ] == -1 ){
				return;
			}
			
			deoglTexUnitConfig &unitEnvMapFade = units[ pTextureTargets[ ettEnvMapFade ] ];
			
			if( useEquiMap ){
				unitEnvMapFade.EnableSpecial( deoglTexUnitConfig::estDirectEnvMapFading,
					pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscRepeatLinearMipMap ) );
				
			}else{
				unitEnvMapFade.EnableSpecial( deoglTexUnitConfig::estDirectEnvMapFading,
					pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscRepeatLinearMipMap ) ); // nearest
			}
			
			return;
		}
	}
	
	// in all other cases use the provided env map
	if( ! envmap || ( ! envmap->GetEnvironmentMap() && ! envmap->GetEquiEnvMap() ) ){
		envmap = envmapSky;
	}
	
	if( envmap ){
		if( useEquiMap ){
			unitEnvMap.EnableTexture( envmap->GetEquiEnvMap(),
				pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscRepeatLinearMipMap ) );
			
		}else{
			unitEnvMap.EnableCubeMap( envmap->GetEnvironmentMap(),
				pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampLinearMipMap ) );
		}
		
	}else{
		if( useEquiMap ){
			unitEnvMap.EnableTexture( pRenderThread.GetDefaultTextures().GetColor(),
				pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscRepeatNearest ) );
			
		}else{
			unitEnvMap.EnableCubeMap( pRenderThread.GetDefaultTextures().GetEnvMap(),
				pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampNearest ) );
		}
	}
	
	// fade
	if( pTextureTargets[ ettEnvMapFade ] == -1 ){
		return;
	}
	
	deoglTexUnitConfig &unitEnvMapFade = units[ pTextureTargets[ ettEnvMapFade ] ];
	
	if( ! envmapFade || ( ! envmapFade->GetEnvironmentMap() && ! envmapFade->GetEquiEnvMap() ) ){
		envmapFade = envmap;
	}
	
	if( envmapFade ){
		if( useEquiMap ){
			unitEnvMapFade.EnableTexture( envmapFade->GetEquiEnvMap(),
				pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscRepeatLinearMipMap ) );
			
		}else{
			unitEnvMapFade.EnableCubeMap( envmapFade->GetEnvironmentMap(),
				pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampLinearMipMap ) );
		}
		
	}else{
		if( useEquiMap ){
			unitEnvMapFade.EnableTexture( pRenderThread.GetDefaultTextures().GetColor(),
				pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscRepeatNearest ) );
			
		}else{
			unitEnvMapFade.EnableCubeMap( pRenderThread.GetDefaultTextures().GetEnvMap(),
				pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampNearest ) );
		}
	}
}



// Shader Generation
//////////////////////

void deoglSkinShader::GenerateShader(){
	//deoglSkinShaderManager &ssmgr = pRenderThread.GetShader().GetSkinShaderManager();
	deoglShaderManager &smgr = pRenderThread.GetShader().GetShaderManager();
	deoglShaderDefines defines;
	
	if( pShader ){
		delete pShader;
		pShader = NULL;
	}
	if( pSources ){
		delete pSources;
		pSources = NULL;
	}
	
	try{
		pSources = new deoglShaderSources;
		
		pSources->SetVersion( "150" );
		
		GenerateDefines( defines );
		GenerateVertexSC();
		GenerateGeometrySC();
		GenerateFragmentSC();
		GenerateTessellationControlSC();
		GenerateTessellationEvaluationSC();
		
		UpdateTextureTargets();
		UpdateUniformTargets();
		
		InitShaderParameters();
		
		// create shader
		pShader = new deoglShaderProgram( pRenderThread, pSources, defines );
		
		// add unit source codes from source files
		const decString &pathVSC = pSources->GetPathVertexSourceCode();
		if( ! pathVSC.IsEmpty() ){
			pShader->SetVertexSourceCode( smgr.GetUnitSourceCodeWithPath( pathVSC ) );
			
			if( ! pShader->GetVertexSourceCode() ){
				pRenderThread.GetLogger().LogErrorFormat( "Skin Shader: Vertex source '%s' not found", pathVSC.GetString() );
				DETHROW( deeInvalidParam );
			}
		}
		
		const decString &pathGSC = pSources->GetPathGeometrySourceCode();
		if( ! pathGSC.IsEmpty() ){
			pShader->SetGeometrySourceCode( smgr.GetUnitSourceCodeWithPath( pathGSC ) );
			
			if( ! pShader->GetGeometrySourceCode() ){
				pRenderThread.GetLogger().LogErrorFormat( "Skin Shader: Geometry source '%s' not found", pathGSC.GetString() );
				DETHROW( deeInvalidParam );
			}
		}
		
		const decString &pathFSC = pSources->GetPathFragmentSourceCode();
		if( ! pathFSC.IsEmpty() ){
			pShader->SetFragmentSourceCode( smgr.GetUnitSourceCodeWithPath( pathFSC ) );
			
			if( ! pShader->GetFragmentSourceCode() ){
				pRenderThread.GetLogger().LogErrorFormat( "Skin Shader: Fragment source '%s' not found", pathFSC.GetString() );
				DETHROW( deeInvalidParam );
			}
		}
		
		const decString &pathTCSC = pSources->GetPathTessellationControlSourceCode();
		if( ! pathTCSC.IsEmpty() ){
			pShader->SetTessellationControlSourceCode( smgr.GetUnitSourceCodeWithPath( pathTCSC ) );
			
			if( ! pShader->GetTessellationControlSourceCode() ){
				pRenderThread.GetLogger().LogErrorFormat( "Skin Shader: Tessellation control source '%s' not found", pathTCSC.GetString() );
				DETHROW( deeInvalidParam );
			}
		}
		
		const decString &pathTESC = pSources->GetPathTessellationEvaluationSourceCode();
		if( ! pathTESC.IsEmpty() ){
			pShader->SetTessellationEvaluationSourceCode( smgr.GetUnitSourceCodeWithPath( pathTESC ) );
			
			if( ! pShader->GetTessellationEvaluationSourceCode() ){
				pRenderThread.GetLogger().LogErrorFormat( "Skin Shader: Tessellation evaluation source '%s' not found", pathTESC.GetString() );
				DETHROW( deeInvalidParam );
			}
		}
		
		// compile shader
		pShader->SetCompiled( pRenderThread.GetShader().GetShaderManager().GetLanguage()->CompileShader( *pShader ) );
		/*
		if( pConfig.GetShaderMode() == deoglSkinShaderConfig::esmGeometry ){
			const int count = pSources->GetParameterCount();
			int i;
			
			pRenderThread.GetLogger().LogInfo( "Skin Shader: Debug Parameters" );
			for( i=0; i<count; i++ ){
				pRenderThread.GetLogger().LogInfoFormat( "- parameter %i (%s) = %i", i, pSources->GetParameterAt( i ), pShader->GetCompiled()->GetParameterAt( i ) );
			}
		}
		*/
		
	}catch( const deException &e ){
		pRenderThread.GetLogger().LogException( e );
		decString text;
		pConfig.DebugGetConfigString( text );
		pRenderThread.GetLogger().LogError( text.GetString() );
		throw;
	}
}

void deoglSkinShader::GenerateDefines( deoglShaderDefines &defines ){
	pRenderThread.GetShader().SetCommonDefines( defines );
	
	// tessellation
	if( pConfig.GetTessellationMode() != deoglSkinShaderConfig::etmNone ){
		defines.SetDefines( "HAS_TESSELLATION_SHADER" );
		
		if( pConfig.GetTessellationMode() == deoglSkinShaderConfig::etmLinear ){
			defines.SetDefines( "TESSELLATION_LINEAR" );
		}
	}
	
	// geometry type definitions
	if( pConfig.GetGeometryMode() == deoglSkinShaderConfig::egmPropField ){
		defines.SetDefines( "PROP_FIELD" );
		
	}else if( pConfig.GetGeometryMode() == deoglSkinShaderConfig::egmHeightMap ){
		defines.SetDefines( "HEIGHT_MAP" );
		
	}else if( pConfig.GetGeometryMode() == deoglSkinShaderConfig::egmParticle ){
		defines.SetDefines( "PARTICLE" );
		
		if( pConfig.GetParticleMode() == deoglSkinShaderConfig::epmRibbon ){
			defines.SetDefines( "PARTICLE_RIBBON" );
			
		}else if( pConfig.GetParticleMode() == deoglSkinShaderConfig::epmBeam ){
			defines.SetDefines( "PARTICLE_BEAM" );
		}
		
	}else if( pConfig.GetGeometryMode() == deoglSkinShaderConfig::egmDecal ){
		defines.SetDefines( "DECAL" );
	}
	
	// depth definitions
	if( pConfig.GetDepthMode() == deoglSkinShaderConfig::edmOrthogonal ){
		defines.SetDefines( "DEPTH_ORTHOGONAL" );
		defines.SetDefines( "DEPTH_OFFSET" );
		
	}else if( pConfig.GetDepthMode() == deoglSkinShaderConfig::edmDistance ){
		defines.SetDefines( "DEPTH_DISTANCE" );
		defines.SetDefines( "DEPTH_OFFSET" );
	}
	
	// shading configuration definitions
	switch( pConfig.GetMaterialNormalModeDec() ){
	case deoglSkinShaderConfig::emnmIntBasic:
		defines.SetDefines( "MATERIAL_NORMAL_DEC_INTBASIC" );
		break;
		
	case deoglSkinShaderConfig::emnmSpheremap:
		defines.SetDefines( "MATERIAL_NORMAL_DEC_SPHEREMAP" );
		break;
		
	default:
		break;
	}
	
	switch( pConfig.GetMaterialNormalModeEnc() ){
	case deoglSkinShaderConfig::emnmIntBasic:
		defines.SetDefines( "MATERIAL_NORMAL_ENC_INTBASIC" );
		break;
		
	case deoglSkinShaderConfig::emnmSpheremap:
		defines.SetDefines( "MATERIAL_NORMAL_ENC_SPHEREMAP" );
		break;
		
	default:
		break;
	}
	
	if( pConfig.GetXRay() ){
		defines.SetDefines( "WITH_XRAY" );
	}
	
	// texture usage definitions
	if( pConfig.GetTextureColor() ){
		defines.SetDefines( "TEXTURE_COLOR" );
	}
	if( pConfig.GetTextureColorTintMask() ){
		defines.SetDefines( "TEXTURE_COLOR_TINT_MASK" );
	}
	if( pConfig.GetTextureTransparency() ){
		defines.SetDefines( "TEXTURE_TRANSPARENCY" );
	}
	if( pConfig.GetTextureSolidity() ){
		defines.SetDefines( "TEXTURE_SOLIDITY" );
	}
	if( pConfig.GetTextureNormal() ){
		defines.SetDefines( "TEXTURE_NORMAL" );
	}
	if( pConfig.GetTextureHeight() ){
		defines.SetDefines( "TEXTURE_HEIGHT" );
	}
	if( pConfig.GetTextureReflectivity() ){
		defines.SetDefines( "TEXTURE_REFLECTIVITY" );
	}
	if( pConfig.GetTextureRoughness() ){
		defines.SetDefines( "TEXTURE_ROUGHNESS" );
	}
	if( pConfig.GetTextureEnvMap() ){
		defines.SetDefines( "TEXTURE_ENVMAP" );
		
		if( pConfig.GetTextureEnvMapEqui() ){
			defines.SetDefines( "TEXTURE_ENVMAP_EQUI" );
		}
		
		if( REFLECTION_TEST_MODE < 2 ){
			defines.SetDefines( "TEXTURE_ENVMAP_FADE" );
		}
	}
	if( pConfig.GetTextureEmissivity() ){
		defines.SetDefines( "TEXTURE_EMISSIVITY" );
	}
	if( pConfig.GetTextureAbsorption() ){
		defines.SetDefines( "TEXTURE_ABSORPTION" );
	}
	if( pConfig.GetTextureRenderColor() ){
		defines.SetDefines( "TEXTURE_RENDERCOLOR" );
	}
	if( pConfig.GetTextureRefractionDistort() ){
		defines.SetDefines( "TEXTURE_REFRACTION_DISTORT" );
	}
	if( pConfig.GetTextureAO() ){
		defines.SetDefines( "TEXTURE_AO" );
	}
	if( pConfig.GetTextureEnvRoom() ){
		defines.SetDefines( "TEXTURE_ENVROOM" );
	}
	if( pConfig.GetTextureEnvRoomMask() ){
		defines.SetDefines( "TEXTURE_ENVROOM_MASK" );
	}
	if( pConfig.GetTextureEnvRoomEmissivity() ){
		defines.SetDefines( "TEXTURE_ENVROOM_EMISSIVITY" );
	}
	if( pConfig.GetTextureRimEmissivity() ){
		defines.SetDefines( "TEXTURE_RIM_EMISSIVITY" );
	}
	if( pConfig.GetTextureNonPbrAlbedo() ){
		defines.SetDefines( "TEXTURE_NONPBR_ALBEDO" );
	}
	if( pConfig.GetTextureNonPbrMetalness() ){
		defines.SetDefines( "TEXTURE_NONPBR_METALNESS" );
	}
	
	// shading definitions
	if( pConfig.GetMaskedSolidity() ){
		defines.SetDefines( "MASKED_SOLIDITY" );
	}
	
	switch( pConfig.GetDepthTestMode() ){
	case deoglSkinShaderConfig::edtmLarger:
		defines.SetDefines( "DEPTH_TEST", "DEPTH_TEST_LARGER" );
		break;
		
	case deoglSkinShaderConfig::edtmSmaller:
		defines.SetDefines( "DEPTH_TEST", "DEPTH_TEST_SMALLER" );
		break;
		
	default:
		break;
	}
	
	if( pConfig.GetClipPlane() ){
		defines.SetDefines( "CLIP_PLANE" );
	}
	if( pConfig.GetNoZClip() ){
		defines.SetDefines( "NO_ZCLIP" );
	}
	if( pConfig.GetDecodeInDepth() ){
		defines.SetDefines( "DECODE_IN_DEPTH" );
	}
	if( pConfig.GetEncodeOutDepth() ){
		defines.SetDefines( "ENCODE_OUT_DEPTH" );
	}
	if( pConfig.GetInverseDepth() ){
		defines.SetDefines( "INVERSE_DEPTH" );
	}
	
	if( pConfig.GetGSRenderCube() ){
		if( ! pRenderThread.GetExtensions().SupportsGeometryShader() ){
			DETHROW( deeInvalidParam );
		}
		
		defines.SetDefines( "GS_RENDER_CUBE", "GS_RENDER_CUBE_CULLING" );
		
	}else if( pConfig.GetGSRenderCascaded() ){
		if( ! pRenderThread.GetExtensions().SupportsGeometryShader() ){
			DETHROW( deeInvalidParam );
		}
		
		defines.SetDefines( "GS_RENDER_CASCADED" );
		
	}else if( pConfig.GetGSRenderStereo() ){
		if( ! pRenderThread.GetExtensions().SupportsGeometryShader() ){
			DETHROW( deeInvalidParam );
		}
		
		defines.SetDefines( "GS_RENDER_STEREO" );
		
	}else if( pConfig.GetVSRenderStereo() ){
		if( ! pRenderThread.GetChoices().GetRenderStereoVSLayer() ){
			DETHROW( deeInvalidParam );
		}
		
		defines.SetDefines( "VS_RENDER_STEREO" );
	}
	
	// shared parameter blocks
	const deoglRTBufferObject &bo = pRenderThread.GetBufferObject();
	
	if( pRenderThread.GetChoices().GetSharedSPBUseSSBO() ){
		defines.SetDefines( "SHARED_SPB_USE_SSBO" );
		
		if( bo.GetLayoutSkinInstanceSSBO()->GetOffsetPadding() >= 16 ){
			defines.SetDefine( "SHARED_SPB_PADDING", bo.GetLayoutSkinInstanceSSBO()->GetOffsetPadding() / 16 );
		}
		
		if( bo.GetLayoutSkinTextureSSBO()->GetOffsetPadding() >= 16 ){
			defines.SetDefine( "SHARED_SPB_TEXTURE_PADDING", bo.GetLayoutSkinTextureSSBO()->GetOffsetPadding() / 16 );
		}
		
	}else{
		// NOTE UBO requires array size to be constant, SSBO does not
		if( bo.GetLayoutSkinInstanceUBO()->GetElementCount() > 0 ){
			defines.SetDefine( "SHARED_SPB_ARRAY_SIZE", bo.GetLayoutSkinInstanceUBO()->GetElementCount() );
		}
		if( bo.GetLayoutSkinTextureUBO()->GetElementCount() > 0 ){
			defines.SetDefine( "SHARED_SPB_TEXTURE_ARRAY_SIZE", bo.GetLayoutSkinTextureUBO()->GetElementCount() );
		}
		
		if( bo.GetLayoutSkinInstanceUBO()->GetOffsetPadding() >= 16 ){
			defines.SetDefine( "SHARED_SPB_PADDING", bo.GetLayoutSkinInstanceUBO()->GetOffsetPadding() / 16 );
		}
		if( bo.GetLayoutSkinTextureUBO()->GetOffsetPadding() >= 16 ){
			defines.SetDefine( "SHARED_SPB_TEXTURE_PADDING", bo.GetLayoutSkinTextureUBO()->GetOffsetPadding() / 16 );
		}
	}
	
	if( pConfig.GetSharedSPB() ){ // affects only instance parameters
		defines.SetDefines( "SHARED_SPB" );
		if( bo.GetInstanceArraySizeUBO() > 0 ){
			defines.SetDefine( "SPB_INSTANCE_ARRAY_SIZE", bo.GetInstanceArraySizeUBO() );
		}
	}
	
	// output definitions
	if( pRenderThread.GetCapabilities().GetMaxDrawBuffers() < 8 ){
		defines.SetDefines( "OUTPUT_LIMITBUFFERS" );
	}
	
	if( pConfig.GetOutputConstant() ){
		defines.SetDefines( "OUTPUT_CONSTANT" );
		
	}else if( pConfig.GetOutputColor() ){
		defines.SetDefines( "OUTPUT_COLOR" );
	}
	if( ! pConfig.GetLuminanceOnly() ){
		if( pConfig.GetGeometryMode() != deoglSkinShaderConfig::egmParticle
		|| GetRenderThread().GetChoices().GetRealTransparentParticles() ){
			defines.SetDefines( "OUTPUT_MATERIAL_PROPERTIES" );
		}
	}
	
	// texture property usage definitions
	defines.SetDefines( "TP_NORMAL_STRENGTH", "TP_ROUGHNESS_REMAP" );
		// ^== needs an option to select if this is required or not
	
	if( pConfig.GetUseNormalRoughnessCorrection() ){
		defines.SetDefines( "USE_NORMAL_ROUGHNESS_CORRECTION" );
	}
	
	if( pConfig.GetAmbientLightProbe() ){
		defines.SetDefines( "AMBIENT_LIGHT_PROBE" );
	}
	
	if( pConfig.GetBillboard() ){
		defines.SetDefines( "BILLBOARD" );
	}
	
	if( pConfig.GetSkinReflections() ){
		defines.SetDefines( "SKIN_REFLECTIONS" );
	}
	
	if( pConfig.GetFadeOutRange() ){
		defines.SetDefines( "FADEOUT_RANGE" );
	}
	
	if( pConfig.GetVariations() ){
		defines.SetDefines( "WITH_VARIATIONS" );
	}
	
	if( pConfig.GetOutline() ){
		defines.SetDefines( "WITH_OUTLINE" );
	}
	if( pConfig.GetOutlineThicknessScreen() ){
		defines.SetDefines( "WITH_OUTLINE_THICKNESS_SCREEN" );
	}
	if( pConfig.GetLuminanceOnly() ){
		defines.SetDefines( "LUMINANCE_ONLY" );
	}
	
	// dynamic texture property usage definitions
	if( pConfig.GetDynamicColorTint() ){
		defines.SetDefines( "DYNAMIC_COLOR_TINT" );
	}
	if( pConfig.GetDynamicColorGamma() ){
		defines.SetDefines( "DYNAMIC_COLOR_GAMMA" );
	}
	if( pConfig.GetDynamicColorSolidityMultiplier() ){
		defines.SetDefines( "DYNAMIC_COLOR_SOLIDITY_MULTIPLIER" );
	}
	if( pConfig.GetDynamicAmbientOcclusionSolidityMultiplier() ){
		defines.SetDefines( "DYNAMIC_AO_SOLIDITY_MULTIPLIER" );
	}
	if( pConfig.GetDynamicTransparencyMultiplier() ){
		defines.SetDefines( "DYNAMIC_TRANSPARENCY_MULTIPLIER" );
	}
	if( pConfig.GetDynamicSolidityMultiplier() ){
		defines.SetDefines( "DYNAMIC_SOLIDITY_MULTIPLIER" );
	}
	if( pConfig.GetDynamicHeightRemap() ){
		defines.SetDefines( "DYNAMIC_HEIGHT_REMAP" );
	}
	if( pConfig.GetDynamicNormalStrength() ){
		defines.SetDefines( "DYNAMIC_NORMAL_STRENGTH" );
	}
	if( pConfig.GetDynamicNormalSolidityMultiplier() ){
		defines.SetDefines( "DYNAMIC_NORMAL_SOLIDITY_MULTIPLIER" );
	}
	if( pConfig.GetDynamicRoughnessRemap() ){
		defines.SetDefines( "DYNAMIC_ROUGHNESS_REMAP" );
	}
	if( pConfig.GetDynamicRoughnessGamma() ){
		defines.SetDefines( "DYNAMIC_ROUGHNESS_GAMMA" );
	}
	if( pConfig.GetDynamicRoughnessSolidityMultiplier() ){
		defines.SetDefines( "DYNAMIC_ROUGHNESS_SOLIDITY_MULTIPLIER" );
	}
	if( pConfig.GetDynamicReflectivitySolidityMultiplier() ){
		defines.SetDefines( "DYNAMIC_REFLECTIVITY_SOLIDITY_MULTIPLIER" );
	}
	if( pConfig.GetDynamicReflectivityMultiplier() ){
		defines.SetDefines( "DYNAMIC_REFLECTIVITY_MULTIPLIER" );
	}
	if( pConfig.GetDynamicRefractionDistortStrength() ){
		defines.SetDefines( "DYNAMIC_REFRACTION_DISTORT_STRENGTH" );
	}
	if( pConfig.GetDynamicEmissivityIntensity() || pConfig.GetDynamicEmissivityTint() ){
		defines.SetDefines( "DYNAMIC_EMISSIVITY_INTENSITY" );
	}
	if( pConfig.GetDynamicEnvRoomSize() ){
		defines.SetDefines( "DYNAMIC_ENVROOM_SIZE" );
	}
	if( pConfig.GetDynamicEnvRoomOffset() ){
		defines.SetDefines( "DYNAMIC_ENVROOM_OFFSET" );
	}
	if( pConfig.GetDynamicEnvRoomEmissivityIntensity() || pConfig.GetDynamicEnvRoomEmissivityTint() ){
		defines.SetDefines( "DYNAMIC_ENVROOM_EMISSIVITY_INTENSITY" );
	}
	if( pConfig.GetDynamicVariation() ){
		defines.SetDefines( "DYNAMIC_VARIATION" );
	}
	if( pConfig.GetDynamicRimEmissivityIntensity() || pConfig.GetDynamicRimEmissivityTint() ){
		defines.SetDefines( "DYNAMIC_RIM_EMISSIVITY_INTENSITY" );
	}
	if( pConfig.GetDynamicRimAngle() ){
		defines.SetDefines( "DYNAMIC_RIM_ANGLE" );
	}
	if( pConfig.GetDynamicRimExponent() ){
		defines.SetDefines( "DYNAMIC_RIM_EXPONENT" );
	}
	if( pConfig.GetDynamicOutlineColor() ){
		defines.SetDefines( "DYNAMIC_OUTLINE_COLOR" );
	}
	if( pConfig.GetDynamicOutlineColorTint() ){
		defines.SetDefines( "DYNAMIC_OUTLINE_COLOR_TINT" );
	}
	if( pConfig.GetDynamicOutlineThickness() ){
		defines.SetDefines( "DYNAMIC_OUTLINE_THICKNESS" );
	}
	if( pConfig.GetDynamicOutlineSolidity() ){
		defines.SetDefines( "DYNAMIC_OUTLINE_SOLIDITY" );
	}
	if( pConfig.GetDynamicOutlineEmissivity() ){
		defines.SetDefines( "DYNAMIC_OUTLINE_EMISSIVITY" );
	}
	if( pConfig.GetDynamicOutlineEmissivityTint() ){
		defines.SetDefines( "DYNAMIC_OUTLINE_EMISSIVITY_TINT" );
	}
}

void deoglSkinShader::GenerateVertexSC(){
	deoglSkinShaderManager::eUnitSourceCodePath unitSourceCodePath;
	
	switch( pConfig.GetGeometryMode() ){
	case deoglSkinShaderConfig::egmParticle:
		unitSourceCodePath = deoglSkinShaderManager::euscpVertexParticle;
		break;
		
	default:
		switch( pConfig.GetShaderMode() ){
		case deoglSkinShaderConfig::esmDepth:
			unitSourceCodePath = deoglSkinShaderManager::euscpVertexDepth;
			break;
			
		default:
			if( pConfig.GetGIMaterial() ){
				unitSourceCodePath = deoglSkinShaderManager::euscpVertexGIMaterialMap;
				
			}else{
				unitSourceCodePath = deoglSkinShaderManager::euscpVertexGeometry;
			}
		}
	}
	
	pSources->SetPathVertexSourceCode( pRenderThread.GetShader().
		GetSkinShaderManager().GetUnitSourceCodePath( unitSourceCodePath ) );
}

void deoglSkinShader::GenerateGeometrySC(){
	deoglSkinShaderManager::eUnitSourceCodePath unitSourceCodePath;
	
	switch( pConfig.GetGeometryMode() ){
	case deoglSkinShaderConfig::egmParticle:
		switch( pConfig.GetParticleMode() ){
		case deoglSkinShaderConfig::epmRibbon:
			unitSourceCodePath = deoglSkinShaderManager::euscpGeometryParticleRibbon;
			break;
		
		case deoglSkinShaderConfig::epmBeam:
			unitSourceCodePath = deoglSkinShaderManager::euscpGeometryParticleRibbon;
			break;
			
		case deoglSkinShaderConfig::epmParticle:
			unitSourceCodePath = deoglSkinShaderManager::euscpGeometryParticle;
			break;
			
		default:
			return; // keep compiler happy
		}
		break;
		
	default:
		if( pConfig.GetGSRenderCube() || pConfig.GetGSRenderCascaded() || pConfig.GetGSRenderStereo() ){
			switch( pConfig.GetShaderMode() ){
			case deoglSkinShaderConfig::esmDepth:
				unitSourceCodePath = deoglSkinShaderManager::euscpGeometryDepth;
				break;
				
			default:
				unitSourceCodePath = deoglSkinShaderManager::euscpGeometryGeometry;
			}
			
		}else{
			return;
		}
		break;
	}
	
	pSources->SetPathGeometrySourceCode( pRenderThread.GetShader().
		GetSkinShaderManager().GetUnitSourceCodePath( unitSourceCodePath ) );
}

void deoglSkinShader::GenerateFragmentSC(){
	deoglSkinShaderManager::eUnitSourceCodePath unitSourceCodePath;
	
	switch( pConfig.GetShaderMode() ){
	case deoglSkinShaderConfig::esmDepth:
		unitSourceCodePath = deoglSkinShaderManager::euscpFragmentDepth;
		break;
		
	default:
		if( pConfig.GetGIMaterial() ){
			unitSourceCodePath = deoglSkinShaderManager::euscpFragmentGIMaterialMap;
			
		}else{
			unitSourceCodePath = deoglSkinShaderManager::euscpFragmentGeometry;
		}
	}
	
	pSources->SetPathFragmentSourceCode( pRenderThread.GetShader().
		GetSkinShaderManager().GetUnitSourceCodePath( unitSourceCodePath ) );
}

void deoglSkinShader::GenerateTessellationControlSC(){
	deoglSkinShaderManager::eUnitSourceCodePath unitSourceCodePath;
	
	switch( pConfig.GetTessellationMode() ){
	case deoglSkinShaderConfig::etmLinear:
		switch( pConfig.GetShaderMode() ){
		case deoglSkinShaderConfig::esmDepth:
			unitSourceCodePath = deoglSkinShaderManager::euscpTessControlDepth;
			break;
			
		default:
			unitSourceCodePath = deoglSkinShaderManager::euscpTessControlGeometry;
		}
		break;
		
	default:
		return;
	}
	
	pSources->SetPathTessellationControlSourceCode( pRenderThread.GetShader().
		GetSkinShaderManager().GetUnitSourceCodePath( unitSourceCodePath ) );
	pSources->SetVersion( "400" );
}

void deoglSkinShader::GenerateTessellationEvaluationSC(){
	deoglSkinShaderManager::eUnitSourceCodePath unitSourceCodePath;
	
	switch( pConfig.GetTessellationMode() ){
	case deoglSkinShaderConfig::etmLinear:
		switch( pConfig.GetShaderMode() ){
		case deoglSkinShaderConfig::esmDepth:
			unitSourceCodePath = deoglSkinShaderManager::euscpTessEvalDepth;
			break;
			
		default:
			unitSourceCodePath = deoglSkinShaderManager::euscpTessEvalGeometry;
		}
		break;
		
	default:
		return;
	}
	
	pSources->SetPathTessellationEvaluationSourceCode( pRenderThread.GetShader().
		GetSkinShaderManager().GetUnitSourceCodePath( unitSourceCodePath ) );
	pSources->SetVersion( "400" );
}

void deoglSkinShader::UpdateTextureTargets(){
	const deoglSkinShaderConfig::eGeometryModes geometryMode = pConfig.GetGeometryMode();
	const deoglSkinShaderConfig::eShaderModes shaderMode = pConfig.GetShaderMode();
	int textureUnitNumber = 0, i;
	
	for( i=0; i<ETT_COUNT; i++ ){
		pTextureTargets[ i ] = -1;
	}
	pUsedTextureTargetCount = 0;
	
	switch( shaderMode ){
	case deoglSkinShaderConfig::esmDepth:
		if( pConfig.GetOutputColor() ){
			pTextureTargets[ ettColor ] = textureUnitNumber++;
		}
		break;
		
	default:
		if( pConfig.GetTextureColor() ){
			pTextureTargets[ ettColor ] = textureUnitNumber++;
		}
	}
	
	if( pConfig.GetTextureTransparency() ){
		pTextureTargets[ ettTransparency ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureColorTintMask() ){
		pTextureTargets[ ettColorTintMask ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureSolidity() ){
		pTextureTargets[ ettSolidity ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureNormal() ){
		pTextureTargets[ ettNormal ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureHeight() ){
		pTextureTargets[ ettHeight ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureReflectivity() ){
		pTextureTargets[ ettReflectivity ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureRoughness() ){
		pTextureTargets[ ettRoughness ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureEnvMap() ){
		pTextureTargets[ ettEnvMap ] = textureUnitNumber++;
		if( REFLECTION_TEST_MODE < 2 ){
			pTextureTargets[ ettEnvMapFade ] = textureUnitNumber++;
		}
	}
	if( pConfig.GetTextureEmissivity() ){
		pTextureTargets[ ettEmissivity ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureRenderColor() ){
		pTextureTargets[ ettRenderColor ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureRefractionDistort() ){
		pTextureTargets[ ettRefractionDistort ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureAO() ){
		pTextureTargets[ ettAO ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureEnvRoom() ){
		pTextureTargets[ ettEnvRoom ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureEnvRoomMask() ){
		pTextureTargets[ ettEnvRoomMask ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureEnvRoomEmissivity() ){
		pTextureTargets[ ettEnvRoomEmissivity ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureAbsorption() ){
		pTextureTargets[ ettAbsorption ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureRimEmissivity() ){
		pTextureTargets[ ettRimEmissivity ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureNonPbrAlbedo() ){
		pTextureTargets[ ettNonPbrAlbedo ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureNonPbrMetalness() ){
		pTextureTargets[ ettNonPbrMetalness ] = textureUnitNumber++;
	}
	
	if( pConfig.GetDepthTestMode() != deoglSkinShaderConfig::edtmNone ){
		pTextureTargets[ ettDepthTest ] = textureUnitNumber++;
	}
	
	if( geometryMode == deoglSkinShaderConfig::egmParticle ){
		pTextureTargets[ ettSamples ] = textureUnitNumber++;
	}
	
	if( geometryMode == deoglSkinShaderConfig::egmPropField ){
		pTextureTargets[ ettSubInstance1 ] = textureUnitNumber++;
		pTextureTargets[ ettSubInstance2 ] = textureUnitNumber++;
		
	}else if( geometryMode == deoglSkinShaderConfig::egmHeightMap ){
		pTextureTargets[ ettHeightMapMask ] = textureUnitNumber++;
	}
	
	pTextureTargets[ ettXRayDepth ] = textureUnitNumber++;
	
	pUsedTextureTargetCount = textureUnitNumber;
}

void deoglSkinShader::UpdateUniformTargets(){
	int i;
	
	for( i=0; i<EIUT_COUNT; i++ ){
		pInstanceUniformTargets[ i ] = -1;
	}
	pUsedInstanceUniformTargetCount = 0;
	
	// instance parameters
	if( pConfig.GetGIMaterial() ){
		return;
	}
	
	if( pConfig.GetGeometryMode() == deoglSkinShaderConfig::egmParticle ){
		pInstanceUniformTargets[ eiutMatrixModel ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutSamplesParams ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutBurstFactor ] = pUsedInstanceUniformTargetCount++;
		if( pConfig.GetParticleMode() == deoglSkinShaderConfig::epmRibbon
		|| pConfig.GetParticleMode() == deoglSkinShaderConfig::epmBeam ){
			pInstanceUniformTargets[ eiutRibbonSheetCount ] = pUsedInstanceUniformTargetCount++;
		}
		
	}else{
		pInstanceUniformTargets[ eiutMatrixModel ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutMatrixNormal ] = pUsedInstanceUniformTargetCount++;
		
		pInstanceUniformTargets[ eiutMatrixTexCoord ] = pUsedInstanceUniformTargetCount++;
		if( pConfig.GetGeometryMode() == deoglSkinShaderConfig::egmPropField ){
			pInstanceUniformTargets[ eiutPropFieldParams ] = pUsedInstanceUniformTargetCount++;
		}
		if( pConfig.GetGeometryMode() == deoglSkinShaderConfig::egmHeightMap ){
			pInstanceUniformTargets[ eiutHeightTerrainMaskTCTransform ] = pUsedInstanceUniformTargetCount++;
			pInstanceUniformTargets[ eiutHeightTerrainMaskSelector ] = pUsedInstanceUniformTargetCount++;
		}
		pInstanceUniformTargets[ eiutDoubleSided ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutEnvMapFade ] = pUsedInstanceUniformTargetCount++;
		
		if( pConfig.GetVariations() ){
			pInstanceUniformTargets[ eiutVariationSeed ] = pUsedInstanceUniformTargetCount++;
		}
		
		pInstanceUniformTargets[ eiutBillboardPosTransform ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutBillboardParams ] = pUsedInstanceUniformTargetCount++;
	}
	
	/*
	pInstanceUniformTargets[ eiutTCTransformColor ] = pUsedInstanceUniformTargetCount++;
	if( pConfig.GetTextureNormal() ){
		pInstanceUniformTargets[ eiutTCTransformNormal ] = pUsedInstanceUniformTargetCount++;
	}
	if( pConfig.GetTextureReflectivityRoughness() ){
		pInstanceUniformTargets[ eiutTCTransformReflectivity ] = pUsedInstanceUniformTargetCount++;
	}
	if( pConfig.GetTextureEmissivity() ){
		pInstanceUniformTargets[ eiutTCTransformEmissivity ] = pUsedInstanceUniformTargetCount++;
	}
	if( pConfig.GetTextureRefractionDistort() ){
		pInstanceUniformTargets[ eiutTCTransformRefractionDistort ] = pUsedInstanceUniformTargetCount++;
	}
	if( pConfig.GetTextureAO() ){
		pInstanceUniformTargets[ eiutTCTransformAO ] = pUsedInstanceUniformTargetCount++;
	}
	*/
	
	pInstanceUniformTargets[ eiutIndexSPBTexParams ] = pUsedInstanceUniformTargetCount++;
	
	if( pConfig.GetGeometryMode() != deoglSkinShaderConfig::egmParticle ){
		pInstanceUniformTargets[ eiutInstColorTint ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstColorGamma ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstColorSolidityMultiplier ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstAOSolidityMultiplier ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstTransparencyMultiplier ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstSolidityMultiplier ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstHeightRemap ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstNormalStrength ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstNormalSolidityMultiplier ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstRoughnessRemap ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstRoughnessGamma ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstRoughnessSolidityMultiplier ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstReflectivitySolidityMultiplier ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstRefractionDistortStrength ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstEmissivityIntensity ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstEnvRoomSize ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstEnvRoomOffset ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstEnvRoomEmissivityIntensity ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstVariationEnableScale ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstReflectivityMultiplier ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstRimEmissivityIntensity ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstRimAngle ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstRimExponent ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstOutlineColor ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstOutlineColorTint ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstOutlineThickness ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstOutlineEmissivity ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstOutlineSolidity ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutInstOutlineEmissivityTint ] = pUsedInstanceUniformTargetCount++;
	}
	
	// shared parameter block support
	if( pConfig.GetSharedSPB() ){
		// re-map instance parameters since the ordering is slightly different
		int backup[ EIUT_COUNT ];
		for( i=0; i<EIUT_COUNT; i++ ){
			backup[ i ] = pInstanceUniformTargets[ i ];
			pInstanceUniformTargets[ i ] = -1;
		}
		
		for( i=0; i<vUBOInstParamMapCount; i++ ){
			if( backup[ vUBOInstParamMap[ i ] ] != -1 ){
				pInstanceUniformTargets[ vUBOInstParamMap[ i ] ] = i;
			}
		}
	}
}

void deoglSkinShader::InitShaderParameters(){
	deoglShaderBindingList &shaderStorageBlockList = pSources->GetShaderStorageBlockList();
	deoglShaderBindingList &uniformBlockList = pSources->GetUniformBlockList();
	deoglShaderBindingList &textureList = pSources->GetTextureList();
	deoglShaderBindingList &inputList = pSources->GetAttributeList();
	deoglShaderBindingList &outputList = pSources->GetOutputList();
	decStringList &parameterList = pSources->GetParameterList();
	const deoglSkinShaderConfig::eShaderModes shaderMode = pConfig.GetShaderMode();
	int i;
	
	// texture targets
	for( i=0; i<ETT_COUNT; i++ ){
		if( pTextureTargets[ i ] != -1 ){
			textureList.Add( vTextureTargetNames[ i ], pTextureTargets[ i ] );
		}
	}
	
	// global uniforms that are not particular to this shader
	// this is currently all a large hack until the right code is in place
	if( pConfig.GetGIMaterial() ){
		parameterList.Add( "pQuadParams" );
		
	}else{
		parameterList.Add( "pAmbient" ); // erutAmbient
		parameterList.Add( "pMatrixVP" ); // erutMatrixVP
		parameterList.Add( "pMatrixV" ); // erutMatrixV
		parameterList.Add( "pMatrixVn" ); // erutMatrixVn
		parameterList.Add( "pMatrixEnvMap" ); // erutMatrixEnvMap
		parameterList.Add( "pMatrixSkyBody" ); // erutMatrixSkyBody
		parameterList.Add( "pDepthToPosition" ); // erutDepthToPosition
		parameterList.Add( "pDepthToPosition2" ); // erutDepthToPosition2
		parameterList.Add( "pDepthTransform" ); // erutDepthTransform
		parameterList.Add( "pEnvMapLodLevel" ); // erutEnvMapLodLevel
		parameterList.Add( "pNorRoughCorrStrength" ); // erutNorRoughCorrStrength
		parameterList.Add( "pSkinDoesReflections" ); // erutSkinDoesReflections
		parameterList.Add( "pFlipCulling" ); // erutFlipCulling
		parameterList.Add( "pClearDepthValue" ); // erutClearDepthValue
		parameterList.Add( "pViewport" ); // erutViewport
		parameterList.Add( "pClipPlane" ); // erutClipPlane
		parameterList.Add( "pScreenSpace" ); // erutScreenSpace
		parameterList.Add( "pRenderSize" ); // erutRenderSize
		parameterList.Add( "pMipMapParams" ); // erutMipMapParams
		parameterList.Add( "pDepthOffset" ); // erutDepthOffset
		parameterList.Add( "pParticleLightHack" ); // erutParticleLightHack
		parameterList.Add( "pFadeRange" ); // erutFadeRange
		parameterList.Add( "pBillboardZScale" ); // erutBillboardZScale
		parameterList.Add( "pCameraStereoMatrix" ); // erutCameraStereoMatrix
		parameterList.Add( "pCameraAdaptedIntensity" ); // erutCameraAdaptedIntensity
		parameterList.Add( "pCameraRange" ); // erutCameraRange
		parameterList.Add( "pDepthSampleOffset" ); // erutDepthSampleOffset
		parameterList.Add( "pFSScreenCoordToTexCoord" ); // erutFSScreenCoordToTexCoord
		parameterList.Add( "pFSTexCoordToScreenCoord" ); // erutFSTexCoordToScreenCoord
		parameterList.Add( "pSSAOParams1" ); // erutSSAOParams1
		parameterList.Add( "pSSAOParams2" ); // erutSSAOParams2
		parameterList.Add( "pSSAOParams3" ); // erutSSAOParams3
		parameterList.Add( "pSSSSSParams1" ); // erutSSSSSParams1
		parameterList.Add( "pSSSSSParams2" ); // erutSSSSSParams2
		parameterList.Add( "pSSRParams1" ); // erutSSRParams1
		parameterList.Add( "pSSRParams2" ); // erutSSRParams2
		parameterList.Add( "pSSRParams3" ); // erutSSRParams3
		parameterList.Add( "pAOSelfShadow" ); // erutAOSelfShadow
		parameterList.Add( "pLumFragCoordScale" ); // erutLumFragCoordScale
		parameterList.Add( "pGIRayMatrix" ); // erutGIRayMatrix
		parameterList.Add( "pGIRayMatrixNormal" ); // erutGIRayMatrixNormal
		parameterList.Add( "pGIHighestCascade" ); // erutGIHighestCascade
		parameterList.Add( "pToneMapSceneKey" ); // erutToneMapSceneKey
		parameterList.Add( "pToneMapAdaption" ); // erutToneMapAdaption
		parameterList.Add( "pToneMapBloom" ); // erutToneMapBloom
		parameterList.Add( "pDebugDepthTransform" ); // erutDebugDepthTransform
		parameterList.Add( "pConditions1" ); // erutConditions1
	}
	
	for( i=0; i<ETUT_COUNT; i++ ){
		parameterList.Add( vTextureUniformTargetNames[ i ] );
	}
	
	if( ! pConfig.GetSharedSPB() ){
		for( i=0; i<EIUT_COUNT; i++ ){
			if( pInstanceUniformTargets[ i ] != -1 ){
				parameterList.Add( vInstanceUniformTargetNames[ i ] );
			}
		}
	}
	
	// special parameters set manually if present
	if( pConfig.GetSharedSPB() ){
		pTargetSPBInstanceIndexBase = parameterList.GetCount();
		parameterList.Add( "pSPBInstanceIndexBase" );
	}
	
	if( pConfig.GetVSRenderStereo() ){
		pTargetDrawIDOffset = parameterList.GetCount();
		parameterList.Add( "pDrawIDOffset" );
	}
	
	// inputs
	if( pConfig.GetGeometryMode() == deoglSkinShaderConfig::egmHeightMap ){
		inputList.Add( "inPosition", 0 );
		inputList.Add( "inHeight", 1 );
		inputList.Add( "inNormal", 2 );
		
	}else if( pConfig.GetGeometryMode() == deoglSkinShaderConfig::egmParticle ){
		inputList.Add( "inParticle0", 0 );
		inputList.Add( "inParticle1", 1 );
		inputList.Add( "inParticle2", 2 );
		inputList.Add( "inParticle3", 3 );
		/*
		if( pConfig.GetParticleMode() == deoglSkinShaderConfig::epmBeam ){
			inputList.Add( "inParticle4", 4 );
		}
		*/
		
	}else if( pConfig.GetGIMaterial() ){
		inputList.Add( "inPosition", 0 );
		
	}else{
		inputList.Add( "inPosition", 0 );
		inputList.Add( "inRealNormal", 1 );
		inputList.Add( "inNormal", 2 );
		inputList.Add( "inTangent", 3 );
		inputList.Add( "inTexCoord", 4 );
	}
	
	// outputs
	switch( shaderMode ){
	case deoglSkinShaderConfig::esmDepth:
		if( pConfig.GetOutputConstant() ){
			outputList.Add( "outConstant", 0 );
			
		}else if( pConfig.GetOutputColor() ){
			outputList.Add( "outColor", 0 );
		}
		if( pConfig.GetEncodeOutDepth() ){
			outputList.Add( "outDepth", outputList.GetCount() );
		}
		break;
		
	default:
		if( pConfig.GetLuminanceOnly() ){
			outputList.Add( "outLuminance", 0 );
			outputList.Add( "outNormal", 1 );
			
		}else if( pConfig.GetGIMaterial() ){
			outputList.Add( "outDiffuse", 0 );
			outputList.Add( "outReflectivity", 1 );
			outputList.Add( "outEmissivity", 2 );
			
		}else if( pConfig.GetGeometryMode() == deoglSkinShaderConfig::egmParticle
		&& ! GetRenderThread().GetChoices().GetRealTransparentParticles() ){
			outputList.Add( "outColor", 0 );
			
		}else{
			if( pRenderThread.GetCapabilities().GetMaxDrawBuffers() >= 8 ){
				outputList.Add( "outDiffuse", 0 );
				outputList.Add( "outNormal", 1 );
				outputList.Add( "outReflectivity", 2 );
				outputList.Add( "outRoughness", 3 );
				outputList.Add( "outAOSolidity", 4 );
				outputList.Add( "outSubSurface", 5 );
				outputList.Add( "outColor", 6 );
				
			}else{
				outputList.Add( "outDiffuse", 0 );
				outputList.Add( "outNormal", 1 );
				outputList.Add( "outReflectivity", 2 );
				outputList.Add( "outColor", 3 );
			}
		}
	}
	
	// uniform blocks
	uniformBlockList.Add( "RenderParameters", eubRenderParameters );
	uniformBlockList.Add( "TextureParameters", eubTextureParameters );
	uniformBlockList.Add( "InstanceParameters", eubInstanceParameters );
	uniformBlockList.Add( "SpecialParameters", eubSpecialParameters );
	uniformBlockList.Add( "InstanceIndex", eubInstanceIndex );
	
	// shader storage blocks
	shaderStorageBlockList.Add( "InstanceParametersSSBO", essboInstanceParameters );
	shaderStorageBlockList.Add( "InstanceIndexSSBO", essboInstanceIndex );
	shaderStorageBlockList.Add( "TextureParametersSSBO", essboTextureParameters );
}
