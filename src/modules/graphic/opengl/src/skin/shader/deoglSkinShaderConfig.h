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

#ifndef _DEOGLSKINSHADERCONFIG_H_
#define _DEOGLSKINSHADERCONFIG_H_

#include <dragengine/common/string/decString.h>



/**
 * \brief Skin shader config.
 */
class deoglSkinShaderConfig{
public:
	/** \brief Shader modes. */
	enum eShaderModes{
		/** \brief Geometry. */
		esmGeometry,
		
		/** \brief Depth. */
		esmDepth,
		
		/** \brief Environment map. */
		esmEnvMap
	};
	
	/** \brief Geometry modes. */
	enum eGeometryModes{
		/** \brief Component. */
		egmComponent,
		
		/** \brief Billboard. */
		egmBillboard,
		
		/** \brief Decal. */
		egmDecal,
		
		/** \brief Prop field. */
		egmPropField,
		
		/** \brief Particle. */
		egmParticle,
		
		/** \brief Height map. */
		egmHeightMap
	};
	
	/** \brief Depth modes. */
	enum eDepthModes{
		/** \brief Projection with projective Z. */
		edmProjection,
		
		/** \brief Orthogonal with linear Z with no near z clipping. */
		edmOrthogonal,
		
		/** \brief Distance with linear Z. */
		edmDistance
	};
	
	/** \brief Material normal modes. */
	enum eMaterialNormalModes{
		/** \brief Float mode for 3-component float textures. */
		emnmFloat,
		
		/** \brief Basic encoding for 3-component int textures. */
		emnmIntBasic,
		
		/** \brief Spheremap encoding for fitting normals into a 2-component int texture. */
		emnmSpheremap
	};
	
	/** \brief Particle mode. */
	enum eParticleModes{
		/** \brief Particle particle mode. */
		epmParticle,
		
		/** \brief Ribbon particle mode. */
		epmRibbon,
		
		/** \brief Beam particle mode. */
		epmBeam
	};
	
	/** \brief Tessellation mode. */
	enum eTessallationModes{
		/** \brief No tessellation. */
		etmNone,
		
		/** \brief Linear tessellation. */
		etmLinear
	};
	
	/** \brief Depth test mode. */
	enum eDepthTestModes{
		/** \brief No depth test. */
		edtmNone,
		
		/** \brief Depth test discard smaller values. */
		edtmSmaller,
		
		/** \brief Depth test discard larger values. */
		edtmLarger
	};
	
	
	
public:
	eShaderModes pShaderMode;
	eGeometryModes pGeometryMode;
	eDepthModes pDepthMode;
	eMaterialNormalModes pMaterialNormalModeDec;
	eMaterialNormalModes pMaterialNormalModeEnc;
	eParticleModes pParticleMode;
	eTessallationModes pTessellationMode;
	eDepthTestModes pDepthTestMode;
	
	bool pDecodeInDepth;
	bool pEncodeOutDepth;
	bool pInverseDepth;
	bool pMaskedSolidity;
	bool pClipPlane;
	bool pNoZClip;
	bool pOutputConstant;
	bool pOutputColor;
	bool pAmbientLightProbe;
	bool pBillboard;
	bool pSkinReflections;
	bool pFadeOutRange;
	bool pVariations;
	bool pUseNormalRoughnessCorrection;
	bool pGSRenderCube;
	bool pSharedSPB;
	bool pSharedSPBUsingSSBO;
	int pSharedSPBArraySize;
	int pSharedSPBPadding;
	int pSPBInstanceArraySize;
	bool pOutline;
	bool pOutlineThicknessScreen;
	bool pLuminanceOnly;
	bool pGIMaterial;
	
