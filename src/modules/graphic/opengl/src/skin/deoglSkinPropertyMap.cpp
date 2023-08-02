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

#include "deoglSkinPropertyMap.h"

#include <dragengine/common/exceptions.h>


deoglSkinPropertyMap::ePropertyTypes deoglSkinPropertyMap::GetTypeFor( const char *type ){
	static const int entryCount = 82;
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
