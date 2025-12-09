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

#ifndef _DEOGLSKINSHADER_H_
#define _DEOGLSKINSHADER_H_

#include "deoglSkinShaderConfig.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderSources.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../../shaders/compiler/deoglShaderCompileListener.h"

#include <dragengine/deObject.h>

class deoglEnvironmentMap;
class deoglRDynamicSkin;
class deoglRenderThread;
class deoglShaderDefines;
class deoglSkinState;
class deoglSkinTexture;
class deoglTexUnitConfig;
class deoglShaderParameterBlock;


/**
 * Skin Shader.
 */
class deoglSkinShader : public deObject{
public:
	typedef deTObjectReference<deoglSkinShader> Ref;
	
	
	enum eReflectionTestMode{
		ertmOldVersion,
		ertmOwnPassReflection,
		ertmSingleBlenderEnvMap
	};
	
	/** ertmSingleBlenderEnvMap */
	static eReflectionTestMode REFLECTION_TEST_MODE;
	
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
		erutViewportImage,
		erutClipPlane,
		erutScreenSpace,
		erutRenderSize,
		erutRenderSizeCompute,
		erutMipMapParams,
		erutDepthOffset,
		erutParticleLightHack,
		erutBillboardZScale,
		erutFadeRange,
		erutCameraStereoMatrix,
		erutCameraRange,
		erutCameraAdaptedIntensity,
		erutDepthSampleOffset,
		erutFSScreenCoordToTexCoord,
		erutFSTexCoordToScreenCoord,
		erutFSFragCoordToTexCoord,
		erutFSFragCoordToScreenCoord,
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
		erutDebugDepthTransform,
		erutConditions1,
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
		
		etutTexSolidityRemap,
		etutTexColorSolidMultiplier,
		etutTexAOSolidityMultiplier,
		
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
		
		etutTexEnvRoomTint,
		etutTexAbsorptionRange,
		
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
		
		etutTexSkinClipPlane,
		etutTexSkinClipPlaneBorder,
		
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
		eiutInstSolidityRemap,
		eiutInstColorSolidityMultiplier,
		eiutInstTransparencyMultiplier,
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
		eiutInstEnvRoomTint,
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
		eiutInstSkinClipPlaneNormal,
		eiutInstSkinClipPlane,
		eiutInstSkinClipPlaneBorder,
		
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
	
	class cShaderPreparedListener{
	public:
		cShaderPreparedListener() = default;
		virtual ~cShaderPreparedListener() = default;
		
		/** Prepare shader finished. If failed shader.GetShader().GetCompiled() is nullptr. */
		virtual void PrepareShaderFinished(deoglSkinShader &shader) = 0;
		/*@}*/
	};
	
	
private:
	class cPrepareShader : public deoglShaderCompileListener{
	private:
		deoglSkinShader &pShader;
		cShaderPreparedListener *pListener;
		
	public:
		cPrepareShader(deoglSkinShader &shader, cShaderPreparedListener *listener);
		void CompileFinished(deoglShaderProgram *program) override;
	};
	
	friend class cPrepareShader;
	
	
	deoglRenderThread &pRenderThread;
	
	deoglSkinShaderConfig pConfig;
	
	int pTextureTargets[ETT_COUNT];
	int pInstanceUniformTargets[EIUT_COUNT];
	int pUsedInstanceUniformTargetCount;
	int pTextureUnitCount;
	
	int pTargetSPBInstanceIndexBase;
	int pTargetDrawIDOffset;
	
	deoglShaderSources::Ref pSources;
	deoglShaderProgram::Ref pShader;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new skin shader. */
	deoglSkinShader(deoglRenderThread &renderThread, const deoglSkinShaderConfig &config);
	/** Cleans up the skin shader. */
	virtual ~deoglSkinShader();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Retrieves the configuration. */
	inline const deoglSkinShaderConfig &GetConfig() const{ return pConfig; }
	
	/** Retrieves the index for a texture target or -1 if not used. */
	int GetTextureTarget(eTextureTargets target) const;
	
	/** Retrieves the index for an instance parameter uniform target or -1 if not used. */
	int GetInstanceUniformTarget(eInstanceUniformTargets target) const;
	
