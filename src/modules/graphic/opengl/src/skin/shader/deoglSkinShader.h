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

#ifndef _DEOGLSKINSHADER_H_
#define _DEOGLSKINSHADER_H_

#include "deoglSkinShaderConfig.h"

#include <dragengine/deObject.h>

class deoglEnvironmentMap;
class deoglRDynamicSkin;
class deoglRenderThread;
class deoglShaderDefines;
class deoglSPBlockUBO;
class deoglSPBlockSSBO;
class deoglShaderProgram;
class deoglShaderSources;
class deoglSkinState;
class deoglSkinTexture;
class deoglTexUnitConfig;
class deoglShaderParameterBlock;



/**
 * @brief Skin Shader.
 */
class deoglSkinShader : public deObject{
public:
	typedef deTObjectReference<deoglSkinShader> Ref;
	
	static int REFLECTION_TEST_MODE;
	
	/** Texture targets. */
	enum eTextureTargets{
		ettColor,
		ettColorTintMask,
		ettTransparency,
		ettSolidity,
		ettNormal,
		ettHeight,
		ettReflectivity,
		ettRoughness,
		ettEnvMap,
		ettEnvMapFade,
		ettEmissivity,
		ettRenderColor,
		ettRefractionDistort,
		ettAO,
		ettEnvRoom,
		ettEnvRoomMask,
		ettEnvRoomEmissivity,
		ettAbsorption,
		ettRimEmissivity,
		ettNonPbrAlbedo,
		ettNonPbrMetalness,
		ettDepth,
		ettDepthTest,
		ettSamples,
		ettSubInstance1,
		ettSubInstance2,
		ettHeightMapMask,
		ETT_COUNT
	};
	
	/** Render parameter uniform targets. */
	enum eRenderUniformTargets{
		erutAmbient,
		erutMatrixV,
		erutMatrixP,
		erutMatrixVP,
		erutMatrixVn,
		erutMatrixEnvMap,
		erutMatrixSkyBody,
		erutDepthToPosition,
		erutDepthToPosition2,
		erutDepthTransform,
		erutEnvMapLodLevel,
		erutNorRoughCorrStrength,
		erutSkinDoesReflections,
		erutFlipCulling,
		erutClearDepthValue,
		erutViewport,
		erutClipPlane,
		erutScreenSpace,
		erutRenderSize,
		erutRenderLayerCount,
		erutMipMapParams,
		erutDepthOffset,
		erutParticleLightHack,
		erutBillboardZScale,
		erutFadeRange,
		erutCameraStereoMatrix,
		erutCameraAdaptedIntensity,
		erutCameraRange,
		erutDepthSampleOffset,
		erutFSScreenCoordToTexCoord,
		erutFSTexCoordToScreenCoord,
		erutSSAOParams1,
		erutSSAOParams2,
		erutSSAOParams3,
		erutSSSSSParams1,
		erutSSSSSParams2,
		erutSSRParams1,
		erutSSRParams2,
		erutSSRParams3,
		erutAOSelfShadow,
		erutLumFragCoordScale,
		erutGIRayMatrix,
		erutGIRayMatrixNormal,
		erutGIHighestCascade,
		erutToneMapSceneKey,
		erutToneMapAdaption,
		erutToneMapBloom,
		ERUT_COUNT
	};
	
	/** Texture parameter uniform targets. */
	enum eTextureUniformTargets{
		etutValueColorTransparency,
		etutValueNormal,
		etutValueReflectivityRoughness,
		etutValueRefractionDistort,
		etutValueSolidity,
		etutValueAO,
		
		etutTexColorTint,
		etutTexColorGamma,
		
		etutTexColorSolidMultiplier,
		etutTexAOSolidityMultiplier,
		etutTexSolidityMultiplier,
		etutTexAbsorptionRange,
		
		etutTexHeightRemap,
		etutTexNormalStrength,
		etutTexNormalSolidityMultiplier,
		
		etutTexRoughnessRemap,
		etutTexRoughnessGamma,
		etutTexRoughnessSolidityMultiplier,
		
		etutTexEnvRoomSize,
		etutTexRefractionDistortStrength,
		etutTexReflectivitySolidityMultiplier,
		
		etutTexEnvRoomOffset,
		etutTexTransparencyMultiplier,
		
		etutTexEnvRoomEmissivityIntensity,
		etutTexThickness,
		
		etutTexEmissivityIntensity,
		etutTexReflectivityMultiplier,
		
		etutTexVariationEnableScale,
		etutTexParticleSheetCount,
		etutTexRimAngle,
		
		etutTexRimEmissivityIntensity,
		etutTexRimExponent,
		
		etutTexOutlineColor,
		etutTexOutlineThickness,
		etutTexOutlineColorTint,
		etutTexOutlineSolidity,
		etutTexOutlineEmissivity,
		etutTexOutlineEmissivityTint,
		
		etutTexEmissivityCameraAdapted,
		