	bool pDynamicColorTint;
	bool pDynamicColorGamma;
	bool pDynamicColorSolidityMultiplier;
	bool pDynamicAmbientOcclusionSolidityMultiplier;
	bool pDynamicTransparencyMultiplier;
	bool pDynamicSolidityMultiplier;
	bool pDynamicHeightRemap;
	bool pDynamicNormalStrength;
	bool pDynamicNormalSolidityMultiplier;
	bool pDynamicRoughnessRemap;
	bool pDynamicRoughnessGamma;
	bool pDynamicRoughnessSolidityMultiplier;
	bool pDynamicReflectivitySolidityMultiplier;
	bool pDynamicReflectivityMultiplier;
	bool pDynamicRefractionDistortStrength;
	bool pDynamicEmissivityTint;
	bool pDynamicEmissivityIntensity;
	bool pDynamicEnvRoomSize;
	bool pDynamicEnvRoomOffset;
	bool pDynamicEnvRoomEmissivityTint;
	bool pDynamicEnvRoomEmissivityIntensity;
	bool pDynamicThickness;
	bool pDynamicAbsorption;
	bool pDynamicVariation;
	bool pDynamicOutlineColor;
	bool pDynamicOutlineColorTint;
	bool pDynamicOutlineThickness;
	bool pDynamicOutlineSolidity;
	bool pDynamicOutlineEmissivity;
	bool pDynamicOutlineEmissivityTint;
	bool pDynamicRimEmissivityTint;
	bool pDynamicRimEmissivityIntensity;
	bool pDynamicRimAngle;
	bool pDynamicRimExponent;
	
	bool pTextureColor;
	bool pTextureColorTintMask;
	bool pTextureTransparency;
	bool pTextureSolidity;
	//bool pTextureColor;
	//bool pTextureTransparency;
	bool pTextureNormal;
	bool pTextureHeight;
	bool pTextureReflectivity;
	bool pTextureRoughness;
	bool pTextureEmissivity;
	bool pTextureAO;
	bool pTextureEnvMap;
	bool pTextureEnvMapEqui;
	bool pTextureAbsorption;
	bool pTextureRenderColor;
	bool pTextureRefractionDistort;
	bool pTextureEnvRoom;
	bool pTextureEnvRoomMask;
	bool pTextureEnvRoomEmissivity;
	bool pTextureRimEmissivity;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create skin shader configuration. */
	deoglSkinShaderConfig();
	
	/** \brief Clean up skin shader configuration. */
	~deoglSkinShaderConfig();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Reset configuration. */
	void Reset();
	
	
	
	/** \brief Shader mode. */
	inline eShaderModes GetShaderMode() const{ return pShaderMode; }
	
	/** \brief Set shader mode. */
	void SetShaderMode( eShaderModes mode );
	
	/** \brief Geometry mode. */
	inline eGeometryModes GetGeometryMode() const{ return pGeometryMode; }
	
	/** \brief Set geometry mode. */
	void SetGeometryMode( eGeometryModes mode );
	
	/** \brief Depth mode. */
	inline eDepthModes GetDepthMode() const{ return pDepthMode; }
	
	/** \brief Set depth mode. */
	void SetDepthMode( eDepthModes mode );
	
	/** \brief Material normal mode. */
	inline eMaterialNormalModes GetMaterialNormalModeDec() const{ return pMaterialNormalModeDec; }
	
	/** \brief Set material normal mode. */
	void SetMaterialNormalModeDec( eMaterialNormalModes mode );
	
	/** \brief Material normal mode. */
	inline eMaterialNormalModes GetMaterialNormalModeEnc() const{ return pMaterialNormalModeEnc; }
	
	/** \brief Set material normal mode. */
	void SetMaterialNormalModeEnc( eMaterialNormalModes mode );
	
	/** \brief Particle mode. */
	inline eParticleModes GetParticleMode() const{ return pParticleMode; }
	
	/** \brief Set particle mode. */
	void SetParticleMode( eParticleModes mode );
	
	/** \brief Tessellation mode. */
	inline eTessallationModes GetTessellationMode() const{ return pTessellationMode; }
	
	/** \brief Set tessellation mode. */
	void SetTessellationMode( eTessallationModes mode );
	
	/** \brief Depth test mode. */
	inline eDepthTestModes GetDepthTestMode() const{ return pDepthTestMode; }
	
	/** \brief Set depth test mode. */
	void SetDepthTestMode( eDepthTestModes mode );
	
	
	
	/** \brief Input depth has to be decoded. */
	inline bool GetDecodeInDepth() const{ return pDecodeInDepth; }
	
	/** \brief Set if input depth has to be decoded. */
	void SetDecodeInDepth( bool decodeInDepth );
	
