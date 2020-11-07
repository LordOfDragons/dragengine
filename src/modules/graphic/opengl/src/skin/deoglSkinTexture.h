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

#ifndef _DEOGLSKINTEXTURE_H_
#define _DEOGLSKINTEXTURE_H_

#include "deoglSkinTextureProperty.h"
#include "channel/deoglSkinChannel.h"

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglRSkin;
class deoglSPBlockUBO;
class deoglSkinShader;
class deoglSkinShaderConfig;

class deSkinTexture;
class deSkinProperty;



/**
 * \brief Skin texture.
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
		EMP_COUNT
	};
	
	/** Shader Types. */
	enum eShaderTypes{
		estComponentGeometry,
		estComponentDepth,
		estComponentDepthClipPlane,
		estComponentDepthReversed,
		estComponentDepthClipPlaneReversed,
		estComponentCounter,
		estComponentCounterClipPlane,
		estComponentShadowProjection,
		estComponentShadowOrthogonal,
		estComponentShadowDistance,
		estComponentShadowDistanceCube,
		estComponentEnvMap,
		estComponentLuminance,
		
		estBillboardGeometry,
		estBillboardDepth,
		estBillboardDepthClipPlane,
		estBillboardDepthReversed,
		estBillboardDepthClipPlaneReversed,
		estBillboardCounter,
		estBillboardCounterClipPlane,
		estBillboardEnvMap,
		
		estDecalGeometry,
		estDecalEnvMap,
		
		estPropFieldGeometry,
		estPropFieldImposterGeometry,
		estPropFieldDepth,
		estPropFieldImposterDepth,
		estPropFieldDepthClipPlane,
		estPropFieldImposterDepthClipPlane,
		estPropFieldDepthReversed,
		estPropFieldImposterDepthReversed,
		estPropFieldDepthClipPlaneReversed,
		estPropFieldImposterDepthClipPlaneReversed,
		estPropFieldCounter,
		estPropFieldCounterClipPlane,
		estPropFieldShadowProjection,
		estPropFieldShadowOrthogonal,
		estPropFieldShadowDistance,
		estPropFieldEnvMap,
		
		estHeightMapGeometry,
		estHeightMapDepth,
		estHeightMapDepthClipPlane,
		estHeightMapDepthReversed,
		estHeightMapDepthClipPlaneReversed,
		estHeightMapTranspCount,
		estHeightMapTranspCountClipPlane,
		estHeightMapShadowProjection,
		estHeightMapShadowOrthogonal,
		estHeightMapShadowDistance,
		estHeightMapEnvMap,
		estHeightMapLuminance,
		
		estParticleGeometry,
		estParticleGeometryDepthTest,
		estParticleDepth,
		estParticleDepthClipPlane,
		estParticleDepthReversed,
		estParticleDepthClipPlaneReversed,
		estParticleShadowProjection,
		estParticleShadowOrthogonal,
		estParticleShadowDistance,
		estParticleCounter,
		estParticleCounterClipPlane,
		estParticleRibbonGeometry,
		estParticleRibbonGeometryDepthTest,
		estParticleRibbonDepth,
		estParticleRibbonDepthClipPlane,
		estParticleRibbonDepthReversed,
		estParticleRibbonDepthClipPlaneReversed,
		estParticleRibbonCounter,
		estParticleRibbonCounterClipPlane,
		estParticleBeamGeometry,
		estParticleBeamGeometryDepthTest,
		estParticleBeamDepth,
		estParticleBeamDepthClipPlane,
		estParticleBeamDepthReversed,
		estParticleBeamDepthClipPlaneReversed,
		estParticleBeamCounter,
		estParticleBeamCounterClipPlane,
		
		estOutlineGeometry,
		estOutlineDepth,
		estOutlineDepthClipPlane,
		estOutlineDepthReversed,
		estOutlineDepthClipPlaneReversed,
		estOutlineCounter,
		estOutlineCounterClipPlane,
	};
	
	static const int ShaderTypeCount = estOutlineCounterClipPlane + 1;
	
