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
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../skin/pipeline/deoglSkinTexturePipelinesList.h"

class deoglRenderThread;
class deoglPipeline;
class deoglTexUnitsConfig;
class deoglVAO;
class deoglRenderTaskSharedInstance;
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
		etpSpecialFlags
	};
	
	/** Task. */
	struct sTask{
		uint32_t pipeline;
		uint32_t tuc;
		uint32_t vao;
		uint32_t instance;
		uint32_t spbInstance;
		uint32_t specialFlags;
	};
	
	/** Task resolved. */
	struct sTaskResolved{
		const deoglPipeline *pipeline;
		const deoglTexUnitsConfig *tuc;
		const deoglVAO *vao;
		const deoglRenderTaskSharedInstance *instance;
		int spbInstance;
		int specialFlags;
	};
	
	
	
private:
	deoglRenderThread &pRenderThread;
	
	deoglSPBlockUBO::Ref pUBOConfig;
	deoglSPBlockSSBO::Ref pSSBOSteps;
	
	bool pRenderVSStereo;
	
	int pSkinPipelineLists;
	deoglSkinTexturePipelines::eTypes pSkinPipelineType;
	
	bool pSolid;
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
	
	
	
	/** Clear. */
	void Clear();
	
	
	
	/** Use vertex shader stereo rendering. */
	inline bool GetRenderVSStereo() const{ return pRenderVSStereo; }
	
	/** Set use vertex shader stereo rendering. */
	void SetRenderVSStereo( bool renderVSStereo );
	
	
	
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
	
	
	
	/** Solid or transparent textures are added. */
	inline bool GetSolid() const{ return pSolid; }
	
	/** Set if solid or transparent texture are added. */
	void SetSolid( bool solid );
	
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
	
	
	
	/** Prepare buffers. */
	void PrepareBuffers( const deoglWorldCompute &worldCompute );
	/*@}*/
	
	
	
private:
	void pPrepareConfig( const deoglWorldCompute &worldCompute );
	void pRenderFilter( int &filter, int &mask ) const;
};

#endif
