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

#ifndef _DEOGLCOMPUTERENDERTASK_H_
#define _DEOGLCOMPUTERENDERTASK_H_

#include <stdint.h>

#include "../../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../skin/pipeline/deoglSkinTexturePipelinesList.h"

class deoglRenderThread;
class deoglPipeline;
class deoglRenderTaskSharedInstance;
class deoglRenderTaskSharedTexture;
class deoglRenderTaskSharedVAO;
class deoglWorldCompute;
class deoglRTLogger;



/**
 * Compute render task.
 */
class deoglComputeRenderTask : public deObject{
public:
	typedef deTObjectReference<deoglComputeRenderTask> Ref;
	
	/** Config parameters. */
	enum eConfigParameters{
		ecpElementGeometryCount,
		ecpFilterCubeFace,
		ecpRenderTaskFilter,
		ecpRenderTaskFilterMask,
		ecpFilterPipelineLists,
		ecpPipelineType,
		ecpPipelineModifier,
		ecpPipelineDoubleSided,
		ecpPipelineSingleSided
	};
	
	/** Task parameters. */
	enum eTaskParameters{
		etpPass,
		etpPipeline,
		etpTuc,
		etpVao,
		etpInstance,
		etpSpbInstance,
		etpSpecialFlags,
		etpSubInstanceCount
	};
	
	/** Step. */
	struct sStep{
		uint32_t pass;
		uint32_t pipeline;
		uint32_t tuc;
		uint32_t vao;
		uint32_t instance;
		uint32_t spbInstance; // SPBInstance + 1
		uint32_t specialFlags;
		uint32_t subInstanceCount;
	};
	
	/** Stage. */
	enum eStates{
		esInitial,
		esPreparing,
		esBuilding,
		esReady
	};
	
	/** Prepare guard. */
	class cGuard{
	private:
		deoglComputeRenderTask &pRenderTask;
		const deoglWorldCompute &pWorldCompute;
		
	public:
		cGuard( deoglComputeRenderTask &renderTask, const deoglWorldCompute &worldCompute, int passCount );
		~cGuard();
	};
	
	
	
private:
	deoglRenderThread &pRenderThread;
	
	deoglSPBlockUBO::Ref pUBOConfig;
	deoglSPBlockSSBO::Ref pSSBOSteps;
	deoglSPBlockSSBO::Ref pSSBOCounters;
	
	eStates pState;
	int pPassCount;
	int pPass;
	
	bool pUseSPBInstanceFlags;
	bool pRenderVSStereo;
	deoglSPBlockUBO::Ref pRenderParamBlock;
	
	int pSkinPipelineLists;
	deoglSkinTexturePipelines::eTypes pSkinPipelineType;
	int pSkinPipelineModifier;
	
	bool pSolid;
	bool pFilterSolid;
	
	bool pNoShadowNone;
	bool pNoNotReflected;
	bool pNoRendered;
	bool pOutline;
	bool pForceDoubleSided;
	bool pOcclusion;
	
	bool pShadow;
	bool pFilterShadow;
	bool pCompactShadow;
	
	bool pFilterDoubleSided;
	bool pDoubleSided;
	
	bool pFilterXRay;
	bool pXRay;
	
	bool pFilterHoles;
	bool pWithHoles;
	
	bool pFilterDecal;
	bool pDecal;
	
	int pFilterCubeFace;
	
	const deoglPipeline *pPipelineDoubleSided;
	const deoglPipeline *pPipelineSingleSided;
	
	bool pUseSpecialParamBlock;
	
	sStep *pSteps;
	int pStepCount;
	int pStepSize;
	bool pSkipSubInstanceGroups;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create compute render task. */
	deoglComputeRenderTask( deoglRenderThread &renderThread );
	
	/** Clean up compute render task. */
	~deoglComputeRenderTask();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Configuration UBO. */
	inline const deoglSPBlockUBO::Ref &GetUBOConfig() const{ return pUBOConfig; }
	
	/** Render steps SSBO. */
	inline const deoglSPBlockSSBO::Ref &GetSSBOSteps() const{ return pSSBOSteps; }
	
	/** Counters SSBO. */
	inline const deoglSPBlockSSBO::Ref &GetSSBOCounters() const{ return pSSBOCounters; }
	
	
	
	/** State. */
	inline eStates GetState() const{ return pState; }
	
