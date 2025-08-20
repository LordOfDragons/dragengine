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
		eptEnvironmentRoomTint,
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
		
		eptXRay,
		
		eptClipPlane,
		eptClipPlaneBorder
	};
	
	/** Type for texture property number. */
	ePropertyTypes GetTypeFor( const char *type );
	/*@}*/
};

#endif