		ETUT_COUNT
	};
	
	/** Instance parameter uniform targets. */
	enum eInstanceUniformTargets{
		eiutMatrixModel,
		eiutMatrixNormal,
		
		eiutMatrixTexCoord,
		eiutPropFieldParams,
		eiutHeightTerrainMaskTCTransform,
		eiutHeightTerrainMaskSelector,
		eiutDoubleSided,
		eiutEnvMapFade,
		eiutVariationSeed,
		eiutBillboardPosTransform,
		eiutBillboardParams,
		
		eiutSamplesParams,
		eiutBurstFactor,
		eiutRibbonSheetCount,
		
		eiutIndexSPBTexParams,
		
		eiutTCTransformColor,
		eiutTCTransformNormal,
		eiutTCTransformReflectivity,
		eiutTCTransformEmissivity,
		eiutTCTransformRefractionDistort,
		eiutTCTransformAO,
		
		eiutInstColorTint,
		eiutInstColorGamma,
		eiutInstColorSolidityMultiplier,
		eiutInstTransparencyMultiplier,
		eiutInstSolidityMultiplier,
		eiutInstAOSolidityMultiplier,
		eiutInstHeightRemap,
		eiutInstNormalStrength,
		eiutInstNormalSolidityMultiplier,
		eiutInstRoughnessRemap,
		eiutInstRoughnessGamma,
		eiutInstRoughnessSolidityMultiplier,
		eiutInstRefractionDistortStrength,
		eiutInstReflectivitySolidityMultiplier,
		eiutInstEmissivityIntensity,
		eiutInstEnvRoomSize,
		eiutInstEnvRoomOffset,
		eiutInstEnvRoomEmissivityIntensity,
		eiutInstVariationEnableScale,
		eiutInstReflectivityMultiplier,
		eiutInstRimEmissivityIntensity,
		eiutInstRimAngle,
		eiutInstRimExponent,
		eiutInstOutlineColor,
		eiutInstOutlineThickness,
		eiutInstOutlineColorTint,
		eiutInstOutlineEmissivity,
		eiutInstOutlineSolidity,
		eiutInstOutlineEmissivityTint,
		
		EIUT_COUNT
	};
	
	/** Special parameter uniform targets. */
	enum eSpecialUniformTargets{
		esutLayerVisibility
	};
	
	/** Uniform blocks. */
	enum eUniformBlocks{
		eubRenderParameters,
		eubTextureParameters,
		eubInstanceParameters,
		eubSpecialParameters,
		eubInstanceIndex,
		EUB_COUNT
	};
	
	/** SSBO blocks. */
	enum eSSBOBlocks{
		essboInstanceParameters,
		essboInstanceIndex,
		essboTextureParameters
	};
	
private:
	deoglRenderThread &pRenderThread;
	
	deoglSkinShaderConfig pConfig;
	
	int pTextureTargets[ ETT_COUNT ];
	int pUsedTextureTargetCount;
	int pInstanceUniformTargets[ EIUT_COUNT ];
	int pUsedInstanceUniformTargetCount;
	
	int pTargetSPBInstanceIndexBase;
	
	deoglShaderSources *pSources;
	deoglShaderProgram *pShader;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new skin shader. */
	deoglSkinShader( deoglRenderThread &renderThread, const deoglSkinShaderConfig &config );
	/** Cleans up the skin shader. */
	virtual ~deoglSkinShader();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Retrieves the configuration. */
	inline const deoglSkinShaderConfig &GetConfig() const{ return pConfig; }
	
	/** Retrieves the index for a texture target or -1 if not used. */
	int GetTextureTarget( eTextureTargets target ) const;
	/** Sets the index for a texture target or -1 if not used. */
	void SetTextureTarget( eTextureTargets target, int index );
	/** Retrieves the used texture target count. */
	inline int GetUsedTextureTargetCount() const{ return pUsedTextureTargetCount; }
	/** Sets the used texture target count. */
	void SetUsedTextureTargetCount( int usedTextureTargetCount );
	
	/** Retrieves the index for an instance parameter uniform target or -1 if not used. */
	int GetInstanceUniformTarget( eInstanceUniformTargets target ) const;
	/** Sets the index for an instance parameter uniform target or -1 if not used. */
	void SetInstanceUniformTarget( eInstanceUniformTargets target, int index );
	
	/** Target of shared SPB instance index base parameter or -1 if not used. */
	inline int GetTargetSPBInstanceIndexBase() const{ return pTargetSPBInstanceIndexBase; }
	
	/** Prepare shader. */
	void PrepareShader();
	
	/** Shader. */
	inline deoglShaderProgram *GetShader() const{ return pShader; }
	
	/** Create render skin shader shader parameter block. */
	static deoglSPBlockUBO *CreateSPBRender( deoglRenderThread &renderThread );
	static deoglSPBlockUBO *CreateSPBRenderCubeMap( deoglRenderThread &renderThread );
	static deoglSPBlockUBO *CreateSPBRenderCascaded( deoglRenderThread &renderThread );
	static deoglSPBlockUBO *CreateSPBRenderStereo( deoglRenderThread &renderThread );
	
