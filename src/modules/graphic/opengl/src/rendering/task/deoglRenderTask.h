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

#ifndef _DEOGLRENDERTASK_H_
#define _DEOGLRENDERTASK_H_

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>

#include "../../deoglBasics.h"
#include "../../shaders/paramblock/deoglShaderParameterBlockList.h"

class deoglQuickSorter;
class deoglRenderTaskInstance;
class deoglRenderTaskShader;
class deoglRenderTaskTexture;
class deoglRenderTaskVAO;
class deoglRenderTaskSharedShader;
class deoglRenderThread;
class deoglRTLogger;
class deoglSPBlockSSBO;
class deoglSPBlockUBO;
class deoglSPBlockUBO;
class deoglTexUnitsConfig;
class deoglVAO;


/**
 * Render Task.
 *
 * Stores a list of render steps to be carried out. Each step contains the combination of shader,
 * texture and vao required. The render task is usually not used directly for rendering. Instead
 * a batched render task is generated from this list where matching steps are grouped together
 * for optimized rendering.
 */
class deoglRenderTask{
private:
	deoglSPBlockUBO *pRenderParamBlock;
	GLuint pTBOInstances;
	deoglShaderParameterBlockList pSPBInstances;
	int pSPBInstanceMaxEntries;
	bool pUseSPBInstanceFlags;
	unsigned int pTrackingNumber;
	bool pForceDoubleSided;
	
	deoglRenderTaskShader *pRootShader;
	deoglRenderTaskShader *pTailShader;
	int pShaderCount;
	deoglRenderTaskShader **pHasShader;
	int pHasShaderCount;
	int pHasShaderSize;
	
	deoglRenderTaskTexture *pRootTexturePool;
	deoglRenderTaskTexture *pTailTexturePool;
	deoglRenderTaskTexture *pNextTexturePool;
	int pTexturePoolCount;
	
	deoglRenderTaskVAO *pRootVAOPool;
	deoglRenderTaskVAO *pTailVAOPool;
	deoglRenderTaskVAO *pNextVAOPool;
	int pVAOPoolCount;
	
	deoglRenderTaskInstance *pRootInstancePool;
	deoglRenderTaskInstance *pTailInstancePool;
	deoglRenderTaskInstance *pNextInstancePool;
	int pInstancePoolCount;
	
	static unsigned int pUpdateTracking;
	
	decPointerList pListShaders;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render task. */
	deoglRenderTask();
	
	/** Clean up render task. */
	~deoglRenderTask();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Clear render task. */
	void Clear();
	
	/** Prepare task to be used for rendering. */
	void PrepareForRender( deoglRenderThread &renderThread );
	
	/** Sort instances by distance. */
	void SortInstancesByDistance( deoglQuickSorter &sorter,
		const decDVector &position, const decDVector &direction );
	
	/** Render parameter shader parameter block or NULL. */
	inline deoglSPBlockUBO *GetRenderParamBlock() const{ return pRenderParamBlock; }
	
	/** Set render parameter shader parameter block or NULL to use none. */
	void SetRenderParamBlock( deoglSPBlockUBO *paramBlock );
	
	/** Instances texture buffer object or NULL. */
	inline GLuint GetTBOInstances() const{ return pTBOInstances; }
	
	/** Set instances texture buffer object or NULL to use none. */
	void SetTBOInstances( GLuint tbo );
	
	/** Use SPB instance flags. */
	inline bool GetUseSPBInstanceFlags() const{ return pUseSPBInstanceFlags; }
	
	/** Set use instance flags. */
	void SetUseSPBInstanceFlags( bool useFlags );
	
	/** Force double sided rendering. */
	inline bool GetForceDoubleSided() const{ return pForceDoubleSided; }
	
	/** Set force double sided rendering. */
	void SetForceDoubleSided( bool forceDoubleSided );
	
	
	
	/** Tracking number. */
	inline unsigned int GetTrackingNumber() const{ return pTrackingNumber; }
	
	/** Advance update tracking returning new value. */
	static unsigned int UpdateTracking();
	
	
	
	/** Root render task shader or NULL if there is none. */
	inline deoglRenderTaskShader *GetRootShader() const{ return pRootShader; }
	
	/** Number of shaders. */
	inline int GetShaderCount() const{ return pShaderCount; }
	
	/** Add shader. */
	deoglRenderTaskShader *AddShader( deoglRenderTaskSharedShader *shader );
	
	
	
	/** Render task texture from pool. */
	deoglRenderTaskTexture *TextureFromPool();
	
	/** Render task vao from pool. */
	deoglRenderTaskVAO *VAOFromPool();
	
	/** Render task instance from pool. */
	deoglRenderTaskInstance *InstanceFromPool();
	
	
	
	/** Number of points in all steps. */
	int GetTotalPointCount() const;
	
	/** Total amount of textures in this shader. */
	int GetTotalTextureCount() const;
	
	/** Total amount of vaos in this shader. */
	int GetTotalVAOCount() const;
	
	/** Total amount of instances in this shader. */
	int GetTotalInstanceCount() const;
	
	/** Total amount of subinstances in this shader. */
	int GetTotalSubInstanceCount() const;
	/*@}*/
	
	
	
	/** \name Debug */
	/*@{*/
	/** Debug print. */
	void DebugPrint( deoglRTLogger &rtlogger );
	
	/** Size of render task texture pool. */
	inline int GetSizeTexturePool() const{ return pTexturePoolCount; }
	
	/** Count number of used entries from render task texture pool. */
	int CountUsedTexturePool() const;
	
	/** Size of render task vao pool. */
	inline int GetSizeVAOPool() const{ return pVAOPoolCount; }
	
	/** Count number of used entries from render task vao pool. */
	int CountUsedVAOPool() const;
	
	/** Size of render task instance pool. */
	inline int GetSizeInstancePool() const{ return pInstancePoolCount; }
	
	/** Count number of used entries from render task instance pool. */
	int CountUsedInstancePool() const;
	
	/** Debug output pool statistics. */
	void DebugPrintPoolStats( deoglRTLogger &rtlogger );
	/*@}*/
	
	
	
private:
	void pCalcSPBInstancesMaxEntries( deoglRenderThread &renderThread );
	void pAssignSPBInstances( deoglRenderThread &renderThread );
	void pUpdateSPBInstances();
	void pCreateSPBInstanceParamBlock( deoglRenderThread &renderThread );
};

#endif