	/** Required count of texture unit index used by this shader. */
	inline int GetTextureUnitCount() const{ return pTextureUnitCount; }
	
	/** Target of shared SPB instance index base parameter or -1 if not used. */
	inline int GetTargetSPBInstanceIndexBase() const{ return pTargetSPBInstanceIndexBase; }
	
	/** Target of draw id offset parameter or -1 if not used. */
	inline int GetTargetDrawIDOffset() const{ return pTargetDrawIDOffset; }
	
	/** Prepare shader. For use by deoglSkinShaderManager only. */
	void PrepareShader(cShaderPreparedListener *listener);
	
	/** Shader. */
	inline const deoglShaderProgram::Ref &GetShader() const{ return pShader; }
	
	/** Create render skin shader shader parameter block. */
	static deoglSPBlockUBO::Ref CreateSPBRender(deoglRenderThread &renderThread);
	
	/** Create occlusion map shader parameter block. */
	static deoglSPBlockUBO::Ref CreateSPBOccMap(deoglRenderThread &renderThread);
	
	/** Create special shader parameter block. */
	static deoglSPBlockUBO::Ref CreateSPBSpecial(deoglRenderThread &renderThread);
	
	/** Create texture parameter shader parameter block. */
	static deoglSPBlockUBO::Ref CreateSPBTexParam(deoglRenderThread &renderThread);
	
	/** Create instance parameter shader parameter block. */
	deoglSPBlockUBO::Ref CreateSPBInstParam() const;
	
	/** Create shared instance parameter shader storage buffer. */
	static deoglSPBlockUBO::Ref CreateLayoutSkinInstanceUBO(deoglRenderThread &renderThread);
	
	/** Create shared instance parameter shader storage buffer. */
	static deoglSPBlockSSBO::Ref CreateLayoutSkinInstanceSSBO(deoglRenderThread &renderThread);
	
	/** Create shared texture parameter shader storage buffer. */
	static deoglSPBlockUBO::Ref CreateLayoutSkinTextureUBO(deoglRenderThread &renderThread);
	
	/** Create shared texture parameter shader storage buffer. */
	static deoglSPBlockSSBO::Ref CreateLayoutSkinTextureSSBO(deoglRenderThread &renderThread);
	
	/** Set texture parameters in instance parameter shader block. */
	void SetTexParamsInInstParamSPB(deoglShaderParameterBlock &paramBlock,
		const deoglSkinTexture &skinTexture) const;
	
	/** Set texture parameters in instance parameter shader block. */
	void SetTexParamsInInstParamSPB(deoglShaderParameterBlock &paramBlock, int element,
		const deoglSkinTexture &skinTexture) const;
	
	/** Set dynamic texture parameters in instance parameter shader block. */
	void SetDynTexParamsInInstParamSPB(deoglShaderParameterBlock &paramBlock,
		const deoglSkinTexture &skinTexture, deoglSkinState *skinState,
		deoglRDynamicSkin *dynamicSkin) const;
	
	/** Set dynamic texture parameters in instance parameter shader block. */
	void SetDynTexParamsInInstParamSPB(deoglShaderParameterBlock &paramBlock,
		int element, const deoglSkinTexture &skinTexture, deoglSkinState *skinState,
		deoglRDynamicSkin *dynamicSkin) const;
	
	/**
	 * Set texture unit configurations.
	 * 
	 * This only sets units for targets used in this skin shader. All unused targets are
	 * left untouched. The configs array has to contain at least GetUsedTextureTargetCount
	 * entries. No boundary checking is conducted.
	 */
	void SetTUCCommon(deoglTexUnitConfig *units, const deoglSkinTexture &skinTexture,
		deoglSkinState *skinState, deoglRDynamicSkin *dynamicSkin);
	
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
	void SetTUCPerObjectEnvMap(deoglTexUnitConfig *units, deoglEnvironmentMap *envmapSky,
		deoglEnvironmentMap *envmap, deoglEnvironmentMap *envmapFade);
	/*@}*/
	
	
	
private:
	/** \name Shader Generation */
	/*@{*/
	/** Generate shader. */
	void GenerateShader(cShaderPreparedListener *listener);
	/** Generate shader defines. */
	void GenerateDefines(deoglShaderDefines &defines);
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
