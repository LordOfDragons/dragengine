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

#ifndef _DEOGLSKINPROPERTYMAP_H_
#define _DEOGLSKINPROPERTYMAP_H_



/**
 * Skin texture property mapping.
 */
namespace deoglSkinPropertyMap{
	/** Property types. */
	enum ePropertyTypes{
		eptUnsupported,
		
		eptColor,
		eptColorGamma,
		eptColorTint,
		eptColorTintMask,
		eptColorOmnidir,
		eptColorOmnidirEquirect,
		eptColorSolidityMultiplier,
		
		eptAmbientOcclusion,
		eptAmbientOcclusionSolidityMultiplier,
		
		eptNormal,
		eptNormalStrength,
		eptNormalSolidityMultiplier,
		
		eptHeight,
		eptHeightScale,
		eptHeightOffset,
		
		eptRoughness,
		eptRoughnessRemapLower,
		eptRoughnessRemapUpper,
		eptRoughnessGamma,
		eptRoughnessSolidityMultiplier,
		
		eptTransparency,
		eptTransparencyMultiplier,
		
		eptSolidity,
		eptSolidityMasked,
		eptSolidityMultiplier,
		eptSolidityFilterPriority,
		
		eptRefractionDistort,
		eptRefractionDistortStrength,
		
		eptReflectivity,
		eptReflectivityMultiplier,
		eptReflectivitySolidityMultiplier,
		
		eptEnvironmentMap,
		eptReflected,
		eptMirror,
		
		eptEmissivity,
		eptEmissivityTint,
		eptEmissivityIntensity,
		eptEmissivityCameraAdapted,
		
		eptEnvironmentRoom,
		eptEnvironmentRoomSize,
		eptEnvironmentRoomOffset,
		eptEnvironmentRoomMask,
		eptEnvironmentRoomEmissivity,
		eptEnvironmentRoomEmissivityTint,
		eptEnvironmentRoomEmissivityIntensity,
		
		eptThickness,
		
		eptAbsorption,
		eptAbsorptionRange,
		eptAbsorptionHalfIntensityDistance,
		
		eptShadeless,
		
		eptShadowNone,
		eptShadowImportance,
		
		eptHintNoCompression,
		eptHintLightBlocker,
		
		eptTexCoordClamp,
		eptTexCoordOffset,
		eptTexCoordScale,
		eptTexCoordRotate,
		
		eptOmniDirRotate,
		eptOmniDirRotateSpot,
		
		eptVariationU,
		eptVariationV,
		eptVariationWeight,
		
		eptParticleSheets,
		
		eptOutlineColor,
		eptOutlineColorTint,
		eptOutlineThickness,
		eptOutlineThicknessScreen,
		eptOutlineSolidity,
		eptOutlineEmissivity,
		eptOutlineEmissivityTint,
		eptOutlineEmissivityIntensity,
		
		eptRimEmissivity,
		eptRimEmissivityTint,
		eptRimEmissivityIntensity,
		eptRimAngle,
		eptRimExponent,
		
		eptNonPbrAlbedo,
		eptNonPbrMetalness,
		
		eptXRay
	};
	
	/** Type for texture property number. */
	ePropertyTypes GetTypeFor( const char *type );
	/*@}*/
};

#endif
