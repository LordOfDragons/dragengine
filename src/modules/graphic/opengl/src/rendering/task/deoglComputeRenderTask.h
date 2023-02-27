/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLCOMPUTERENDERTASK_H_
#define _DEOGLCOMPUTERENDERTASK_H_

#include <stdint.h>

#include "../../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../../shaders/paramblock/deoglSPBlockReadBackSSBO.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../skin/pipeline/deoglSkinTexturePipelinesList.h"

class deoglRenderThread;
class deoglPipeline;
class deoglRenderTaskSharedInstance;
class deoglRenderTaskSharedTexture;
class deoglRenderTaskSharedVAO;
class deoglWorldCompute;


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
		ecpPipelineModifier
	};
	
	/** Task parameters. */
	enum eTaskParameters{
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
		uint32_t spbInstance;
		uint32_t specialFlags;
		uint32_t subInstanceCount;
	};
	
	/** Counters. */
	struct sCounters{
		uint32_t workGroupSize[ 3 ];
		uint32_t counter;
	};
	
	/** Step resolved. */
	struct sStepResolved{
		const deoglPipeline *pipeline;
		const deoglRenderTaskSharedTexture *texture;
		const deoglRenderTaskSharedVAO *vao;
		const deoglRenderTaskSharedInstance *instance;
		int spbInstance;
		int specialFlags;
		int subInstanceCount;
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
	
	int pPassCount;
	int pPass;
	
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
	
	bool pFilterXRay;
	bool pXRay;
	
	bool pFilterHoles;
	bool pWithHoles;
	
	bool pFilterDecal;
	bool pDecal;
	
	int pFilterCubeFace;
	
	int pFilters;
	int pFilterMask;
	int pFiltersMasked;
	
	bool pUseSpecialParamBlock;
	
	sStepResolved *pStepsResolved;
	int pStepsResolvedSize;
	int pStepsResolvedCount;
	
	
	
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
	
	
	
	/** Begin preparing render task. */
	void BeginPrepare( int passCount );
	
	/** Clear. */
	void Clear();
	
	/** End pass switching to the next one. */
	void EndPass( const deoglWorldCompute &worldCompute );
	
	/** Finish preparing render task. */
	void EndPrepare( const deoglWorldCompute &worldCompute );
	
	/** Read back render task steps. */
	void ReadBackSteps( const deoglWorldCompute &worldCompute );
	
	
	
	/** Pass count. */
	inline int GetPassCount() const{ return pPassCount; }
	
	
	
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
	
	
	
	/** Use special shader parameter blocks. */
	inline bool GetUseSpecialParamBlock() const{ return pUseSpecialParamBlock; }
	
	/** Set if special shader parameter blocks are used. */
	void SetUseSpecialParamBlock( bool use );
	
	
	
	/** Count of resolved render steps. */
	inline int GetCountSteps() const{ return pStepsResolvedCount; }
	
	/** Resolved render steps direct access. */
	inline const sStepResolved *GetSteps() const{ return pStepsResolved; }
	/*@}*/
	
	
	
private:
	void pRenderFilter( int &filter, int &mask ) const;
	void pClearCounters();
};

#endif
