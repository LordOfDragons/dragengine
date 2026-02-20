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

#ifndef _DEOGLSKINSHADERCONFIG_H_
#define _DEOGLSKINSHADERCONFIG_H_

#include <stdint.h>
#include <dragengine/common/string/decString.h>



/**
 * Skin shader config.
 */
class deoglSkinShaderConfig{
public:
	/** Shader modes. */
	enum eShaderModes{
		/** Geometry. */
		esmGeometry,
		
		/** Depth. */
		esmDepth,
		
		/** Environment map. */
		esmEnvMap
	};
	
	/** Geometry modes. */
	enum eGeometryModes{
		/** Component. */
		egmComponent,
		
		/** Billboard. */
		egmBillboard,
		
		/** Decal. */
		egmDecal,
		
		/** Prop field. */
		egmPropField,
		
		/** Prop field. */
		egmPropFieldImposter,
		
		/** Particle. */
		egmParticle,
		
		/** Height map. */
		egmHeightMap
	};
	
	/** Depth modes. */
	enum eDepthModes{
		/** Projection with projective Z. */
		edmProjection,
		
		/** Orthogonal with linear Z with no near z clipping. */
		edmOrthogonal,
		
		/** Distance with linear Z. */
		edmDistance
	};
	
	/** Material normal modes. */
	enum eMaterialNormalModes{
		/** Float mode for 3-component float textures. */
		emnmFloat,
		
		/** Basic encoding for 3-component int textures. */
		emnmIntBasic,
		
		/** Spheremap encoding for fitting normals into a 2-component int texture. */
		emnmSpheremap
	};
	
	/** Particle mode. */
	enum eParticleModes{
		/** Particle particle mode. */
		epmParticle,
		
		/** Ribbon particle mode. */
		epmRibbon,
		
		/** Beam particle mode. */
		epmBeam
	};
	
	/** Tessellation mode. */
	enum eTessallationModes{
		/** No tessellation. */
		etmNone,
		
		/** Linear tessellation. */
		etmLinear
	};
	
	/** Depth test mode. */
	enum eDepthTestModes{
		/** No depth test. */
		edtmNone,
		
		/** Depth test discard smaller values. */
		edtmSmaller,
		
		/** Depth test discard larger values. */
		edtmLarger
	};
	
	/** Layered rendering mode. */
	enum eLayeredRenderingMode{
		/** No layered rendering. */
		elrmNone,
		
		/** Stereo rendering. */
		elrmStereo,
		
		/** Cube map rendering. */
		elrmCube,
		
		/** Cascaded map rendering. */
		elrmCascaded
	};
	
	/** Output mode. */
	enum eOutputMode{
		/** Default. */
		eomDefault,
		
		/** Constant. */
		eomConstant,
		
		/** Color. */
		eomColor,
		
		/** Material. */
		eomMaterial,
		
		/** Material with reduced FBO attachment count. */
		eomMaterialReduced,
		
		/** Luminance only. */
		eomLuminance
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
	bool pOutputConstant;
	bool pOutputColor;
	bool pAmbientLightProbe;
	bool pSkinReflections;
	bool pFadeOutRange;
	bool pVariations;
	bool pUseNormalRoughnessCorrection;
	bool pGSRenderCube;
	bool pGSRenderCascaded;
	bool pGSRenderStereo;
	bool pVSRenderStereo;
	bool pSharedSPB;
	bool pOutline;
	bool pOutlineThicknessScreen;
	bool pLuminanceOnly;
	bool pGIMaterial;
	bool pSkinClipPlane;
	
	bool pDynamicColorTint;
	bool pDynamicColorGamma;
	bool pDynamicColorSolidityMultiplier;
	bool pDynamicAmbientOcclusionSolidityMultiplier;
	bool pDynamicTransparencyMultiplier;
	bool pDynamicSolidityRemap;
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
	bool pDynamicEnvRoomTint;
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
	bool pDynamicSkinClipPlane;
	bool pDynamicSkinClipPlaneBorder;
	
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
	bool pTextureNonPbrAlbedo;
	bool pTextureNonPbrMetalness;
	