	/** \brief Output depth has to be encoded. */
	inline bool GetEncodeOutDepth() const{ return pEncodeOutDepth; }
	
	/** \brief Set if output depth has to be encoded. */
	void SetEncodeOutDepth( bool encodeOutDepth );
	
	/** \brief Use inverse depth. */
	inline bool GetInverseDepth() const{ return pInverseDepth; }
	
	/** \brief Set use inverse depth. */
	void SetInverseDepth( bool inverseDepth );
	
	
	
	/** \brief Fragments with masked solidity values are discarded. */
	inline bool GetMaskedSolidity() const{ return pMaskedSolidity; }
	
	/** \brief Set if fragments with masked solidity values are discarded. */
	void SetMaskedSolidity( bool maskedSolidity );
	
	/** \brief Fragments are clipped against a clipping plane. */
	inline bool GetClipPlane() const{ return pClipPlane; }
	
	/** \brief Set if fragments are clipped against a clipping plane. */
	void SetClipPlane( bool clipPlane );
	
	/** \brief No z coordinate clipping has to be done. */
	inline bool GetNoZClip() const{ return pNoZClip; }
	
	/** \brief Set if no z coordinate clipping has to be done. */
	void SetNoZClip( bool noZClip );
	
	/** \brief Constant value is required as output (not set in the shader). */
	inline bool GetOutputConstant() const{ return pOutputConstant; }
	
	/** \brief Set if constant value is required as output (not set in the shader). */
	void SetOutputConstant( bool outputConstant );
	
	/** \brief Color and transparency texture properties are required as a single output. */
	inline bool GetOutputColor() const{ return pOutputColor; }
	
	/** \brief Set if color and transparency texture properties are required as a single output. */
	void SetOutputColor( bool outputColor );
	
	/** \brief Lighting has to be taken from an ambient light probe. */
	inline bool GetAmbientLightProbe() const{ return pAmbientLightProbe; }
	
	/** \brief Set if lighting has to be taken from an ambient light probe. */
	void SetAmbientLightProbe( bool ambientLightProbe );
	
	/** \brief Billboard is used. */
	inline bool GetBillboard() const{ return pBillboard; }
	
	/** \brief Set if billboard is used. */
	void SetBillboard( bool billboard );
	
	/** \brief Skin does reflections. */
	inline bool GetSkinReflections() const{ return pSkinReflections; }
	
	/** \brief Set if skin does reflections. */
	void SetSkinReflections( bool skinReflections );
	
	/** \brief Fade out near render range. */
	inline bool GetFadeOutRange() const{ return pFadeOutRange; }
	
	/** \brief Set if fade out near render range. */
	void SetFadeOutRange( bool fadeOutRange );
	
	/** \brief Variation textures are enabled using texture arrays. */
	inline bool GetVariations() const{ return pVariations; }
	
	/** \brief Set if variation textures are enabled using texture arrays. */
	void SetVariations( bool variations );
	
	/** \brief Use normal roughness correction. */
	inline bool GetUseNormalRoughnessCorrection() const{ return pUseNormalRoughnessCorrection; }
	
	/** \brief Set use normal roughness correction. */
	void SetUseNormalRoughnessCorrection( bool useNormalRoughnessCorrection );
	
	/** \brief Render to cube map using geometry shader. */
	inline bool GetGSRenderCube() const{ return pGSRenderCube; }
	
	/** \brief Set render to cube map using geometry shader. */
	void SetGSRenderCube( bool gsRenderCube );
	
	/** \brief Use shared SPB. */
	inline bool GetSharedSPB() const{ return pSharedSPB; }
	
	/** \brief Set use shared SPB. */
	void SetSharedSPB( bool sharedSPB );
	
	/** \brief Use shared SPB using SSBO. */
	inline bool GetSharedSPBUsingSSBO() const{ return pSharedSPBUsingSSBO; }
	
	/** \brief Set use shared SPB using SSBO. */
	void SetSharedSPBUsingSSBO( bool useSSBO );
	
	/** \brief Shared SPB array size. */
	inline int GetSharedSPBArraySize() const{ return pSharedSPBArraySize; }
	
	/** \brief Set shared SPB array size. */
	void SetSharedSPBArraySize( int arraySize );
	
