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

#ifndef _DEOGLSKINTEXTURE_H_
#define _DEOGLSKINTEXTURE_H_

#include "deoglSkinTextureProperty.h"
#include "channel/deoglSkinChannel.h"
#include "pipeline/deoglSkinTexturePipelinesList.h"
#include "shader/deoglSkinShader.h"

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglRSkin;
class deoglSPBlockUBO;
class deoglSkinShaderConfig;
class deoglSharedSPBElement;
class deoglShaderParameterBlock;

class deSkinTexture;
class deSkinProperty;



/**
 * Skin texture.
 */
class deoglSkinTexture{
public:
	/** Material parameters. */
	enum eMaterialParameters{
		empAbsorption,
		empAbsorptionHalfIntensityDistance,
		empAbsorptionRange,
		empAmbientOcclusion,
		empAmbientOcclusionSolidityMultiplier,
		empColor,
		empColorGamma,
		empColorTint,
		empColorTintMask,
		empColorSolidityMultiplier,
		empColorOmnidirCube,
		empColorOmnidirEquirect,
		empEmissivity,
		empEmissivityTint,
		empEmissivityIntensity,
		empEnvironmentRoomEmissivityIntensity,
		empEnvironmentRoomEmissivityTint,
		empEnvironmentRoomSize,
		empEnvironmentRoomOffset,
		empHeightScale,
		empHeightOffset,
		empNormal,
		empNormalStrength,
		empNormalSolidityMultiplier,
		empReflectivity,
		empReflectivityMultiplier,
		empReflectivitySolidityMultiplier,
		empRefractDistortStrength,
		empRoughness,
		empRoughnessGamma,
		empRoughnessRemapLower,
		empRoughnessRemapUpper,
		empRoughnessSolidityMultiplier,
		empTexCoordClamp,
		empTexCoordOffset,
		empTexCoordScale,
		empTexCoordRotate,
		empOmniDirRotate,
		empOmniDirRotateSpot,
		empThickness,
		empTransparency,
		empTransparencyMultiplier,
		empSolidity,
		empSolidityMultiplier,
		empVariationU,
		empVariationV,
		empOutlineColor,
		empOutlineColorTint,
		empOutlineThickness,
		empOutlineSolidity,
		empOutlineEmissivity,
		empOutlineEmissivityTint,
		empOutlineEmissivityIntensity,
		empRimEmissivity,
		empRimEmissivityTint,
		empRimEmissivityIntensity,
		empRimAngle,
		empRimExponent,
		empNonPbrAlbedo,
		empNonPbrMetalness,
		empSkinClipPlane,
		empSkinClipPlaneBorder,
		EMP_COUNT
	};
	
private:
	deoglRenderThread &pRenderThread;
	int pRTSIndex;
	
	const deoglRSkin &pSkin;
	decString pName;
	
	bool pHasSolidity;
	bool pHasZeroSolidity;
	bool pHasTransparency;
	bool pHasEmissivity;
	
	bool pSolidityMasked;
	float pSolidityFilterPriority;
	bool pSolid;
	bool pHasHoles;
	bool pReflects;
	bool pDynamicChannels;
	bool pCalculatedProperties;
	bool pConstructedProperties;
	bool pBoneProperties;
	bool pRenderableChannels;
	bool pRenderableMaterialProperties;
	deoglSkinChannel *pChannels[ deoglSkinChannel::CHANNEL_COUNT ];
	deoglSkinTexturePipelinesList pPipelines;
	deoglSharedSPBElement *pSharedSPBElement;
	
	decColor pAbsorption;
	float pAbsorptionHalfIntensityDistance;
	float pAbsorptionRange;
	float pAmbientOcclusion;
	float pAmbientOcclusionSolidityMultiplier;
	decColor pColor;
	float pColorGamma;
	decColor pColorTint;
	float pColorTintMask;
	float pColorSolidityMultiplier;
	decColor pColorOmnidirCube;
	decColor pColorOmnidirEquirect;
	float pHeightScale;
	float pHeightOffset;
	decColor pNormal;
	float pNormalStrength;
	float pNormalSolidityMultiplier;
	decColor pReflectivity;
	float pReflectivityMultiplier;
	float pReflectivitySolidityMultiplier;
	float pRefractDistortStrength;
	float pRoughness;
	float pRoughnessGamma;
	float pRoughnessRemapLower;
	float pRoughnessRemapUpper;
	float pRoughnessSolidityMultiplier;
	float pThickness;
	float pTransparency;
	float pTransparencyMultiplier;
	float pSolidity;
	float pSolidityMultiplier;
	