	uint32_t pKey1;
	uint32_t pKey2;
	uint64_t pKey3;
	uint32_t pKey4;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin shader configuration. */
	deoglSkinShaderConfig();
	
	/** Create copy of skin shader configuration. */
	deoglSkinShaderConfig(const deoglSkinShaderConfig &copy);
	
	/** Clean up skin shader configuration. */
	~deoglSkinShaderConfig();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Reset configuration. */
	void Reset();
	
	
	
	/** Shader mode. */
	inline eShaderModes GetShaderMode() const{ return pShaderMode; }
	
	/** Set shader mode. */
	void SetShaderMode(eShaderModes mode);
	
	/** Geometry mode. */
	inline eGeometryModes GetGeometryMode() const{ return pGeometryMode; }
	
	/** Set geometry mode. */
	void SetGeometryMode(eGeometryModes mode);
	
	/** Depth mode. */
	inline eDepthModes GetDepthMode() const{ return pDepthMode; }
	
	/** Set depth mode. */
	void SetDepthMode(eDepthModes mode);
	
	/** Material normal mode. */
	inline eMaterialNormalModes GetMaterialNormalModeDec() const{ return pMaterialNormalModeDec; }
	
	/** Set material normal mode. */
	void SetMaterialNormalModeDec(eMaterialNormalModes mode);
	
	/** Material normal mode. */
	inline eMaterialNormalModes GetMaterialNormalModeEnc() const{ return pMaterialNormalModeEnc; }
	
	/** Set material normal mode. */
	void SetMaterialNormalModeEnc(eMaterialNormalModes mode);
	
	/** Particle mode. */
	inline eParticleModes GetParticleMode() const{ return pParticleMode; }
	
	/** Set particle mode. */
	void SetParticleMode(eParticleModes mode);
	
	/** Tessellation mode. */
	inline eTessallationModes GetTessellationMode() const{ return pTessellationMode; }
	
	/** Set tessellation mode. */
	void SetTessellationMode(eTessallationModes mode);
	
	/** Depth test mode. */
	inline eDepthTestModes GetDepthTestMode() const{ return pDepthTestMode; }
	
	/** Set depth test mode. */
	void SetDepthTestMode(eDepthTestModes mode);
	
	
	
	/** Input depth has to be decoded. */
	inline bool GetDecodeInDepth() const{ return pDecodeInDepth; }
	
	/** Set if input depth has to be decoded. */
	void SetDecodeInDepth(bool decodeInDepth);
	
	/** Output depth has to be encoded. */
	inline bool GetEncodeOutDepth() const{ return pEncodeOutDepth; }
	
	/** Set if output depth has to be encoded. */
	void SetEncodeOutDepth(bool encodeOutDepth);
	
	/** Use inverse depth. */
	inline bool GetInverseDepth() const{ return pInverseDepth; }
	
	/** Set use inverse depth. */
	void SetInverseDepth(bool inverseDepth);
	
	
	
	/** Fragments with masked solidity values are discarded. */
	inline bool GetMaskedSolidity() const{ return pMaskedSolidity; }
	
	/** Set if fragments with masked solidity values are discarded. */
	void SetMaskedSolidity(bool maskedSolidity);
	
	/** Fragments are clipped against a clipping plane. */
	inline bool GetClipPlane() const{ return pClipPlane; }
	
	/** Set if fragments are clipped against a clipping plane. */
	void SetClipPlane(bool clipPlane);
	
	/** Constant value is required as output (not set in the shader). */
	inline bool GetOutputConstant() const{ return pOutputConstant; }
	
	/** Set if constant value is required as output (not set in the shader). */
	void SetOutputConstant(bool outputConstant);
	
	/** Color and transparency texture properties are required as a single output. */
	inline bool GetOutputColor() const{ return pOutputColor; }
	