	/** \brief Shared SPB padding. */
	inline int GetSharedSPBPadding() const{ return pSharedSPBPadding; }
	
	/** \brief Set shared SPB padding. */
	void SetSharedSPBPadding( int padding );
	
	/** \brief SPB instance array size. */
	inline int GetSPBInstanceArraySize() const{ return pSPBInstanceArraySize; }
	
	/** \brief Set SPB instance array size. */
	void SetSPBInstanceArraySize( int arraySize );
	
	/** \brief Render outline. */
	inline bool GetOutline() const{ return pOutline; }
	
	/** \brief Set render outline. */
	void SetOutline( bool outline );
	
	/** \brief Outline thickness relative to screen width. */
	inline bool GetOutlineThicknessScreen() const{ return pOutlineThicknessScreen; }
	
	/** \brief Set if outline thickness is relative to screen width. */
	void SetOutlineThicknessScreen( bool enable );
	
	/** \brief Luminance only. */
	inline bool GetLuminanceOnly() const{ return pLuminanceOnly; }
	
	/** \brief Set luminance only. */
	void SetLuminanceOnly( bool luminanceOnly );
	
	/** \brief GI material. */
	inline bool GetGIMaterial() const{ return pGIMaterial; }
	
	/** \brief Set GI material. */
	void SetGIMaterial( bool gimaterial );
	
	
	
	/** \brief Color tint texture property is dynamic. */
	inline bool GetDynamicColorTint() const{ return pDynamicColorTint; }
	
	/** \brief Set if color tint texture property is dynamic. */
	void SetDynamicColorTint( bool dynamic );
	
	/** \brief Color gamma texture property is dynamic. */
	inline bool GetDynamicColorGamma() const{ return pDynamicColorGamma; }
	
	/** \brief Set if color gamma texture property is dynamic. */
	void SetDynamicColorGamma( bool dynamic );
	
	/** \brief Color solidity multiplier texture property is dynamic. */
	inline bool GetDynamicColorSolidityMultiplier() const{ return pDynamicColorSolidityMultiplier; }
	
	/** \brief Set if color solidity multiplier texture property is dynamic. */
	void SetDynamicColorSolidityMultiplier( bool dynamic );
	
	/** \brief Ambient occlusion solidity multiplier texture property is dynamic. */
	inline bool GetDynamicAmbientOcclusionSolidityMultiplier() const{ return pDynamicAmbientOcclusionSolidityMultiplier; }
	
	/** \brief Set if ambient occlusion solidity multiplier texture property is dynamic. */
	void SetDynamicAmbientOcclusionSolidityMultiplier( bool dynamic );
	
	/** \brief Transparency multiplier texture property is dynamic. */
	inline bool GetDynamicTransparencyMultiplier() const{ return pDynamicTransparencyMultiplier; }
	
	/** \brief Set if transparency multiplier texture property is dynamic. */
	void SetDynamicTransparencyMultiplier( bool dynamic );
	
	/** \brief Solidity multiplier texture property is dynamic. */
	inline bool GetDynamicSolidityMultiplier() const{ return pDynamicSolidityMultiplier; }
	
	/** \brief Set if solidity multiplier texture property is dynamic. */
	void SetDynamicSolidityMultiplier( bool dynamic );
	
	/** \brief Height scale and height offset texture properties are dynamic. */
	inline bool GetDynamicHeightRemap() const{ return pDynamicHeightRemap; }
	
	/** \brief Set if height scale and height offset texture properties are dynamic. */
	void SetDynamicHeightRemap( bool dynamic );
	
	/** \brief Normal strength texture property is dynamic. */
	inline bool GetDynamicNormalStrength() const{ return pDynamicNormalStrength; }
	
	/** \brief Set if normal strength texture property is dynamic. */
	void SetDynamicNormalStrength( bool dynamic );
	
	/** \brief Normal solidity multiplier texture property is dynamic. */
	inline bool GetDynamicNormalSolidityMultiplier() const{ return pDynamicNormalSolidityMultiplier; }
	
	/** \brief Set if normal solidity multiplier texture property is dynamic. */
	void SetDynamicNormalSolidityMultiplier( bool dynamic );
	
	/** \brief Roughness remap texture property is dynamic. */
	inline bool GetDynamicRoughnessRemap() const{ return pDynamicRoughnessRemap; }
	