	static deoglSPBlockUBO *CreateSPBRender( deoglRenderThread &renderThread,
		int matrixLayerCount, int projectionMatrixLayerCount, int depthOffsetLayerCount );
	
	/** Create occlusion map shader parameter block. */
	static deoglSPBlockUBO *CreateSPBOccMap( deoglRenderThread &renderThread );
	
	/** Create special shader parameter block. */
	static deoglSPBlockUBO *CreateSPBSpecial( deoglRenderThread &renderThread );
	
	/** Create texture parameter shader parameter block. */
	static deoglSPBlockUBO *CreateSPBTexParam( deoglRenderThread &renderThread );
	
	/** Create instance parameter shader parameter block. */
	deoglSPBlockUBO *CreateSPBInstParam() const;
	
	/** Create shared instance parameter shader storage buffer. */
	static deoglSPBlockUBO *CreateLayoutSkinInstanceUBO( deoglRenderThread &renderThread );
	
	/** Create shared instance parameter shader storage buffer. */
	static deoglSPBlockSSBO *CreateLayoutSkinInstanceSSBO( deoglRenderThread &renderThread );
	
	/** Create shared texture parameter shader storage buffer. */
	static deoglSPBlockUBO *CreateLayoutSkinTextureUBO( deoglRenderThread &renderThread );
	
	/** Create shared texture parameter shader storage buffer. */
	static deoglSPBlockSSBO *CreateLayoutSkinTextureSSBO( deoglRenderThread &renderThread );
	
	/** Set texture parameters in instance parameter shader block. */
	void SetTexParamsInInstParamSPB( deoglShaderParameterBlock &paramBlock,
		const deoglSkinTexture &skinTexture ) const;
	
	/** Set texture parameters in instance parameter shader block. */
	void SetTexParamsInInstParamSPB( deoglShaderParameterBlock &paramBlock, int element,
		const deoglSkinTexture &skinTexture ) const;
	
	/** Set dynamic texture parameters in instance parameter shader block. */
	void SetDynTexParamsInInstParamSPB( deoglShaderParameterBlock &paramBlock,
		const deoglSkinTexture &skinTexture, deoglSkinState *skinState,
		deoglRDynamicSkin *dynamicSkin ) const;
	
	/** Set dynamic texture parameters in instance parameter shader block. */
	void SetDynTexParamsInInstParamSPB( deoglShaderParameterBlock &paramBlock,
		int element, const deoglSkinTexture &skinTexture, deoglSkinState *skinState,
		deoglRDynamicSkin *dynamicSkin ) const;
	
	/**
	 * Set texture unit configurations.
	 * 
	 * This only sets units for targets used in this skin shader. All unused targets are
	 * left untouched. The configs array has to contain at least GetUsedTextureTargetCount
	 * entries. No boundary checking is conducted.
	 */
	void SetTUCCommon( deoglTexUnitConfig *units, const deoglSkinTexture &skinTexture,
		deoglSkinState *skinState, deoglRDynamicSkin *dynamicSkin );
	
	/**
	 * Sets per-object environment map texture unit configurations.
	 * 
	 * This only sets units for targets used in this skin shader. All unused targets are
	 * left untouched. The configs array has to contain at least GetUsedTextureTargetCount
	 * entries. No boundary checking is conducted. The env map targets are only set if they
	 * are not yet set to a user specified value. For this to work SetTUCPerObjectEnvMap has
	 * * to be called after SetTUCCommon.
	 * 
	 * This is a temporary call used to handle the per-object environment map situation until
	 * the real solution is implemented and working correctly.
	 */
	void SetTUCPerObjectEnvMap( deoglTexUnitConfig *units, deoglEnvironmentMap *envmapSky,
		deoglEnvironmentMap *envmap, deoglEnvironmentMap *envmapFade );
	/*@}*/
	
	
	
	/** @name Shader Generation */
	/*@{*/
	/** Generate shader. */
	void GenerateShader();
	/** Generate shader defines. */
	void GenerateDefines( deoglShaderDefines &defines );
	/** Generate source code for the vertex unit. */
	void GenerateVertexSC();
	/** Generate source code for the geometry unit. */
	void GenerateGeometrySC();
	/** Generate source code for the fragment unit. */
	void GenerateFragmentSC();
	/** Generate source code for the tessellation control unit. */
	void GenerateTessellationControlSC();
	/** Generate source code for the tessellation evaluation unit. */
	void GenerateTessellationEvaluationSC();
	/** Update texture targets. */
	void UpdateTextureTargets();
	/** Update static and dynamic uniform targets. */
	void UpdateUniformTargets();
	/** Init shader parameters. */
	void InitShaderParameters();
	/*@}*/
};

#endif