	/** Set if color and transparency texture properties are required as a single output. */
	void SetOutputColor(bool outputColor);
	
	/** Lighting has to be taken from an ambient light probe. */
	inline bool GetAmbientLightProbe() const{ return pAmbientLightProbe; }
	
	/** Set if lighting has to be taken from an ambient light probe. */
	void SetAmbientLightProbe(bool ambientLightProbe);
	
	/** Skin does reflections. */
	inline bool GetSkinReflections() const{ return pSkinReflections; }
	
	/** Set if skin does reflections. */
	void SetSkinReflections(bool skinReflections);
	
	/** Fade out near render range. */
	inline bool GetFadeOutRange() const{ return pFadeOutRange; }
	
	/** Set if fade out near render range. */
	void SetFadeOutRange(bool fadeOutRange);
	
	/** Variation textures are enabled using texture arrays. */
	inline bool GetVariations() const{ return pVariations; }
	
	/** Set if variation textures are enabled using texture arrays. */
	void SetVariations(bool variations);
	
	/** Use normal roughness correction. */
	inline bool GetUseNormalRoughnessCorrection() const{ return pUseNormalRoughnessCorrection; }
	
	/** Set use normal roughness correction. */
	void SetUseNormalRoughnessCorrection(bool useNormalRoughnessCorrection);
	
	/** Render to cube map using geometry shader. */
	inline bool GetGSRenderCube() const{ return pGSRenderCube; }
	
	/** Set render to cube map using geometry shader. */
	void SetGSRenderCube(bool gsRenderCube);
	
	/** Render to cascaded using geometry shader. */
	inline bool GetGSRenderCascaded() const{ return pGSRenderCascaded; }
	
	/** Set render to cascaded using geometry shader. */
	void SetGSRenderCascaded(bool gsRenderCascaded);
	
	/** Render dual view using geometry shader. */
	inline bool GetGSRenderStereo() const{ return pGSRenderStereo; }
	
	/** Set render dual view using geometry shader. */
	void SetGSRenderStereo(bool gsRenderStereo);
	
	/** Render dual view using vertex shader. */
	inline bool GetVSRenderStereo() const{ return pVSRenderStereo; }
	
	/** Set render dual view using vertex shader. */
	void SetVSRenderStereo(bool vsRenderStereo);
	
	/** Use shared SPB. */
	inline bool GetSharedSPB() const{ return pSharedSPB; }
	
	/** Set use shared SPB. */
	void SetSharedSPB(bool sharedSPB);
	
	/** Render outline. */
	inline bool GetOutline() const{ return pOutline; }
	
	/** Set render outline. */
	void SetOutline(bool outline);
	
	/** Outline thickness relative to screen width. */
	inline bool GetOutlineThicknessScreen() const{ return pOutlineThicknessScreen; }
	
	/** Set if outline thickness is relative to screen width. */
	void SetOutlineThicknessScreen(bool enable);
	
	/** Luminance only. */
	inline bool GetLuminanceOnly() const{ return pLuminanceOnly; }
	
	/** Set luminance only. */
	void SetLuminanceOnly(bool luminanceOnly);
	
	/** GI material. */
	inline bool GetGIMaterial() const{ return pGIMaterial; }
	void SetGIMaterial(bool gimaterial);
	
	/** Use skin clip plane. */
	inline bool GetSkinClipPlane() const{ return pSkinClipPlane; }
	void SetSkinClipPlane(bool skinClipPlane);
	
	
	
	/** Color tint texture property is dynamic. */
	inline bool GetDynamicColorTint() const{ return pDynamicColorTint; }
	
	/** Set if color tint texture property is dynamic. */
	void SetDynamicColorTint(bool dynamic);
	
	/** Color gamma texture property is dynamic. */
	inline bool GetDynamicColorGamma() const{ return pDynamicColorGamma; }
	
	/** Set if color gamma texture property is dynamic. */
	void SetDynamicColorGamma(bool dynamic);
	