	/** \brief Set if roughness remap texture property is dynamic. */
	void SetDynamicRoughnessRemap( bool dynamic );
	
	/** \brief Roughness gamma texture property is dynamic. */
	inline bool GetDynamicRoughnessGamma() const{ return pDynamicRoughnessGamma; }
	
	/** \brief Set if roughness gamma texture property is dynamic. */
	void SetDynamicRoughnessGamma( bool dynamic );
	
	/** \brief Roughness solidity multiplier texture property is dynamic. */
	inline bool GetDynamicRoughnessSolidityMultiplier() const{ return pDynamicRoughnessSolidityMultiplier; }
	
	/** \brief Set if roughness solidity multiplier texture property is dynamic. */
	void SetDynamicRoughnessSolidityMultiplier( bool dynamic );
	
	/** \brief Reflectivity solidity multiplier texture property is dynamic. */
	inline bool GetDynamicReflectivitySolidityMultiplier() const{ return pDynamicReflectivitySolidityMultiplier; }
	
	/** \brief Set if reflectivity solidity multiplier texture property is dynamic. */
	void SetDynamicReflectivitySolidityMultiplier( bool dynamic );
	
	/** \brief Reflectivity multiplier texture property is dynamic. */
	inline bool GetDynamicReflectivityMultiplier() const{ return pDynamicReflectivityMultiplier; }
	
	/** \brief Set if reflectivity multiplier texture property is dynamic. */
	void SetDynamicReflectivityMultiplier( bool dynamic );
	
	/** \brief Refraction distort strength texture property is dynamic. */
	inline bool GetDynamicRefractionDistortStrength() const{ return pDynamicRefractionDistortStrength; }
	
	/** \brief Set if refraction distort strength texture property is dynamic. */
	void SetDynamicRefractionDistortStrength( bool dynamic );
	
	/** \brief Emissivity tint texture property is dynamic. */
	inline bool GetDynamicEmissivityTint() const{ return pDynamicEmissivityTint; }
	
	/** \brief Set if emissivity tint texture property is dynamic. */
	void SetDynamicEmissivityTint( bool dynamic );
	
	/** \brief Emissivity intensity texture property is dynamic. */
	inline bool GetDynamicEmissivityIntensity() const{ return pDynamicEmissivityIntensity; }
	
	/** \brief Set if emissivity intensity texture property is dynamic. */
	void SetDynamicEmissivityIntensity( bool dynamic );
	
	/** \brief Environent room size texture property is dynamic. */
	inline bool GetDynamicEnvRoomSize() const{ return pDynamicEnvRoomSize; }
	
	/** \brief Set if environent room size texture property is dynamic. */
	void SetDynamicEnvRoomSize( bool dynamic );
	
	/** \brief Environent room offset texture property is dynamic. */
	inline bool GetDynamicEnvRoomOffset() const{ return pDynamicEnvRoomOffset; }
	
	/** \brief Set if environent room offset texture property is dynamic. */
	void SetDynamicEnvRoomOffset( bool dynamic );
	
	/** \brief Environent room emissivity tint texture property is dynamic. */
	inline bool GetDynamicEnvRoomEmissivityTint() const{ return pDynamicEnvRoomEmissivityTint; }
	
	/** \brief Set if environent room emissivity tint texture property is dynamic. */
	void SetDynamicEnvRoomEmissivityTint( bool dynamic );
	
	/** \brief Environent room emissivity intensity texture property is dynamic. */
	inline bool GetDynamicEnvRoomEmissivityIntensity() const{ return pDynamicEnvRoomEmissivityIntensity; }
	
	/** \brief Set if environent room emissivity intensity texture property is dynamic. */
	void SetDynamicEnvRoomEmissivityIntensity( bool dynamic );
	
	/** \brief Thickness texture property is dynamic. */
	inline bool GetDynamicThickness() const{ return pDynamicThickness; }
	
	/** \brief Set if thickness texture property is dynamic. */
	void SetDynamicThickness( bool dynamic );
	
	/** \brief Absorption range or absorption half intensity distance property is dynamic. */
	inline bool GetDynamicAbsorption() const{ return pDynamicAbsorption; }
	