	bool pReflected;
	bool pMirror;
	bool pRendered;
	decColor pEmissivity;
	decColor pEmissivityTint;
	float pEmissivityIntensity;
	bool pEmissivityCameraAdapted;
	decVector2 pEnvironmentRoomSize;
	decVector pEnvironmentRoomOffset;
	decColor pEnvironmentRoomEmissivityTint;
	float pEnvironmentRoomEmissivityIntensity;
	
	bool pShadeless;
	bool pShadowNone;
	char pShadowImportance;
	bool pHintNoCompression;
	bool pHintLightBlocker;
	bool pTexCoordClamp;
	decVector2 pTexCoordOffset;
	decVector2 pTexCoordScale;
	float pTexCoordRotate;
	decVector pOmniDirRotate;
	decVector pOmniDirRotateSpot;
	
	bool pVariationU;
	bool pVariationV;
	
	int pParticleSheetCount;
	
	decColor pOutlineColor;
	decColor pOutlineColorTint;
	float pOutlineThickness;
	bool pOutlineThicknessScreen;
	float pOutlineSolidity;
	decColor pOutlineEmissivity;
	decColor pOutlineEmissivityTint;
	float pOutlineEmissivityIntensity;
	bool pHasOutline;
	bool pIsOutlineSolid;
	bool pIsOutlineEmissive;
	int pRenderTaskFilters;
	
	decColor pRimEmissivity;
	decColor pRimEmissivityTint;
	float pRimEmissivityIntensity;
	float pRimAngle;
	float pRimExponent;
	
	decColor pNonPbrAlbedo;
	float pNonPbrMetalness;
	
	bool pXRay;
	
	float pSkinClipPlane;
	float pSkinClipPlaneBorder;
	
	bool pQuickTransp;
	