	/** Color solidity multiplier texture property is dynamic. */
	inline bool GetDynamicColorSolidityMultiplier() const{ return pDynamicColorSolidityMultiplier; }
	
	/** Set if color solidity multiplier texture property is dynamic. */
	void SetDynamicColorSolidityMultiplier(bool dynamic);
	
	/** Ambient occlusion solidity multiplier texture property is dynamic. */
	inline bool GetDynamicAmbientOcclusionSolidityMultiplier() const{ return pDynamicAmbientOcclusionSolidityMultiplier; }
	
	/** Set if ambient occlusion solidity multiplier texture property is dynamic. */
	void SetDynamicAmbientOcclusionSolidityMultiplier(bool dynamic);
	
	/** Transparency multiplier texture property is dynamic. */
	inline bool GetDynamicTransparencyMultiplier() const{ return pDynamicTransparencyMultiplier; }
	
	/** Set if transparency multiplier texture property is dynamic. */
	void SetDynamicTransparencyMultiplier(bool dynamic);
	
	/** Solidity remap texture property is dynamic. */
	inline bool GetDynamicSolidityRemap() const{ return pDynamicSolidityRemap; }
	
	/** Set if solidity remap texture property is dynamic. */
	void SetDynamicSolidityRemap(bool dynamic);
	
	/** Height scale and height offset texture properties are dynamic. */
	inline bool GetDynamicHeightRemap() const{ return pDynamicHeightRemap; }
	
	/** Set if height scale and height offset texture properties are dynamic. */
	void SetDynamicHeightRemap(bool dynamic);
	
	/** Normal strength texture property is dynamic. */
	inline bool GetDynamicNormalStrength() const{ return pDynamicNormalStrength; }
	
	/** Set if normal strength texture property is dynamic. */
	void SetDynamicNormalStrength(bool dynamic);
	
	/** Normal solidity multiplier texture property is dynamic. */
	inline bool GetDynamicNormalSolidityMultiplier() const{ return pDynamicNormalSolidityMultiplier; }
	
	/** Set if normal solidity multiplier texture property is dynamic. */
	void SetDynamicNormalSolidityMultiplier(bool dynamic);
	
	/** Roughness remap texture property is dynamic. */
	inline bool GetDynamicRoughnessRemap() const{ return pDynamicRoughnessRemap; }
	
	/** Set if roughness remap texture property is dynamic. */
	void SetDynamicRoughnessRemap(bool dynamic);
	
	/** Roughness gamma texture property is dynamic. */
	inline bool GetDynamicRoughnessGamma() const{ return pDynamicRoughnessGamma; }
	
	/** Set if roughness gamma texture property is dynamic. */
	void SetDynamicRoughnessGamma(bool dynamic);
	
	/** Roughness solidity multiplier texture property is dynamic. */
	inline bool GetDynamicRoughnessSolidityMultiplier() const{ return pDynamicRoughnessSolidityMultiplier; }
	
	/** Set if roughness solidity multiplier texture property is dynamic. */
	void SetDynamicRoughnessSolidityMultiplier(bool dynamic);
	
	/** Reflectivity solidity multiplier texture property is dynamic. */
	inline bool GetDynamicReflectivitySolidityMultiplier() const{ return pDynamicReflectivitySolidityMultiplier; }
	
	/** Set if reflectivity solidity multiplier texture property is dynamic. */
	void SetDynamicReflectivitySolidityMultiplier(bool dynamic);
	
	/** Reflectivity multiplier texture property is dynamic. */
	inline bool GetDynamicReflectivityMultiplier() const{ return pDynamicReflectivityMultiplier; }
	
	/** Set if reflectivity multiplier texture property is dynamic. */
	void SetDynamicReflectivityMultiplier(bool dynamic);
	
	/** Refraction distort strength texture property is dynamic. */
	inline bool GetDynamicRefractionDistortStrength() const{ return pDynamicRefractionDistortStrength; }
	