	/** \brief Set if absorption range or absorption half intensity distance property is dynamic. */
	void SetDynamicAbsorption( bool dynamic );
	
	/** \brief Variation U or V property is dynamic. */
	inline bool GetDynamicVariation() const{ return pDynamicVariation; }
	
	/** \brief Set if variation in U or V direction is property is dynamic. */
	void SetDynamicVariation( bool dynamic );
	
	/** \brief Outline color property is dynamic. */
	inline bool GetDynamicOutlineColor() const{ return pDynamicOutlineColor; }
	
	/** \brief Set if outline color property is dynamic. */
	void SetDynamicOutlineColor( bool dynamic );
	
	/** \brief Outline color tint property is dynamic. */
	inline bool GetDynamicOutlineColorTint() const{ return pDynamicOutlineColorTint; }
	
	/** \brief Set if outline color tint property is dynamic. */
	void SetDynamicOutlineColorTint( bool dynamic );
	
	/** \brief Outline thickness property is dynamic. */
	inline bool GetDynamicOutlineThickness() const{ return pDynamicOutlineThickness; }
	
	/** \brief Set if outline thickness property is dynamic. */
	void SetDynamicOutlineThickness( bool dynamic );
	
	/** \brief Outline solidity property is dynamic. */
	inline bool GetDynamicOutlineSolidity() const{ return pDynamicOutlineSolidity; }
	
	/** \brief Set if outline solidity property is dynamic. */
	void SetDynamicOutlineSolidity( bool dynamic );
	
	/** \brief Outline emissivity property is dynamic. */
	inline bool GetDynamicOutlineEmissivity() const{ return pDynamicOutlineEmissivity; }
	
	/** \brief Set if outline emissivity property is dynamic. */
	void SetDynamicOutlineEmissivity( bool dynamic );
	
	/** \brief Outline emissivity tint property is dynamic. */
	inline bool GetDynamicOutlineEmissivityTint() const{ return pDynamicOutlineEmissivityTint; }
	
	/** \brief Set if outline emissivity tint property is dynamic. */
	void SetDynamicOutlineEmissivityTint( bool dynamic );
	
	/** \brief Rim emissivity tint property is dynamic. */
	inline bool GetDynamicRimEmissivityTint() const{ return pDynamicRimEmissivityTint; }
	
	/** \brief Set if rim emissivity tint property is dynamic. */
	void SetDynamicRimEmissivityTint( bool dynamic );
	
	/** \brief Rim emissivity intensity property is dynamic. */
	inline bool GetDynamicRimEmissivityIntensity() const{ return pDynamicRimEmissivityIntensity; }
	
	/** \brief Set if rim emissivity intensity property is dynamic. */
	void SetDynamicRimEmissivityIntensity( bool dynamic );
	
	/** \brief Rim angle property is dynamic. */
	inline bool GetDynamicRimAngle() const{ return pDynamicRimAngle; }
	
	/** \brief Set if rim angle property is dynamic. */
	void SetDynamicRimAngle( bool dynamic );
	
	/** \brief Rim exponent property is dynamic. */
	inline bool GetDynamicRimExponent() const{ return pDynamicRimExponent; }
	
	/** \brief Set if rim exponent property is dynamic. */
	void SetDynamicRimExponent( bool dynamic );
	
	
	
	/** \brief Color texture is used. */
	inline bool GetTextureColor() const{ return pTextureColor; }
	
	/** \brief Set if color texture is used. */
	void SetTextureColor( bool hasTexture );
	
	/** \brief Color tint mask texture is used. */
	inline bool GetTextureColorTintMask() const{ return pTextureColorTintMask; }
	
	/** \brief Set if color tint mask texture is used. */
	void SetTextureColorTintMask( bool hasTexture );
	
	/** \brief Transparency texture is used. */
	inline bool GetTextureTransparency() const{ return pTextureTransparency; }
	
	/** \brief Set if transparency texture is used. */
	void SetTextureTransparency( bool textureTransparency );
	
	/** \brief Solidity texture is used. */
	inline bool GetTextureSolidity() const{ return pTextureSolidity; }
	
	/** \brief Set if solidity texture is used. */
	void SetTextureSolidity( bool textureSolidity );
	