private:
	deoglRenderThread &pRenderThread;
	
	decString pName;
	
	bool pHasSolidity;
	bool pHasZeroSolidity;
	bool pHasTransparency;
	bool pHasEmissivity;
	
	bool pSolidityMasked;
	bool pSolid;
	bool pHasHoles;
	bool pReflects;
	bool pDynamicChannels;
	bool pCalculatedProperties;
	bool pRenderableChannels;
	bool pRenderableMaterialProperties;
	deoglSkinChannel *pChannels[ deoglSkinChannel::CHANNEL_COUNT ];
	deoglSkinShader *pShaders[ ShaderTypeCount ];
	deoglSPBlockUBO *pParamBlocks[ ShaderTypeCount ];
	
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
	
	decColor pRimEmissivity;
	decColor pRimEmissivityTint;
	float pRimEmissivityIntensity;
	float pRimAngle;
	float pRimExponent;
	
	bool pQuickTransp;
	
	deoglSkinTextureProperty pMaterialProperties[ EMP_COUNT ];
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create skin texture.
	 * 
	 * Prepares channels and load caches. To build the actual channel texture use BuildChannels().
	 */
	deoglSkinTexture( deoglRenderThread &renderThread, deoglRSkin &skin, const deSkinTexture &texture );
	
	/** Cleans up the skin texture. */
	~deoglSkinTexture();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/**
	 * \brief Build channel textures.
	 * 
	 * Ignores channels loaded from caches and channels using already loaded shared images.
	 */
	void BuildChannels( deoglRSkin &skin, const deSkinTexture &texture );
	
	/** \brief Finalize after asynchronous resource loading. */
	void FinalizeAsyncResLoading();
	
	
	
	/** \brief Texture name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Texture contains solidity. */
	inline bool GetHasSolidity() const{ return pHasSolidity; }
	
	/** \brief Set if texture contains solidity. */
	void SetHasSolidity( bool hasSolidity );
	
	/** \brief Texture contains zero solidity. */
	inline bool GetHasZeroSolidity() const{ return pHasZeroSolidity; }
	
	/** \brief Set if texture contains zero solidity. */
	void SetHasZeroSolidity( bool hasZeroSolidity );
	
	/** \brief Texture contains transparency. */
	inline bool GetHasTransparency() const{ return pHasTransparency; }
	
	/** \brief Set if texture contains transparency. */
	void SetHasTransparency( bool hasTransparency );
	
	/** \brief Texture contains emissivity. */
	inline bool GetHasEmissivity() const{ return pHasEmissivity; }
	
	/** \brief Set if texture contains emissivity. */
	void SetHasEmissivity( bool hasEmissivity );
	
	/** Determines if the texture uses masked solidity. */
	inline bool GetSolidityMasked() const{ return pSolidityMasked; }
	/** Sets if the texture uses full alpha. */
	void SetSolidityMasked( bool solidityMasked );
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
	
	/** \brief Texture has dynamic channels. */
	inline bool GetDynamicChannels() const{ return pDynamicChannels; }
	
	/** \brief Set if texture has dynamic channels. */
	void SetDynamicChannels( bool dynamicChannels );
	
	/** \brief Texture has calculated properties. */
	inline bool GetCalculatedProperties() const{ return pCalculatedProperties; }
	
	/** \brief Set if texture has calculated properties. */
	void SetCalculatedProperties( bool calculatedProperties );
	
	/** \brief Texture has renderable channels. */
	inline bool GetRenderableChannels() const{ return pRenderableChannels; }
	
	/** \brief Set if texture has renderable channels. */
	void SetRenderableChannels( bool renderableChannels );
	
	/** \brief Texture has renderable material properties. */
	inline bool GetRenderableMaterialProperties() const{ return pRenderableMaterialProperties; }
	
	/** \brief Set if texture has renderable material properties. */
	void SetRenderableMaterialProperties( bool renderableMaterialProperties );
	
	/** Retrieves the shader for a shader type. */
	deoglSkinShader *GetShaderFor( eShaderTypes shaderType );
	/** Retrieves the shader configuration for a shader type. */
	bool GetShaderConfigFor( eShaderTypes shaderType, deoglSkinShaderConfig &config );
	/** Retrieves the parameter block for a shader type for static texture properties shared across components. */
	deoglSPBlockUBO *GetParameterBlockFor( eShaderTypes shaderType );
	/*@}*/
	
	/** @name Material Properties */
	/*@{*/
	/** \brief Color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set color. */
	void SetColor( const decColor &color );
	
	/** Retrieves the color gamma correction. */
	inline float GetColorGamma() const{ return pColorGamma; }
	/** Sets the color gamma correction. */
	void SetColorGamma( float gamma );
	
	/** \brief Color tint. */
	inline const decColor &GetColorTint() const{ return pColorTint; }
	
	/** \brief Set color tint. */
	void SetColorTint( const decColor &tint );
	
	/** \brief Color tint mask. */
	inline float GetColorTintMask() const{ return pColorTintMask; }
	
	/** \brief Set color tint mask. */
	void SetColorTintMask( float mask );
	
	/** Retrieves the color solidity multiplier. */
	inline float GetColorSolidityMultiplier() const{ return pColorSolidityMultiplier; }
	/** Sets the color solidity multiplier. */
	void SetColorSolidityMultiplier( float multiplier );
	
	/** \brief Omni-directional color cube map. */
	inline const decColor &GetColorOmnidirCube() const{ return pColorOmnidirCube; }
	
	/** \brief Set omni-directional color cube map. */
	void SetColorOmnidirCube( const decColor &color );
	
	/** \brief Omni-directional color equirectangular map. */
	inline const decColor &GetColorOmnidirEquirect() const{ return pColorOmnidirEquirect; }
	
	/** \brief Set omni-directional color equirectangular map. */
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
	
	/** \brief Retrieves the height scale. */
	inline float GetHeightScale() const{ return pHeightScale; }
	/** \brief Sets the height scale. */
	void SetHeightScale( float scale );
	/** \brief Retrieves the height offset. */
	inline float GetHeightOffset() const{ return pHeightOffset; }
	/** \brief Sets the height offset. */
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
	
	/** \brief Texture is render. */
	inline bool GetRendered() const{ return pRendered; }
	
	/** \brief Set if texture is rendered. */
	void SetRendered( bool rendered );
	
	/** Determines if the texture is reflected by other geometry. */
	inline bool GetReflected() const{ return pReflected; }
	/** Sets if the texture is reflected by other geometry. */
	void SetReflected( bool reflected );
	
	
	
	/** \brief Emissivity. */
	inline const decColor &GetEmissivity() const{ return pEmissivity; }
	
	/** \brief Set emissivity. */
	void SetEmissivity( const decColor &emissivity );
	
	/** \brief Emissivity tint. */
	inline const decColor &GetEmissivityTint() const{ return pEmissivityTint; }
	
	/** \brief Set emissivity tint. */
	void SetEmissivityTint( const decColor &tint );
	
	/** \brief Emissivity intensity. */
	inline float GetEmissivityIntensity() const{ return pEmissivityIntensity; }
	
	/** \brief Set emissivity intensity. */
	void SetEmissivityIntensity( float intensity );
	
	/** \brief Emissivity camera adapted. */
	inline bool GetEmissivityCameraAdapted() const{ return pEmissivityCameraAdapted; }
	
	/** \brief Set emissivity camera adapted. */
	void SetEmissivityCameraAdapted( bool cameraAdapted );
	
	
	
	/** \brief Environment room size. */
	inline const decVector2 &GetEnvironmentRoomSize() const{ return pEnvironmentRoomSize; }
	
	/** \brief Set environment room size. */
	void SetEnvironmentRoomSize( const decVector2 &size );
	
	/** \brief Environment room offset. */
	inline const decVector &GetEnvironmentRoomOffset() const{ return pEnvironmentRoomOffset; }
	
	/** \brief Set environment room offset. */
	void SetEnvironmentRoomOffset( const decVector &offset );
	
	/** \brief Environment room emissivity tint. */
	inline const decColor &GetEnvironmentRoomEmissivityTint() const{ return pEnvironmentRoomEmissivityTint; }
	
	/** \brief Set environment room emissivity tint. */
	void SetEnvironmentRoomEmissivityTint( const decColor &tint );
	
	/** \brief Environment room emissivity intensity. */
	inline float GetEnvironmentRoomEmissivityIntensity() const{ return pEnvironmentRoomEmissivityIntensity; }
	
	/** \brief Set environment room emissivity intensity. */
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
	
	/** \brief Rim emissivity. */
	inline const decColor &GetRimEmissivity() const{ return pRimEmissivity; }
	
	/** \brief Set rim emissivity. */
	void SetRimEmissivity( const decColor &emissivity );
	
	/** \brief Rim emissivity tint. */
	inline const decColor &GetRimEmissivityTint() const{ return pRimEmissivityTint; }
	
	/** \brief Set rim emissivity tint. */
	void SetRimEmissivityTint( const decColor &emissivity );
	
	/** \brief Rim emissivity intensity. */
	inline float GetRimEmissivityIntensity() const{ return pRimEmissivityIntensity; }
	
	/** \brief Set rim emissivity intensity. */
	void SetRimEmissivityIntensity( float intensity );
	
	/** \brief Rim angle. */
	inline float GetRimAngle() const{ return pRimAngle; }
	
	/** \brief Set rim angle. */
	void SetRimAngle( float angle );
	
	/** \brief Rim exponent. */
	inline float GetRimExponent() const{ return pRimExponent; }
	
	/** \brief Set rim exponent. */
	void SetRimExponent( float exponent );
	
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
	
	/** \brief Texture coordinates offset. */
	inline const decVector2 &GetTexCoordOffset() const{ return pTexCoordOffset; }
	
	/** \brief Set texture coordinates offset. */
	void SetTexCoordOffset( const decVector2 &offset );
	
	/** \brief Texture coordinates scale. */
	inline const decVector2 &GetTexCoordScale() const{ return pTexCoordScale; }
	
	/** \brief Set texture coordinates scale. */
	void SetTexCoordScale( const decVector2 &scale );
	
	/** \brief Texture coordinates rotate. */
	inline float GetTexCoordRotate() const{ return pTexCoordRotate; }
	
	/** \brief Set texture coordinates rotate. */
	void SetTexCoordRotate( float rotate );
	
	/** \brief Omni directional texture rotate. */
	inline const decVector &GetOmniDirRotate() const{ return pOmniDirRotate; }
	
	/** \brief Set omni directional texture rotate. */
	void SetOmniDirRotate( const decVector &rotate );
	
	/** \brief Omni directional texture rotate for spot lights only. */
	inline const decVector &GetOmniDirRotateSpot() const{ return pOmniDirRotateSpot; }
	
	/** \brief Set omni directional texture rotate for spot lights only. */
	void SetOmniDirRotateSpot( const decVector &rotate );
	
	
	
	/** \brief Texture has variation in U direction. */
	inline bool GetVariationU() const{ return pVariationU; }
	
	/** \brief Set if texture has variation in U direction. */
	void SetVariationU( bool enable );
	
	/** \brief Texture has variation in V direction. */
	inline bool GetVariationV() const{ return pVariationV; }
	
	/** \brief Set if texture has variation in V direction. */
	void SetVariationV( bool enable );
	
	
	
	/** \brief Particle sheets count. */
	inline int GetParticleSheetCount() const{ return pParticleSheetCount; }
	
	/** \brief Set particle sheets count. */
	void SetParticleSheetCount( int count );
	
	
	
	/** \brief Outline color. */
	inline const decColor &GetOutlineColor() const{ return pOutlineColor; }
	
	/** \brief Set outline color. */
	void SetOutlineColor( const decColor &color );
	
	/** \brief Outline color tint. */
	inline const decColor &GetOutlineColorTint() const{ return pOutlineColorTint; }
	
	/** \brief Set outline color tint. */
	void SetOutlineColorTint( const decColor &color );
	
	/** \brief Outline thickness. */
	inline float GetOutlineThickness() const{ return pOutlineThickness; }
	
	/** \brief Set outline thickness. */
	void SetOutlineThickness( float thickness );
	
	/** \brief Outline thickness is relative to screen width. */
	inline bool GetOutlineThicknessScreen() const{ return pOutlineThicknessScreen; }
	
	/** \brief Set if outline thickness is relative to screen width. */
	void SetOutlineThicknessScreen( bool enable );
	
	/** \brief Outline solidity. */
	inline float GetOutlineSolidity() const{ return pOutlineSolidity; }
	
	/** \brief Set outline solidity. */
	void SetOutlineSolidity( float solidity );
	
	/** \brief Outline emissivity. */
	inline const decColor &GetOutlineEmissivity() const{ return pOutlineEmissivity; }
	
	/** \brief Set outline emissivity. */
	void SetOutlineEmissivity( const decColor &emissivity );
	
	/** \brief Outline emissivity tint. */
	inline const decColor &GetOutlineEmissivityTint() const{ return pOutlineEmissivityTint; }
	
	/** \brief Set outline emissivity tint. */
	void SetOutlineEmissivityTint( const decColor &emissivity );
	
	/** \brief Outline emissivity intensity. */
	inline float GetOutlineEmissivityIntensity() const{ return pOutlineEmissivityIntensity; }
	
	/** \brief Set outline emissivity intensity. */
	void SetOutlineEmissivityIntensity( float intensity );
	
	/** \brief Has outline. */
	inline bool GetHasOutline() const{ return pHasOutline; }
	
	/** \brief Outline is solid. */
	inline bool GetIsOutlineSolid() const{ return pIsOutlineSolid; }
	
	/** \brief Outline is emissive. */
	inline bool GetIsOutlineEmissive() const{ return pIsOutlineEmissive; }
	
	
	
	/** Retrieves a material property. */
	deoglSkinTextureProperty &GetMaterialPropertyAt( int property );
	const deoglSkinTextureProperty &GetMaterialPropertyAt( int property ) const;
	
	/** \brief Channel for type or \em NULL if not used. */
	deoglSkinChannel *GetChannelAt( deoglSkinChannel::eChannelTypes type ) const;
	
	/** \brief Channel for type is not \em NULL. */
	bool IsChannelEnabled( deoglSkinChannel::eChannelTypes type ) const;
	
	
	
	/** \brief Drop objects containing delayed deletion support. */
	void DropDelayedDeletionObjects();
	/*@}*/
	
private:
	void pCleanUp();
	
	void pPrepareChannels( deoglRSkin &skin, const deSkinTexture &texture );
	void pLoadCached( deoglRSkin &skin );
	void pCreateMipMaps();
	void pCompressTextures( deoglRSkin &skin, const deSkinTexture &texture );
	void pWriteCached( deoglRSkin &skin );
	void pProcessProperty( deoglRSkin &skin, deSkinProperty &property );
};

#endif