	/** Set if refraction distort strength texture property is dynamic. */
	void SetDynamicRefractionDistortStrength(bool dynamic);
	
	/** Emissivity tint texture property is dynamic. */
	inline bool GetDynamicEmissivityTint() const{ return pDynamicEmissivityTint; }
	
	/** Set if emissivity tint texture property is dynamic. */
	void SetDynamicEmissivityTint(bool dynamic);
	
	/** Emissivity intensity texture property is dynamic. */
	inline bool GetDynamicEmissivityIntensity() const{ return pDynamicEmissivityIntensity; }
	
	/** Set if emissivity intensity texture property is dynamic. */
	void SetDynamicEmissivityIntensity(bool dynamic);
	
	/** Environent room tint texture property is dynamic. */
	inline bool GetDynamicEnvRoomTint() const{ return pDynamicEnvRoomTint; }
	
	/** Set if environent room tint texture property is dynamic. */
	void SetDynamicEnvRoomTint(bool dynamic);
	
	/** Environent room size texture property is dynamic. */
	inline bool GetDynamicEnvRoomSize() const{ return pDynamicEnvRoomSize; }
	
	/** Set if environent room size texture property is dynamic. */
	void SetDynamicEnvRoomSize(bool dynamic);
	
	/** Environent room offset texture property is dynamic. */
	inline bool GetDynamicEnvRoomOffset() const{ return pDynamicEnvRoomOffset; }
	
	/** Set if environent room offset texture property is dynamic. */
	void SetDynamicEnvRoomOffset(bool dynamic);
	
	/** Environent room emissivity tint texture property is dynamic. */
	inline bool GetDynamicEnvRoomEmissivityTint() const{ return pDynamicEnvRoomEmissivityTint; }
	
	/** Set if environent room emissivity tint texture property is dynamic. */
	void SetDynamicEnvRoomEmissivityTint(bool dynamic);
	
	/** Environent room emissivity intensity texture property is dynamic. */
	inline bool GetDynamicEnvRoomEmissivityIntensity() const{ return pDynamicEnvRoomEmissivityIntensity; }
	
	/** Set if environent room emissivity intensity texture property is dynamic. */
	void SetDynamicEnvRoomEmissivityIntensity(bool dynamic);
	
	/** Thickness texture property is dynamic. */
	inline bool GetDynamicThickness() const{ return pDynamicThickness; }
	
	/** Set if thickness texture property is dynamic. */
	void SetDynamicThickness(bool dynamic);
	
	/** Absorption range or absorption half intensity distance property is dynamic. */
	inline bool GetDynamicAbsorption() const{ return pDynamicAbsorption; }
	
	/** Set if absorption range or absorption half intensity distance property is dynamic. */
	void SetDynamicAbsorption(bool dynamic);
	
	/** Variation U or V property is dynamic. */
	inline bool GetDynamicVariation() const{ return pDynamicVariation; }
	
	/** Set if variation in U or V direction is property is dynamic. */
	void SetDynamicVariation(bool dynamic);
	
	/** Outline color property is dynamic. */
	inline bool GetDynamicOutlineColor() const{ return pDynamicOutlineColor; }
	
	/** Set if outline color property is dynamic. */
	void SetDynamicOutlineColor(bool dynamic);
	
	/** Outline color tint property is dynamic. */
	inline bool GetDynamicOutlineColorTint() const{ return pDynamicOutlineColorTint; }
	
	/** Set if outline color tint property is dynamic. */
	void SetDynamicOutlineColorTint(bool dynamic);
	
	/** Outline thickness property is dynamic. */
	inline bool GetDynamicOutlineThickness() const{ return pDynamicOutlineThickness; }
	
	/** Set if outline thickness property is dynamic. */
	void SetDynamicOutlineThickness(bool dynamic);
	
	/** Outline solidity property is dynamic. */
	inline bool GetDynamicOutlineSolidity() const{ return pDynamicOutlineSolidity; }
	
