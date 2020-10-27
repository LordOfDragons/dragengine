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
 * \brief Skin texture property mapping.
 */
namespace deoglSkinPropertyMap{
	/** \brief Property types. */
	enum ePropertyTypes{
		/** \brief Unsupported. */
		eptUnsupported,
		
		
		/** \brief Color. */
		eptColor,
		
		/** \brief Color Gamma. */
		eptColorGamma,
		
		/** \brief Tint color property. */
		eptColorTint,
		
		/** \brief Tint color mask property. */
		eptColorTintMask,
		
		/** \brief Color omni-directional cube map. */
		eptColorOmnidir,
		
		/** \brief Color omni-directional equirectangular map. */
		eptColorOmnidirEquirect,
		
		/** \brief color solidity multiplier. */
		eptColorSolidityMultiplier,
		
		/** \brief Ambient occlusion. */
		eptAmbientOcclusion,
		
		/** \brief Ambient occlusion solidity multiplier. */
		eptAmbientOcclusionSolidityMultiplier,
		
		
		
		/** \brief Normal. */
		eptNormal,
		
		/** \brief Normal strength. */
		eptNormalStrength,
		
		/** \brief Normal solidity multiplier. */
		eptNormalSolidityMultiplier,
		
		/** \brief Height. */
		eptHeight,
		
		/** \brief Height scale. */
		eptHeightScale,
		
		/** \brief Height offset. */
		eptHeightOffset,
		
		
		
		/** \brief Surface roughness. */
		eptRoughness,
		
		/** \brief Surface roughness remapping lower value. */
		eptRoughnessRemapLower,
		
		/** \brief Surface roughness remapping upper value. */
		eptRoughnessRemapUpper,
		
		/** \brief Surface roughness gamma correction. */
		eptRoughnessGamma,
		
		/** \brief Roughness solidity multiplier. */
		eptRoughnessSolidityMultiplier,
		
		
		
		/** \brief Transparency. */
		eptTransparency,
		
		/** \brief Transparency multiplier. */
		eptTransparencyMultiplier,
		
		/** \brief Solidity. */
		eptSolidity,
		
		/** \brief Solidity type. */
		eptSolidityMasked,
		
		/** \brief Solidity multiplier. */
		eptSolidityMultiplier,
		
		/** \brief Refraction distortion. */
		eptRefractionDistort,
		
		/** \brief Refraction distortion strength. */
		eptRefractionDistortStrength,
		
		
		
		/** \brief Reflectivity. */
		eptReflectivity,
		
		/** \brief Reflectivity multiplier. */
		eptReflectivityMultiplier,
		
		/** \brief Reflectivity solidity multiplier. */
		eptReflectivitySolidityMultiplier,
		
		/** \brief Environment map used for reflection. */
		eptEnvironmentMap,
		
		/** \brief Reflected by other geometry. */
		eptReflected,
		
		/** \brief Mirror. */
		eptMirror,
		
		
		
		/** \brief Emissivity. */
		eptEmissivity,
		
		/** \brief Tint emissivity property. */
		eptEmissivityTint,
		
		/** \brief Emissivity intensity. */
		eptEmissivityIntensity,
		
		
		
		/** \brief Environment room. */
		eptEnvironmentRoom,
		
		/** \brief Environment room size. */
		eptEnvironmentRoomSize,
		
		/** \brief Environment room offset. */
		eptEnvironmentRoomOffset,
		
		/** \brief Environment room mask. */
		eptEnvironmentRoomMask,
		
		/** \brief Environment room emissivity. */
		eptEnvironmentRoomEmissivity,
		
		/** \brief Tint environment room emissivity. */
		eptEnvironmentRoomEmissivityTint,
		
		/** \brief Environment room emissivity intensity. */
		eptEnvironmentRoomEmissivityIntensity,
		
		
		
		/** \brief Thickness. */
		eptThickness,
		
		/** \brief Absorption. */
		eptAbsorption,
		
		/** \brief Absorption range. */
		eptAbsorptionRange,
		
		/** \brief Absorption half intensity distance. */
		eptAbsorptionHalfIntensityDistance,
		
		
		
		/** \brief Shadeless. */
		eptShadeless,
		
		/** \brief No shadow. */
		eptShadowNone,
		
		/** \brief Shadow importance. */
		eptShadowImportance,
		
		
		
		/** \brief Hint no compression. */
		eptHintNoCompression,
		
		/** \brief Hint light blocker. */
		eptHintLightBlocker,
		
		
		
		/** \brief Clamp texture coordinates. */
		eptTexCoordClamp,
		
		/** \brief Texture coordinates offset. */
		eptTexCoordOffset,
		
		/** \brief Texture coordinates scaling. */
		eptTexCoordScale,
		
		/** \brief Texture coordinates rotation. */
		eptTexCoordRotate,
		
		/** \brief Omni direction texture rotation. */
		eptOmniDirRotate,
		
		/** \brief Omni direction texture rotation applied only on spot type light sources. */
		eptOmniDirRotateSpot,
		
		
		
		/** \brief Use texture variation in U direction. */
		eptVariationU,
		
		/** \brief Use texture variation in V direction. */
		eptVariationV,
		
		/** \brief Weighting of texture variation layers. */
		eptVariationWeight,
		
		
		
		/** \brief Particle beam and ribbon sheet count. */
		eptParticleSheets,
		
		
		
		/** \brief Outline color. */
		eptOutlineColor,
		
		/** \brief Outline color tint. */
		eptOutlineColorTint,
		
		/** \brief Outline thickness. */
		eptOutlineThickness,
		
		/** \brief Outline thickness relative to screen/world switch. */
		eptOutlineThicknessScreen,
		
		/** \brief Outline solidity. */
		eptOutlineSolidity,
		
		/** \brief Outline emissivity. */
		eptOutlineEmissivity,
		
		/** \brief Outline emissivity tint. */
		eptOutlineEmissivityTint,
		
		/** \brief Outline emissivity intensity. */
		eptOutlineEmissivityIntensity,
		
		
		
		/** \brief Rim emissivity. */
		eptRimEmissivity,
		
		/** \brief Rim emissivity tint. */
		eptRimEmissivityTint,
		
		/** \brief Rim emissivity intensity. */
		eptRimEmissivityIntensity,
		
		/** \brief Rim angle. */
		eptRimAngle,
		
		/** \brief Rim exponent. */
		eptRimExponent
	};
	
	/** \brief Type for texture property number. */
	ePropertyTypes GetTypeFor( const char *type );
	/*@}*/
};

#endif