	/** \brief Normal texture is used. */
	inline bool GetTextureNormal() const{ return pTextureNormal; }
	
	/** \brief Set if normal texture is used. */
	void SetTextureNormal( bool textureNormal );
	
	/** \brief Height texture is used. */
	inline bool GetTextureHeight() const{ return pTextureHeight; }
	
	/** \brief Set if height texture is used. */
	void SetTextureHeight( bool textureHeightCone );
	
	/** \brief Reflectivity texture is used. */
	inline bool GetTextureReflectivity() const{ return pTextureReflectivity; }
	
	/** \brief Set if reflectivity texture is used. */
	void SetTextureReflectivity( bool textureReflectivity );
	
	/** \brief Roughness texture is used. */
	inline bool GetTextureRoughness() const{ return pTextureRoughness; }
	
	/** \brief Set if roughness texture is used. */
	void SetTextureRoughness( bool textureRoughness );
	
	/** \brief Emissivity texture is used. */
	inline bool GetTextureEmissivity() const{ return pTextureEmissivity; }
	
	/** \brief Set if emissivity texture is used. */
	void SetTextureEmissivity( bool textureEmissivity );
	
	/** \brief Ambient occlusion texture is used. */
	inline bool GetTextureAO() const{ return pTextureAO; }
	
	/** \brief Set if ambient occlusion texture is used. */
	void SetTextureAO( bool textureAO );
	
	/** \brief Environment map textures are used. */
	inline bool GetTextureEnvMap() const{ return pTextureEnvMap; }
	
	/** \brief Set if environment map textures are used. */
	void SetTextureEnvMap( bool textureEnvMap );
	
	/** \brief Equi environment map textures are used. */
	inline bool GetTextureEnvMapEqui() const{ return pTextureEnvMapEqui; }
	
	/** \brief Set if equi environment map textures are used. */
	void SetTextureEnvMapEqui( bool textureEnvMapEqui );
	
	/** \brief Absorption texture is used. */
	inline bool GetTextureAbsorption() const{ return pTextureAbsorption; }
	
	/** \brief Set if absorption texture is used. */
	void SetTextureAbsorption( bool textureAbsorption );
	
	/** \brief Render color texture is used. */
	inline bool GetTextureRenderColor() const{ return pTextureRenderColor; }
	
	/** \brief Set if render color texture is used. */
	void SetTextureRenderColor( bool textureRenderColor );
	
	/** \brief Refraction distort texture is used. */
	inline bool GetTextureRefractionDistort() const{ return pTextureRefractionDistort; }
	
	/** \brief Set if refraction distort texture is used. */
	void SetTextureRefractionDistort( bool textureRefractionDistort );
	
	/** \brief Environment room texture is used. */
	inline bool GetTextureEnvRoom() const{ return pTextureEnvRoom; }
	
	/** \brief Set if environment room texture is used. */
	void SetTextureEnvRoom( bool textureEnvRoom );
	
	/** \brief Environment room mask texture is used. */
	inline bool GetTextureEnvRoomMask() const{ return pTextureEnvRoomMask; }
	
	/** \brief Set if environment room mask texture is used. */
	void SetTextureEnvRoomMask( bool textureEnvRoomMask );
	
	/** \brief Environment room emissivity texture is used. */
	inline bool GetTextureEnvRoomEmissivity() const{ return pTextureEnvRoomEmissivity; }
	
	/** \brief Set if environment room emissivity texture is used. */
	void SetTextureEnvRoomEmissivity( bool textureEnvRoomEmissivity );
	
	/** \brief Rim emissivity texture is used. */
	inline bool GetTextureRimEmissivity() const{ return pTextureRimEmissivity; }
	
	/** \brief Set if rim emissivity texture is used. */
	void SetTextureRimEmissivity( bool useTexture );
	/*@}*/
	
	
	
	/** \name Debug */
	/*@{*/
	/** \brief Create short string list with the configuration. */
	void DebugGetConfigString( decString &string ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set shader configuration from another shader configuration. */
	deoglSkinShaderConfig &operator=( const deoglSkinShaderConfig &config );
	
	/** \brief Shader configurations are equal. */
	bool operator==( const deoglSkinShaderConfig &config ) const;
	/*@}*/
};

#endif