	/** Begin preparing render task. */
	void BeginPrepare( int passCount );
	
	/** Clear. */
	void Clear();
	
	/** End pass switching to the next one. */
	void EndPass( const deoglWorldCompute &worldCompute );
	
	/** Finish preparing render task. */
	void EndPrepare( const deoglWorldCompute &worldCompute );
	
	/**
	 * Read back render task steps.
	 * 
	 * Checks first if the counters are less than or equal to the steps SSBO element count.
	 * 
	 * If the SSBO has not been large enough the SSBO is enlarged, the counter is reset,
	 * the stage set to esBuilding and false is returned. In this case the caller has to
	 * rebuild the render task and call ReadBackSteps() again.
	 * 
	 * If the SSBO is large enough reading back is done and true is returned. This prevents
	 * the steps SSBO from growing very large compared to the size required consuming lots
	 * of GPU memory for nothing
	 */
	bool ReadBackSteps();
	
	/** Render task. */
	void Render();
	
	
	
	/** Pass count. */
	inline int GetPassCount() const{ return pPassCount; }
	
	
	
	/** Use SPB instance flags. */
	inline bool GetUseSPBInstanceFlags() const{ return pUseSPBInstanceFlags; }
	
	/** Set use instance flags. */
	void SetUseSPBInstanceFlags( bool useFlags );
	
	/** Use vertex shader stereo rendering. */
	inline bool GetRenderVSStereo() const{ return pRenderVSStereo; }
	
	/** Set use vertex shader stereo rendering. */
	void SetRenderVSStereo( bool renderVSStereo );
	
	/** Render parameter shader parameter block or nullptr. */
	inline const deoglSPBlockUBO::Ref &GetRenderParamBlock() const{ return pRenderParamBlock; }
	
	/** Set render parameter shader parameter block or nullptr. */
	void SetRenderParamBlock( deoglSPBlockUBO *paramBlock );
	
	
	
	/** Pipeline list mask. */
	inline int GetSkinPipelineLists() const{ return pSkinPipelineLists; }
	
	/** Set pipeline list mask. */
	void SetSkinPipelineLists( int mask );
	
	/** Enable pipeline list. */
	void EnableSkinPipelineList( deoglSkinTexturePipelinesList::ePipelineTypes list );
	
	/** Disable pipeline list. */
	void DisableSkinPipelineList( deoglSkinTexturePipelinesList::ePipelineTypes list );
	
	/** Pipeline type. */
	inline deoglSkinTexturePipelines::eTypes GetSkinPipelineType() const{ return pSkinPipelineType; }
	
	/** Set pipeline type. */
	void SetSkinPipelineType( deoglSkinTexturePipelines::eTypes type );
	
	/** Pipeline modifier. */
	inline int GetSkinPipelineModifier() const{ return pSkinPipelineModifier; }
	
	/** Set pipeline modifier. */
	void SetSkinPipelineModifier( int modifier );
	
	
	
	/** Solid or transparent textures are added. */
	inline bool GetSolid() const{ return pSolid; }
	
	/** Set if solid or transparent texture are added. */
	void SetSolid( bool solid );
	
	/** Filter solid. */
	inline bool GetFilterSolid() const{ return pFilterSolid; }
	
	/** Set to filter solid. */
	void SetFilterSolid( bool filterSolid );
	
	
	
	/** Textures with the shadow none property are not added. */
	inline bool GetNoShadowNone() const{ return pNoShadowNone; }
	
	/** Set if textures with the shadow none property are not added. */
	void SetNoShadowNone( bool noShadowNone );
	
	/** Textures without the reflected property are not added. */
	inline bool GetNoNotReflected() const{ return pNoNotReflected; }
	
	/** Set if textures without the reflected property are not added. */
	void SetNoNotReflected( bool noNotReflected );
	
	/** Rendered textures are not added. */
	inline bool GetNoRendered() const{ return pNoRendered; }
	
	/** Set if rendered textures are not added. */
	void SetNoRendered( bool noRendered );
	
	/** Outline textures are added. */
	inline bool GetOutline() const{ return pOutline; }
	
	/** Set if outline transparent texture are added. */
	void SetOutline( bool outline );
	
	/** Force double sided. */
	inline bool GetForceDoubleSided() const{ return pForceDoubleSided; }
	
	/** Set to force double sided. */
	void SetForceDoubleSided( bool forceDoubleSided );
	