	/** Set if outline solidity property is dynamic. */
	void SetDynamicOutlineSolidity(bool dynamic);
	
	/** Outline emissivity property is dynamic. */
	inline bool GetDynamicOutlineEmissivity() const{ return pDynamicOutlineEmissivity; }
	
	/** Set if outline emissivity property is dynamic. */
	void SetDynamicOutlineEmissivity(bool dynamic);
	
	/** Outline emissivity tint property is dynamic. */
	inline bool GetDynamicOutlineEmissivityTint() const{ return pDynamicOutlineEmissivityTint; }
	
	/** Set if outline emissivity tint property is dynamic. */
	void SetDynamicOutlineEmissivityTint(bool dynamic);
	
	/** Rim emissivity tint property is dynamic. */
	inline bool GetDynamicRimEmissivityTint() const{ return pDynamicRimEmissivityTint; }
	
	/** Set if rim emissivity tint property is dynamic. */
	void SetDynamicRimEmissivityTint(bool dynamic);
	
	/** Rim emissivity intensity property is dynamic. */
	inline bool GetDynamicRimEmissivityIntensity() const{ return pDynamicRimEmissivityIntensity; }
	
	/** Set if rim emissivity intensity property is dynamic. */
	void SetDynamicRimEmissivityIntensity(bool dynamic);
	
	/** Rim angle property is dynamic. */
	inline bool GetDynamicRimAngle() const{ return pDynamicRimAngle; }
	
	/** Set if rim angle property is dynamic. */
	void SetDynamicRimAngle(bool dynamic);
	
	/** Rim exponent property is dynamic. */
	inline bool GetDynamicRimExponent() const{ return pDynamicRimExponent; }
	void SetDynamicRimExponent(bool dynamic);
	
	/** Clip plane property is dynamic. */
	inline bool GetDynamicSkinClipPlane() const{ return pDynamicSkinClipPlane; }
	void SetDynamicSkinClipPlane(bool dynamic);
	
	/** Clip plane border property is dynamic. */
	inline bool GetDynamicSkinClipPlaneBorder() const{ return pDynamicSkinClipPlaneBorder; }
	void SetDynamicSkinClipPlaneBorder(bool dynamic);
	
	
	
	/** Color texture is used. */
	inline bool GetTextureColor() const{ return pTextureColor; }
	
	/** Set if color texture is used. */
	void SetTextureColor(bool hasTexture);
	
	/** Color tint mask texture is used. */
	inline bool GetTextureColorTintMask() const{ return pTextureColorTintMask; }
	
	/** Set if color tint mask texture is used. */
	void SetTextureColorTintMask(bool hasTexture);
	
	/** Transparency texture is used. */
	inline bool GetTextureTransparency() const{ return pTextureTransparency; }
	
	/** Set if transparency texture is used. */
	void SetTextureTransparency(bool textureTransparency);
	
	/** Solidity texture is used. */
	inline bool GetTextureSolidity() const{ return pTextureSolidity; }
	
	/** Set if solidity texture is used. */
	void SetTextureSolidity(bool textureSolidity);
	
	/** Normal texture is used. */
	inline bool GetTextureNormal() const{ return pTextureNormal; }
	
	/** Set if normal texture is used. */
	void SetTextureNormal(bool textureNormal);
	
	/** Height texture is used. */
	inline bool GetTextureHeight() const{ return pTextureHeight; }
	
	/** Set if height texture is used. */
	void SetTextureHeight(bool textureHeightCone);
	
	/** Reflectivity texture is used. */
	inline bool GetTextureReflectivity() const{ return pTextureReflectivity; }
	
	/** Set if reflectivity texture is used. */
	void SetTextureReflectivity(bool textureReflectivity);
	
	/** Roughness texture is used. */
	inline bool GetTextureRoughness() const{ return pTextureRoughness; }
	
	/** Set if roughness texture is used. */
	void SetTextureRoughness(bool textureRoughness);
	
