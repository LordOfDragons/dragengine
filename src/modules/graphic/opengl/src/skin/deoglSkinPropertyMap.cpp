/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglSkinPropertyMap.h"

#include <dragengine/common/exceptions.h>


deoglSkinPropertyMap::ePropertyTypes deoglSkinPropertyMap::GetTypeFor( const char *type ){
	static const int entryCount = 83;
	static struct sEntry{
		const char *name;
		deoglSkinPropertyMap::ePropertyTypes type;
	} entries[ entryCount ] = {
		{ "color", eptColor },
		{ "color.gamma", eptColorGamma },
		{ "color.tint", eptColorTint },
		{ "color.tint.mask", eptColorTintMask },
		{ "color.solidity.multiplier", eptColorSolidityMultiplier },
		{ "color.omnidir", eptColorOmnidir },
		{ "color.omnidir.equirect", eptColorOmnidirEquirect },
		
		{ "ambient.occlusion", eptAmbientOcclusion },
		{ "ambient.occlusion.solidity.multiplier", eptAmbientOcclusionSolidityMultiplier },
		
		{ "normal", eptNormal },
		{ "normal.strength", eptNormalStrength },
		{ "normal.solidity.multiplier", eptNormalSolidityMultiplier },
		
		{ "height", eptHeight },
		{ "height.scale", eptHeightScale },
		{ "height.offset", eptHeightOffset },
		
		{ "roughness", eptRoughness },
		{ "roughness.remap.lower", eptRoughnessRemapLower },
		{ "roughness.remap.upper", eptRoughnessRemapUpper },
		{ "roughness.gamma", eptRoughnessGamma },
		{ "roughness.solidity.multiplier", eptRoughnessSolidityMultiplier },
		
		{ "transparency", eptTransparency },
		{ "transparency.multiplier", eptTransparencyMultiplier },
		
		{ "solidity", eptSolidity },
		{ "solidity.masked", eptSolidityMasked },
		{ "solidity.multiplier", eptSolidityMultiplier },
		{ "solidity.filterPriority", eptSolidityFilterPriority },
		
		{ "refraction.distort", eptRefractionDistort },
		{ "refraction.distort.strength", eptRefractionDistortStrength },
		
		{ "reflectivity", eptReflectivity },
		{ "reflectivity.multiplier", eptReflectivityMultiplier },
		{ "reflectivity.solidity.multiplier", eptReflectivitySolidityMultiplier },
		
		{ "environmentmap", eptEnvironmentMap },
		{ "reflected", eptReflected },
		{ "mirror", eptMirror },
		
		{ "emissivity", eptEmissivity },
		{ "emissivity.tint", eptEmissivityTint },
		{ "emissivity.intensity", eptEmissivityIntensity },
		{ "emissivity.camera.adapted", eptEmissivityCameraAdapted },
		
		{ "environmentroom", eptEnvironmentRoom },
		{ "environmentroom.tint", eptEnvironmentRoomTint },
		{ "environmentroom.size", eptEnvironmentRoomSize },
		{ "environmentroom.offset", eptEnvironmentRoomOffset },
		{ "environmentroom.mask", eptEnvironmentRoomMask },
		{ "environmentroom.emissivity", eptEnvironmentRoomEmissivity },
		{ "environmentroom.emissivity.tint", eptEnvironmentRoomEmissivityTint },
		{ "environmentroom.emissivity.intensity", eptEnvironmentRoomEmissivityIntensity },
		
		{ "thickness", eptThickness },
		
		{ "absorption", eptAbsorption },
		{ "absorption.range", eptAbsorptionRange },
		{ "absorption.halfIntensityDistance", eptAbsorptionHalfIntensityDistance },
		
		{ "shadeless", eptShadeless },
		
		{ "shadow.none", eptShadowNone },
		{ "shadow.importance", eptShadowImportance },
		
		{ "hint.nocompression", eptHintNoCompression },
		{ "hint.lightblocker", eptHintLightBlocker },
		
		{ "texcoord.clamp", eptTexCoordClamp },
		{ "texcoord.offset", eptTexCoordOffset },
		{ "texcoord.scale", eptTexCoordScale },
		{ "texcoord.rotate", eptTexCoordRotate },
		
		{ "omnidir.rotate", eptOmniDirRotate },
		{ "omnidir.rotate.spot", eptOmniDirRotateSpot },
		
		{ "variation.u", eptVariationU },
		{ "variation.v", eptVariationV },
		{ "variation.weight", eptVariationWeight },
		
		{ "particle.sheets", eptParticleSheets },
		
		{ "outline.color", eptOutlineColor },
		{ "outline.color.tint", eptOutlineColorTint },
		{ "outline.thickness", eptOutlineThickness },
		{ "outline.thickness.screen", eptOutlineThicknessScreen },
		{ "outline.solidity", eptOutlineSolidity },
		{ "outline.emissivity", eptOutlineEmissivity },
		{ "outline.emissivity.tint", eptOutlineEmissivityTint },
		{ "outline.emissivity.intensity", eptOutlineEmissivityIntensity },
		
		{ "rim.emissivity", eptRimEmissivity },
		{ "rim.emissivity.tint", eptRimEmissivityTint },
		{ "rim.emissivity.intensity", eptRimEmissivityIntensity },
		{ "rim.angle", eptRimAngle },
		{ "rim.exponent", eptRimExponent },
		
		{ "nonpbr.albedo", eptNonPbrAlbedo },
		{ "nonpbr.metalness", eptNonPbrMetalness },
		
		{ "xray", eptXRay },
		
		{ "clip.plane", eptClipPlane },
		{ "clip.plane.border", eptClipPlaneBorder }
	};
	
	int i;
	for( i=0; i<entryCount; i++ ){
		if( strcmp( entries[ i ].name, type ) == 0 ){
			return entries[ i ].type;
		}
	}
	
	return eptUnsupported;
}