	/** Occlusion. */
	inline bool GetOcclusion() const{ return pOcclusion; }
	
	/** Set occlusion. */
	void SetOcclusion( bool occlusion );
	
	
	
	/** Shadow. */
	inline bool GetShadow() const{ return pShadow; }
	
	/** Set shadow. */
	void SetShadow( bool shadow );
	
	/** Filter shadow. */
	inline bool GetFilterShadow() const{ return pFilterShadow; }
	
	/** Set filter shadow. */
	void SetFilterShadow( bool filter );
	
	/** Compact shadow. */
	inline bool GetCompactShadow() const{ return pCompactShadow; }
	
	/** Set compact shadow. */
	void SetCompactShadow( bool compactShadow );
	
	
	
	/** Filtering for double sided is enabled. */
	inline bool GetFilterDoubleSided() const{ return pFilterDoubleSided; }
	
	/** Set if filtering for double sided is enabled. */
	void SetFilterDoubleSided( bool filterDoubleSided );
	
	/** Double sided textures are added. */
	inline bool GetDoubleSided() const{ return pDoubleSided; }
	
	/** Set if double sided texture are added. */
	void SetDoubleSided( bool doubleSided );
	
	
	
	/** Filtering for XRay is enabled. */
	inline bool GetFilterXRay() const{ return pFilterXRay; }
	
	/** Set if filtering for XRay is enabled. */
	void SetFilterXRay( bool filterXRay );
	
	/** XRay textures are added. */
	inline bool GetXRay() const{ return pXRay; }
	
	/** Set if XRay texture are added. */
	void SetXRay( bool xray );
	
	
	
	/** Filtering for holes is enabled. */
	inline bool GetFilterHoles() const{ return pFilterHoles; }
	
	/** Set if filtering for holes is enabled. */
	void SetFilterHoles( bool filterHoles );
	
	/** Textures with or without holes are added. */
	inline bool GetWithHoles() const{ return pWithHoles; }
	
	/** Set if textures with or without holes are added. */
	void SetWithHoles( bool withHoles );
	
	
	
	/** Filtering for decal is enabled. */
	inline bool GetFilterDecal() const{ return pFilterDecal; }
	
	/** Set if filtering for decal is enabled. */
	void SetFilterDecal( bool filterDecal );
	
	/** Decal textures are selected if decal filtering is enabled. */
	inline bool GetDecal() const{ return pDecal; }
	
	/** Set if decal textures are selected if decal filtering is enabled. */
	void SetDecal( bool decal );
	
	
	
	/** Filter by cube face test result or -1 if disabled. */
	inline int GetFilterCubeFace() const{ return pFilterCubeFace; }
	
	/** Set filter by cube face test result or -1 if disabled. */
	void SetFilterCubeFace( int cubeFace );
	
	
	
	/** Pipeline for double sided occlusion or nullptr. */
	inline const deoglPipeline *GetPipelineDoubleSided() const{ return pPipelineDoubleSided; }
	
	/** Set pipeline for double sided occlusion or nullptr. */
	void SetPipelineDoubleSided( const deoglPipeline *pipeline );
	
	/** Pipeline for single sided occlusion or nullptr. */
	inline const deoglPipeline *GetPipelineSingleSided() const{ return pPipelineSingleSided; }
	
	/** Set pipeline for single sided occlusion or nullptr. */
	void SetPipelineSingleSided( const deoglPipeline *pipeline );
	
	
	
	/** Use special shader parameter blocks. */
	inline bool GetUseSpecialParamBlock() const{ return pUseSpecialParamBlock; }
	
	/** Set if special shader parameter blocks are used. */
	void SetUseSpecialParamBlock( bool use );
	
	
	
	/** Count of steps. */
	inline int GetStepCount() const{ return pStepCount; }
	
	/** Render steps direct access or nullptr if not mapped. */
	inline const sStep *GetSteps() const{ return pSteps; }
	
	/** Skip sub instance groups. */
	inline bool GetSkipSubInstanceGroups() const{ return pSkipSubInstanceGroups; }
	/*@}*/
	
	
	
	/** \name Debug */
	/*@{*/
	/** Debug print. */
	void DebugSimple( deoglRTLogger &logger, bool sorted );
	/*@}*/
	
	
	
private:
	void pRenderFilter( int &filter, int &mask ) const;
	void pClearCounters();
};

#endif