	/** Emissivity texture is used. */
	inline bool GetTextureEmissivity() const{ return pTextureEmissivity; }
	
	/** Set if emissivity texture is used. */
	void SetTextureEmissivity(bool textureEmissivity);
	
	/** Ambient occlusion texture is used. */
	inline bool GetTextureAO() const{ return pTextureAO; }
	
	/** Set if ambient occlusion texture is used. */
	void SetTextureAO(bool textureAO);
	
	/** Environment map textures are used. */
	inline bool GetTextureEnvMap() const{ return pTextureEnvMap; }
	
	/** Set if environment map textures are used. */
	void SetTextureEnvMap(bool textureEnvMap);
	
	/** Equi environment map textures are used. */
	inline bool GetTextureEnvMapEqui() const{ return pTextureEnvMapEqui; }
	
	/** Set if equi environment map textures are used. */
	void SetTextureEnvMapEqui(bool textureEnvMapEqui);
	
	/** Absorption texture is used. */
	inline bool GetTextureAbsorption() const{ return pTextureAbsorption; }
	
	/** Set if absorption texture is used. */
	void SetTextureAbsorption(bool textureAbsorption);
	
	/** Render color texture is used. */
	inline bool GetTextureRenderColor() const{ return pTextureRenderColor; }
	
	/** Set if render color texture is used. */
	void SetTextureRenderColor(bool textureRenderColor);
	
	/** Refraction distort texture is used. */
	inline bool GetTextureRefractionDistort() const{ return pTextureRefractionDistort; }
	
	/** Set if refraction distort texture is used. */
	void SetTextureRefractionDistort(bool textureRefractionDistort);
	
	/** Environment room texture is used. */
	inline bool GetTextureEnvRoom() const{ return pTextureEnvRoom; }
	
	/** Set if environment room texture is used. */
	void SetTextureEnvRoom(bool textureEnvRoom);
	
	/** Environment room mask texture is used. */
	inline bool GetTextureEnvRoomMask() const{ return pTextureEnvRoomMask; }
	
	/** Set if environment room mask texture is used. */
	void SetTextureEnvRoomMask(bool textureEnvRoomMask);
	
	/** Environment room emissivity texture is used. */
	inline bool GetTextureEnvRoomEmissivity() const{ return pTextureEnvRoomEmissivity; }
	
	/** Set if environment room emissivity texture is used. */
	void SetTextureEnvRoomEmissivity(bool textureEnvRoomEmissivity);
	
	/** Rim emissivity texture is used. */
	inline bool GetTextureRimEmissivity() const{ return pTextureRimEmissivity; }
	
	/** Set if rim emissivity texture is used. */
	void SetTextureRimEmissivity(bool useTexture);
	
	inline bool GetTextureNonPbrAlbedo() const{ return pTextureNonPbrAlbedo; }
	void SetTextureNonPbrAlbedo(bool texture);
	
	inline bool GetTextureNonPbrMetalness() const{ return pTextureNonPbrMetalness; }
	void SetTextureNonPbrMetalness(bool texture);
	
	
	
	/** Key. */
	inline uint32_t GetKey1() const{ return pKey1; }
	inline uint32_t GetKey2() const{ return pKey2; }
	inline uint64_t GetKey3() const{ return pKey3; }
	inline uint32_t GetKey4() const{ return pKey4; }
	
	/** Update key. */
	void UpdateKey();
	/*@}*/
	
	
	
	/** \name Debug */
	/*@{*/
	/** Create short string list with the configuration. */
	void DebugGetConfigString(decString &string) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Set shader configuration from another shader configuration. */
	deoglSkinShaderConfig &operator=(const deoglSkinShaderConfig &config);
	
	/**
	 * Shader configurations are equal.
	 * \warning Before comparing configurations make sure UpdateKey() has been called on both.
	 */
	bool operator==(const deoglSkinShaderConfig &config) const;
	/*@}*/
};

#endif