	deoglSkinTextureProperty pMaterialProperties[ EMP_COUNT ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * Create skin texture.
	 * 
	 * Prepares channels and load caches. To build the actual channel texture use BuildChannels().
	 */
	deoglSkinTexture( deoglRenderThread &renderThread, deoglRSkin &skin, const deSkinTexture &texture );
	
	/** Cleans up the skin texture. */
	~deoglSkinTexture();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Render task shared index. */
	inline int GetRTSIndex() const{ return pRTSIndex; }
	
	/** Assign render task shared index. Called by deoglDelayedOperations only. */
	void AssignRTSIndex();
	
	
	
	/**
	 * Build channel textures.
	 * 
	 * Ignores channels loaded from caches and channels using already loaded shared images.
	 */
	void BuildChannels( deoglRSkin &skin, const deSkinTexture &texture );
	
	/** Finalize after asynchronous resource loading. */
	void FinalizeAsyncResLoading();
	
	
	
	/** Owner skin. */
	inline const deoglRSkin &GetSkin() const{ return pSkin; }
	
	/** Texture name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Texture contains solidity. */
	inline bool GetHasSolidity() const{ return pHasSolidity; }
	
	/** Set if texture contains solidity. */
	void SetHasSolidity( bool hasSolidity );
	
	/** Texture contains zero solidity. */
	inline bool GetHasZeroSolidity() const{ return pHasZeroSolidity; }
	
	/** Set if texture contains zero solidity. */
	void SetHasZeroSolidity( bool hasZeroSolidity );
	
	/** Texture contains transparency. */
	inline bool GetHasTransparency() const{ return pHasTransparency; }
	
	/** Set if texture contains transparency. */
	void SetHasTransparency( bool hasTransparency );
	
	/** Texture contains emissivity. */
	inline bool GetHasEmissivity() const{ return pHasEmissivity; }
	
	/** Set if texture contains emissivity. */
	void SetHasEmissivity( bool hasEmissivity );
	
	/** Determines if the texture uses masked solidity. */
	inline bool GetSolidityMasked() const{ return pSolidityMasked; }
	/** Sets if the texture uses full alpha. */
	void SetSolidityMasked( bool solidityMasked );
	
	/** Solidity filter priority. */
	inline bool GetSolidityFilterPriority() const{ return pSolidityFilterPriority; }
	
	/** Set solidity filter priority. */
	void SetSolidityFilterPriority( float solidityFilterPriority );
	
	/** Determines if the texture is solid. */
	inline bool GetSolid() const{ return pSolid; }
	/** Sets if the texture is solid. */
	void SetSolid( bool solid );
	/** Determines if the texture has holes. */
	inline bool GetHasHoles() const{ return pHasHoles; }
	/** Sets if the texture has holes. */
	void SetHasHoles( bool hasHoles );
	/** Determines if the texture reflects. */
	inline bool GetReflects() const{ return pReflects; }
	/** Sets if the texture reflects. */
	void SetReflects( bool reflects );
	
	/** Determines if quick transparency can to be used for this texture. */
	inline bool GetQuickTransparency() const{ return pQuickTransp; }
	/** Sets if quick transparency can be used for this texture. */
	void SetQuickTransparency( bool quickTransp );
	
	/** Texture has dynamic channels. */
	inline bool GetDynamicChannels() const{ return pDynamicChannels; }
	
	/** Set if texture has dynamic channels. */
	void SetDynamicChannels( bool dynamicChannels );
	
	/** Texture has calculated properties. */
	inline bool GetCalculatedProperties() const{ return pCalculatedProperties; }
	void SetCalculatedProperties( bool calculatedProperties );
	
	/** Texture has constructed properties. */
	inline bool GetConstructedProperties() const{ return pConstructedProperties; }
	void SetConstructedProperties( bool constructedProperties );
	
	/** Texture has bone properties. */
	inline bool GetBoneProperties() const{ return pBoneProperties; }
	void SetBoneProperties( bool boneProperties );
	
	/** Texture has renderable channels. */
	inline bool GetRenderableChannels() const{ return pRenderableChannels; }
	
	/** Set if texture has renderable channels. */
	void SetRenderableChannels( bool renderableChannels );
	
	/** Texture has renderable material properties. */
	inline bool GetRenderableMaterialProperties() const{ return pRenderableMaterialProperties; }
	
	/** Set if texture has renderable material properties. */
	void SetRenderableMaterialProperties( bool renderableMaterialProperties );
	
	/** Pipelines list. */
	inline deoglSkinTexturePipelinesList &GetPipelines(){ return pPipelines; }
	inline const deoglSkinTexturePipelinesList &GetPipelines() const{ return pPipelines; }
	
	/** Shared shader parameter block element. */
	inline deoglSharedSPBElement *GetSharedSPBElement() const{ return pSharedSPBElement; }
	
	/** Prepare parameter block. */
	void PrepareParamBlock();
	/*@}*/
	
	/** \name Material Properties */
	/*@{*/
	/** Color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** Set color. */
	void SetColor( const decColor &color );
	
	/** Retrieves the color gamma correction. */
	inline float GetColorGamma() const{ return pColorGamma; }
	/** Sets the color gamma correction. */
	void SetColorGamma( float gamma );
	
	/** Color tint. */
	inline const decColor &GetColorTint() const{ return pColorTint; }
	
	/** Set color tint. */
	void SetColorTint( const decColor &tint );
	
	/** Color tint mask. */
	inline float GetColorTintMask() const{ return pColorTintMask; }
	
	/** Set color tint mask. */
	void SetColorTintMask( float mask );
	
	/** Retrieves the color solidity multiplier. */
	inline float GetColorSolidityMultiplier() const{ return pColorSolidityMultiplier; }
	/** Sets the color solidity multiplier. */
	void SetColorSolidityMultiplier( float multiplier );
	
	/** Omni-directional color cube map. */
	inline const decColor &GetColorOmnidirCube() const{ return pColorOmnidirCube; }
	
	/** Set omni-directional color cube map. */
	void SetColorOmnidirCube( const decColor &color );
	
	/** Omni-directional color equirectangular map. */
	inline const decColor &GetColorOmnidirEquirect() const{ return pColorOmnidirEquirect; }
	
	/** Set omni-directional color equirectangular map. */
	void SetColorOmnidirEquirect( const decColor &color );
	
	/** Retrieves the transparency. */
	inline float GetTransparency() const{ return pTransparency; }
	/** Sets the transparency. */
	void SetTransparency( float transparency );
	/** Retrieves the transparency multiplier. */
	inline float GetTransparencyMultiplier() const{ return pTransparencyMultiplier; }
	/** Sets the transparency multiplier. */
	void SetTransparencyMultiplier( float multiplier );
	
	/** Retrieves the solidity. */
	inline float GetSolidity() const{ return pSolidity; }
	/** Sets the solidity. */
	void SetSolidity( float solidity );
	/** Retrieves the solidity multiplier. */
	inline float GetSolidityMultiplier() const{ return pSolidityMultiplier; }
	/** Sets the solidity multiplier. */
	void SetSolidityMultiplier( float multiplier );
	
	/** Retrieves the ambient occlusion. */
	inline float GetAmbientOcclusion() const{ return pAmbientOcclusion; }
	/** Sets the ambient occlusion. */
	void SetAmbientOcclusion( float ao );
	/** Retrieves the ambient occlusion solidity multiplier. */
	inline float GetAmbientOcclusionSolidityMultiplier() const{ return pAmbientOcclusionSolidityMultiplier; }
	/** Sets the ambient occlusion solidity multiplier. */
	void SetAmbientOcclusionSolidityMultiplier( float multiplier );
	
	/** Retrieves the height scale. */
	inline float GetHeightScale() const{ return pHeightScale; }
	/** Sets the height scale. */
	void SetHeightScale( float scale );
	/** Retrieves the height offset. */
	inline float GetHeightOffset() const{ return pHeightOffset; }
	/** Sets the height offset. */
	void SetHeightOffset( float offset );
	
	/** Retrieves the normal. */
	inline const decColor &GetNormal() const{ return pNormal; }
	/** Sets the normal. */
	void SetNormal( const decColor &normal );
	/** Retrieves the normal map strength. */
	inline float GetNormalStrength() const{ return pNormalStrength; }
	/** Sets the normal map strength. */
	void SetNormalStrength( float strength );
	/** Retrieves the normal solidity multiplier. */
	inline float GetNormalSolidityMultiplier() const{ return pNormalSolidityMultiplier; }
	/** Sets the normal solidity multiplier. */
	void SetNormalSolidityMultiplier( float multiplier );
	
	/** Retrieves the surface roughness. */
	inline float GetRoughness() const{ return pRoughness; }
	/** Sets the surface roughness. */
	void SetRoughness( float roughness );
	/** Retrieves the surface roughness lower remapping value. */
	inline float GetRoughnessRemapLower() const{ return pRoughnessRemapLower; }
	/** Sets the surface roughness lower remapping value. */
	void SetRoughnessRemapLower( float value );
	/** Retrieves the surface roughness upper remapping value. */
	inline float GetRoughnessRemapUpper() const{ return pRoughnessRemapUpper; }
	/** Sets the surface roughness upper remapping value. */
	void SetRoughnessRemapUpper( float value );
	/** Retrieves the surface roughness gamma correction. */
	inline float GetRoughnessGamma() const{ return pRoughnessGamma; }
	/** Sets the surface roughness gamma correction. */
	void SetRoughnessGamma( float gamma );
	/** Retrieves the roughness solidity multiplier. */
	inline float GetRoughnessSolidityMultiplier() const{ return pRoughnessSolidityMultiplier; }
	/** Sets the roughness solidity multiplier. */
	void SetRoughnessSolidityMultiplier( float multiplier );
	
	/** Retrieves the refraction distortion strength. */
	inline float GetRefractDistortStrength() const{ return pRefractDistortStrength; }
	/** Sets the refraction distortion strength. */
	void SetRefractDistortStrength( float strength );
	
	/** Retrieves the reflectivity. */
	inline const decColor &GetReflectivity() const{ return pReflectivity; }
	/** Sets the reflectivity. */
	void SetReflectivity( const decColor &reflectivity );
	/** Retrieves the reflectivity multiplier. */
	inline float GetReflectivityMultiplier() const{ return pReflectivityMultiplier; }
	/** Sets the reflectivity multiplier. */
	void SetReflectivityMultiplier( float multiplier );
	/** Retrieves the reflectivity solidity multiplier. */
	inline float GetReflectivitySolidityMultiplier() const{ return pReflectivitySolidityMultiplier; }
	/** Sets the reflectivity solidity multiplier. */
	void SetReflectivitySolidityMultiplier( float multiplier );
	/** Determines if this texture is a perfect mirror. */
	inline bool GetMirror() const{ return pMirror; }
	/** Sets if this texture is a perfect mirror. */
	void SetMirror( bool mirror );
	
	/** Texture is render. */
	inline bool GetRendered() const{ return pRendered; }
	
	/** Set if texture is rendered. */
	void SetRendered( bool rendered );
	
	/** Determines if the texture is reflected by other geometry. */
	inline bool GetReflected() const{ return pReflected; }
	/** Sets if the texture is reflected by other geometry. */
	void SetReflected( bool reflected );
	
	
	
	/** Emissivity. */
	inline const decColor &GetEmissivity() const{ return pEmissivity; }
	
	/** Set emissivity. */
	void SetEmissivity( const decColor &emissivity );
	
	/** Emissivity tint. */
	inline const decColor &GetEmissivityTint() const{ return pEmissivityTint; }
	
	/** Set emissivity tint. */
	void SetEmissivityTint( const decColor &tint );
	
	/** Emissivity intensity. */
	inline float GetEmissivityIntensity() const{ return pEmissivityIntensity; }
	
	/** Set emissivity intensity. */
	void SetEmissivityIntensity( float intensity );
	
	/** Emissivity camera adapted. */
	inline bool GetEmissivityCameraAdapted() const{ return pEmissivityCameraAdapted; }
	
	/** Set emissivity camera adapted. */
	void SetEmissivityCameraAdapted( bool cameraAdapted );
	
	
	
	/** Environment room size. */
	inline const decVector2 &GetEnvironmentRoomSize() const{ return pEnvironmentRoomSize; }
	
	/** Set environment room size. */
	void SetEnvironmentRoomSize( const decVector2 &size );
	
	/** Environment room offset. */
	inline const decVector &GetEnvironmentRoomOffset() const{ return pEnvironmentRoomOffset; }
	
	/** Set environment room offset. */
	void SetEnvironmentRoomOffset( const decVector &offset );
	
	/** Environment room emissivity tint. */
	inline const decColor &GetEnvironmentRoomEmissivityTint() const{ return pEnvironmentRoomEmissivityTint; }
	
	/** Set environment room emissivity tint. */
	void SetEnvironmentRoomEmissivityTint( const decColor &tint );
	
	/** Environment room emissivity intensity. */
	inline float GetEnvironmentRoomEmissivityIntensity() const{ return pEnvironmentRoomEmissivityIntensity; }
	
	/** Set environment room emissivity intensity. */
	void SetEnvironmentRoomEmissivityIntensity( float intensity );
	
	
	
	/** Retrieves the thickness. */
	inline float GetThickness() const{ return pThickness; }
	/** Sets the thickness. */
	void SetThickness( float thickness );
	/** Retrieves the absorption. */
	inline const decColor &GetAbsorption() const{ return pAbsorption; }
	/** Sets the absorption. */
	void SetAbsorption( const decColor &absorption );
	/** Retrieves the absorption range. */
	inline float GetAbsorptionRange() const{ return pAbsorptionRange; }
	/** Sets the absorption range. */
	void SetAbsorptionRange( float range );
	/** Retrieves the absorption half intensity distance. */
	inline float GetAbsorptionHalfIntensityDistance() const{ return pAbsorptionHalfIntensityDistance; }
	/** Sets the absorption half intensity distance. */
	void SetAbsorptionHalfIntensityDistance( float distance );
	
	/** Rim emissivity. */
	inline const decColor &GetRimEmissivity() const{ return pRimEmissivity; }
	
	/** Set rim emissivity. */
	void SetRimEmissivity( const decColor &emissivity );
	
	/** Rim emissivity tint. */
	inline const decColor &GetRimEmissivityTint() const{ return pRimEmissivityTint; }
	
	/** Set rim emissivity tint. */
	void SetRimEmissivityTint( const decColor &emissivity );
	
	/** Rim emissivity intensity. */
	inline float GetRimEmissivityIntensity() const{ return pRimEmissivityIntensity; }
	
	/** Set rim emissivity intensity. */
	void SetRimEmissivityIntensity( float intensity );
	
	/** Rim angle. */
	inline float GetRimAngle() const{ return pRimAngle; }
	
	/** Set rim angle. */
	void SetRimAngle( float angle );
	
	/** Rim exponent. */
	inline float GetRimExponent() const{ return pRimExponent; }
	void SetRimExponent( float exponent );
	
	/** Clip plane. */
	inline float GetSkinClipPlane() const{ return pSkinClipPlane; }
	void SetSkinClipPlane( float clipPlane );
	
	/** Clip plane border. */
	inline float GetSkinClipPlaneBorder() const{ return pSkinClipPlaneBorder; }
	void SetSkinClipPlaneBorder( float border );
	
	/** Determines if the texture is shadeless. */
	inline bool GetShadeless() const{ return pShadeless; }
	/** Sets if the texture is shadeless. */
	void SetShadeless( bool shadeless );
	
	/** Determines if the texture casts no shadows. */
	inline bool GetShadowNone() const{ return pShadowNone; }
	/** Sets if the texture casts no shadows. */
	void SetShadowNone( bool shadowNone );
	/** Retrieves the shadow importance level. */
	inline int GetShadowImportance() const{ return ( int )pShadowImportance; }
	/** Sets the shadow importance level. */
	void SetShadowImportance( int importance );
	
	/** Determines if the texture should not use compression. */
	inline bool GetHintNoCompression() const{ return pHintNoCompression; }
	/** Sets if the texture should not use compression. */
	void SetHintNoCompression( bool hintNoCompression );
	/** Determines if the texture is a light blocker. */
	inline bool GetHintLightBlocker() const{ return pHintLightBlocker; }
	/** Sets if the texture is a light blocker. */
	void SetHintLightBlocker( bool hintLightBlocker );
	
	/** Determines if the texture coordinates are clamped. */
	inline bool GetTexCoordClamp() const{ return pTexCoordClamp; }
	/** Sets if the texture coordinates are clamped. */
	void SetTexCoordClamp( bool clamp );
	
	/** Texture coordinates offset. */
	inline const decVector2 &GetTexCoordOffset() const{ return pTexCoordOffset; }
	
	/** Set texture coordinates offset. */
	void SetTexCoordOffset( const decVector2 &offset );
	
	/** Texture coordinates scale. */
	inline const decVector2 &GetTexCoordScale() const{ return pTexCoordScale; }
	
	/** Set texture coordinates scale. */
	void SetTexCoordScale( const decVector2 &scale );
	
	/** Texture coordinates rotate. */
	inline float GetTexCoordRotate() const{ return pTexCoordRotate; }
	
	/** Set texture coordinates rotate. */
	void SetTexCoordRotate( float rotate );
	
	/** Omni directional texture rotate. */
	inline const decVector &GetOmniDirRotate() const{ return pOmniDirRotate; }
	
	/** Set omni directional texture rotate. */
	void SetOmniDirRotate( const decVector &rotate );
	
	/** Omni directional texture rotate for spot lights only. */
	inline const decVector &GetOmniDirRotateSpot() const{ return pOmniDirRotateSpot; }
	
	/** Set omni directional texture rotate for spot lights only. */
	void SetOmniDirRotateSpot( const decVector &rotate );
	
	
	
	/** Texture has variation in U direction. */
	inline bool GetVariationU() const{ return pVariationU; }
	
	/** Set if texture has variation in U direction. */
	void SetVariationU( bool enable );
	
	/** Texture has variation in V direction. */
	inline bool GetVariationV() const{ return pVariationV; }
	
	/** Set if texture has variation in V direction. */
	void SetVariationV( bool enable );
	
	
	
	/** Particle sheets count. */
	inline int GetParticleSheetCount() const{ return pParticleSheetCount; }
	
	/** Set particle sheets count. */
	void SetParticleSheetCount( int count );
	
	
	
	/** Outline color. */
	inline const decColor &GetOutlineColor() const{ return pOutlineColor; }
	
	/** Set outline color. */
	void SetOutlineColor( const decColor &color );
	
	/** Outline color tint. */
	inline const decColor &GetOutlineColorTint() const{ return pOutlineColorTint; }
	
	/** Set outline color tint. */
	void SetOutlineColorTint( const decColor &color );
	
	/** Outline thickness. */
	inline float GetOutlineThickness() const{ return pOutlineThickness; }
	
	/** Set outline thickness. */
	void SetOutlineThickness( float thickness );
	
	/** Outline thickness is relative to screen width. */
	inline bool GetOutlineThicknessScreen() const{ return pOutlineThicknessScreen; }
	
	/** Set if outline thickness is relative to screen width. */
	void SetOutlineThicknessScreen( bool enable );
	
	/** Outline solidity. */
	inline float GetOutlineSolidity() const{ return pOutlineSolidity; }
	
	/** Set outline solidity. */
	void SetOutlineSolidity( float solidity );
	
	/** Outline emissivity. */
	inline const decColor &GetOutlineEmissivity() const{ return pOutlineEmissivity; }
	
	/** Set outline emissivity. */
	void SetOutlineEmissivity( const decColor &emissivity );
	
	/** Outline emissivity tint. */
	inline const decColor &GetOutlineEmissivityTint() const{ return pOutlineEmissivityTint; }
	
	/** Set outline emissivity tint. */
	void SetOutlineEmissivityTint( const decColor &emissivity );
	
	/** Outline emissivity intensity. */
	inline float GetOutlineEmissivityIntensity() const{ return pOutlineEmissivityIntensity; }
	
	/** Set outline emissivity intensity. */
	void SetOutlineEmissivityIntensity( float intensity );
	
	
	
	inline const decColor &GetNonPbrAlbeod() const{ return pNonPbrAlbedo; }
	void SetNonPbrAlbedo( const decColor &albedo );
	
	inline float GetNonPbrMetalness() const{ return pNonPbrMetalness; }
	void SetNonPbrMetalness( float metalness );
	
	inline bool GetXRay() const{ return pXRay; }
	void SetXRay( bool xray );
	
	
	
	/** Has outline. */
	inline bool GetHasOutline() const{ return pHasOutline; }
	
	/** Outline is solid. */
	inline bool GetIsOutlineSolid() const{ return pIsOutlineSolid; }
	
	/** Outline is emissive. */
	inline bool GetIsOutlineEmissive() const{ return pIsOutlineEmissive; }
	
	/** Render task filters. */
	inline int GetRenderTaskFilters() const{ return pRenderTaskFilters; }
	
	
	/** Retrieves a material property. */
	deoglSkinTextureProperty &GetMaterialPropertyAt( int property );
	const deoglSkinTextureProperty &GetMaterialPropertyAt( int property ) const;
	
	/** Channel for type or \em NULL if not used. */
	deoglSkinChannel *GetChannelAt( deoglSkinChannel::eChannelTypes type ) const;
	
	/** Channel for type is not \em NULL. */
	bool IsChannelEnabled( deoglSkinChannel::eChannelTypes type ) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pPrepareChannels( deoglRSkin &skin, const deSkinTexture &texture );
	void pLoadCached( deoglRSkin &skin );
	void pCreateMipMaps();
	void pCompressTextures( deoglRSkin &skin, const deSkinTexture &texture );
	void pWriteCached( deoglRSkin &skin );
	void pProcessProperty( deoglRSkin &skin, deSkinProperty &property );
	void pUpdateParamBlock( deoglShaderParameterBlock &paramBlock, int index );
	void pUpdateRenderTaskFilters();
};

#endif
