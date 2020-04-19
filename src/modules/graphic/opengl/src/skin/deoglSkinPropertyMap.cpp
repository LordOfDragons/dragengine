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
	if( strcmp( type, "color" ) == 0 ){
		return eptColor;
		
	}else if( strcmp( type, "color.gamma" ) == 0 ){
		return eptColorGamma;
		
	}else if( strcmp( type, "color.tint" ) == 0 ){
		return eptColorTint;
		
	}else if( strcmp( type, "color.tint.mask" ) == 0 ){
		return eptColorTintMask;
		
	}else if( strcmp( type, "color.solidity.multiplier" ) == 0 ){
		return eptColorSolidityMultiplier;
		
	}else if( strcmp( type, "color.omnidir" ) == 0 ){
		return eptColorOmnidir;
		
	}else if( strcmp( type, "color.omnidir.equirect" ) == 0 ){
		return eptColorOmnidirEquirect;
		
		
		
	}else if( strcmp( type, "ambient.occlusion" ) == 0 ){
		return eptAmbientOcclusion;
		
	}else if( strcmp( type, "ambient.occlusion.solidity.multiplier" ) == 0 ){
		return eptAmbientOcclusionSolidityMultiplier;
		
		
		
	}else if( strcmp( type, "normal" ) == 0 ){
		return eptNormal;
		
	}else if( strcmp( type, "normal.strength" ) == 0 ){
		return eptNormalStrength;
		
	}else if( strcmp( type, "normal.solidity.multiplier" ) == 0 ){
		return eptNormalSolidityMultiplier;
		
		
		
	}else if( strcmp( type, "height" ) == 0 ){
		return eptHeight;
		
	}else if( strcmp( type, "height.scale" ) == 0 ){
		return eptHeightScale;
		
	}else if( strcmp( type, "height.offset" ) == 0 ){
		return eptHeightOffset;
		
		
		
	}else if( strcmp( type, "roughness" ) == 0 ){
		return eptRoughness;
		
	}else if( strcmp( type, "roughness.remap.lower" ) == 0 ){
		return eptRoughnessRemapLower;
		
	}else if( strcmp( type, "roughness.remap.upper" ) == 0 ){
		return eptRoughnessRemapUpper;
		
	}else if( strcmp( type, "roughness.gamma" ) == 0 ){
		return eptRoughnessGamma;
		
	}else if( strcmp( type, "roughness.solidity.multiplier" ) == 0 ){
		return eptRoughnessSolidityMultiplier;
		
		
		
	}else if( strcmp( type, "transparency" ) == 0 ){
		return eptTransparency;
		
	}else if( strcmp( type, "transparency.multiplier" ) == 0 ){
		return eptTransparencyMultiplier;
		
		
		
	}else if( strcmp( type, "solidity" ) == 0 ){
		return eptSolidity;
		
	}else if( strcmp( type, "solidity.masked" ) == 0 ){
		return eptSolidityMasked;
		
	}else if( strcmp( type, "solidity.multiplier" ) == 0 ){
		return eptSolidityMultiplier;
		
		
		
	}else if( strcmp( type, "refraction.distort" ) == 0 ){
		return eptRefractionDistort;
		
	}else if( strcmp( type, "refraction.distort.strength" ) == 0 ){
		return eptRefractionDistortStrength;
		
		
		
	}else if( strcmp( type, "reflectivity" ) == 0 ){
		return eptReflectivity;
		
	}else if( strcmp( type, "reflectivity.multiplier" ) == 0 ){
		return eptReflectivityMultiplier;
		
	}else if( strcmp( type, "reflectivity.solidity.multiplier" ) == 0 ){
		return eptReflectivitySolidityMultiplier;
		
		
		
	}else if( strcmp( type, "environmentmap" ) == 0 ){
		return eptEnvironmentMap;
		
	}else if( strcmp( type, "reflected" ) == 0 ){
		return eptReflected;
		
	}else if( strcmp( type, "mirror" ) == 0 ){
		return eptMirror;
		
		
		
	}else if( strcmp( type, "emissivity" ) == 0 ){
		return eptEmissivity;
		
	}else if( strcmp( type, "emissivity.tint" ) == 0 ){
		return eptEmissivityTint;
		
	}else if( strcmp( type, "emissivity.intensity" ) == 0 ){
		return eptEmissivityIntensity;
		
	}else if( strcmp( type, "environmentroom" ) == 0 ){
		return eptEnvironmentRoom;
		
		
		
	}else if( strcmp( type, "environmentroom.size" ) == 0 ){
		return eptEnvironmentRoomSize;
		
	}else if( strcmp( type, "environmentroom.offset" ) == 0 ){
		return eptEnvironmentRoomOffset;
		
	}else if( strcmp( type, "environmentroom.mask" ) == 0 ){
		return eptEnvironmentRoomMask;
		
	}else if( strcmp( type, "environmentroom.emissivity" ) == 0 ){
		return eptEnvironmentRoomEmissivity;
		
	}else if( strcmp( type, "environmentroom.emissivity.tint" ) == 0 ){
		return eptEnvironmentRoomEmissivityTint;
		
	}else if( strcmp( type, "environmentroom.emissivity.intensity" ) == 0 ){
		return eptEnvironmentRoomEmissivityIntensity;
		
		
		
	}else if( strcmp( type, "thickness" ) == 0 ){
		return eptThickness;
		
	}else if( strcmp( type, "absorption" ) == 0 ){
		return eptAbsorption;
		
	}else if( strcmp( type, "absorption.range" ) == 0 ){
		return eptAbsorptionRange;
		
	}else if( strcmp( type, "absorption.halfIntensityDistance" ) == 0 ){
		return eptAbsorptionHalfIntensityDistance;
		
		
		
	}else if( strcmp( type, "shadeless" ) == 0 ){
		return eptShadeless;
		
	}else if( strcmp( type, "shadow.none" ) == 0 ){
		return eptShadowNone;
		
	}else if( strcmp( type, "shadow.importance" ) == 0 ){
		return eptShadowImportance;
		
	}else if( strcmp( type, "hint.nocompression" ) == 0 ){
		return eptHintNoCompression;
		
	}else if( strcmp( type, "hint.lightblocker" ) == 0 ){
		return eptHintLightBlocker;
		
	}else if( strcmp( type, "texcoord.clamp" ) == 0 ){
		return eptTexCoordClamp;
		
	}else if( strcmp( type, "texcoord.offset" ) == 0 ){
		return eptTexCoordOffset;
		
	}else if( strcmp( type, "texcoord.scale" ) == 0 ){
		return eptTexCoordScale;
		
	}else if( strcmp( type, "texcoord.rotate" ) == 0 ){
		return eptTexCoordRotate;
		
		
		
	}else if( strcmp( type, "variation.u" ) == 0 ){
		return eptVariationU;
		
	}else if( strcmp( type, "variation.v" ) == 0 ){
		return eptVariationV;
		
	}else if( strcmp( type, "variation.weight" ) == 0 ){
		return eptVariationWeight;
		
		
		
	}else if( strcmp( type, "particle.sheets" ) == 0 ){
		return eptParticleSheets;
		
		
		
	}else{
		return eptUnsupported;
	}
}
