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
class deoglRenderTaskInstanceGroup;
class deoglRenderTaskShader;
class deoglRenderTaskTexture;
class deoglRenderTaskVAO;
class deoglRenderThread;
class deoglRTLogger;
class deoglShaderProgram;
class deoglSPBlockSSBO;
class deoglSPBlockUBO;
class deoglSPBlockUBO;
class deoglTexUnitsConfig;
class deoglVAO;


/**
 * \brief Render Task.
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
	decPointerList pListShaders;
	int pShaderCount;
	
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
	
	decPointerList pListTUCs;
	decPointerList pListVAOs;
	decPointerList pListInstanceGroup;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render task. */
	deoglRenderTask();
	
	/** \brief Clean up render task. */
	~deoglRenderTask();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Clear render task. */
	void Clear();
	
	/** \brief Prepare task to be used for rendering. */
	void PrepareForRender( deoglRenderThread &renderThread );
	
	/** \brief Sort instances by distance. */
	void SortInstancesByDistance( deoglQuickSorter &sorter,
		const decDVector &position, const decDVector &direction );
	
	/** \brief Render parameter shader parameter block or \em NULL. */
	inline deoglSPBlockUBO *GetRenderParamBlock() const{ return pRenderParamBlock; }
	
	/** \brief Set render parameter shader parameter block or \em NULL to use none. */
	void SetRenderParamBlock( deoglSPBlockUBO *paramBlock );
	
	/** \brief Instances texture buffer object or \em NULL. */
	inline GLuint GetTBOInstances() const{ return pTBOInstances; }
	
	/** \brief Set instances texture buffer object or \em NULL to use none. */
	void SetTBOInstances( GLuint tbo );
	
	/** \brief Use SPB instance flags. */
	inline bool GetUseSPBInstanceFlags() const{ return pUseSPBInstanceFlags; }
	
	/** \brief Set use instance flags. */
	void SetUseSPBInstanceFlags( bool useFlags );
	
	
	
	/** \brief Tracking number. */
	inline unsigned int GetTrackingNumber() const{ return pTrackingNumber; }
	
	/** \brief Advance update tracking returning new value. */
	static unsigned int UpdateTracking();
	
	
	
	/** \brief Number of shaders. */
	inline int GetShaderCount() const{ return pShaderCount; }
	
	/** \brief Shader at index. */
	deoglRenderTaskShader *GetShaderAt( int index ) const;
	
	/** \brief Add shader. */
	deoglRenderTaskShader *AddShader( deoglShaderProgram *shader );
	
	/** \brief Remove all shaders. */
	void RemoveAllShaders();
	
	
	
	/** \brief Render task texture from pool. */
	deoglRenderTaskTexture *TextureFromPool();
	
	/** \brief Render task vao from pool. */
	deoglRenderTaskVAO *VAOFromPool();
	
	/** \brief Render task instance from pool. */
	deoglRenderTaskInstance *InstanceFromPool();
	
	
	
	/** \brief Number of points in all steps. */
	int GetTotalPointCount() const;
	
	/** \brief Add texture units configuration. */
	void AddTUC( deoglTexUnitsConfig *tuc );
	
	/** \brfief Add vao. */
	void AddVAO( deoglVAO *vao );
	
	/** \brfief Add instance group. */
	void AddInstanceGroup( deoglRenderTaskInstanceGroup *group );
	/*@}*/
	
	
	
	/** \name Debug */
	/*@{*/
	/** \brief Debug print. */
	void DebugPrint( deoglRTLogger &rtlogger );
	
	/** \brief Size of render task texture pool. */
	inline int GetSizeTexturePool() const{ return pTexturePoolCount; }
	
	/** \brief Count number of used entries from render task texture pool. */
	int CountUsedTexturePool() const;
	
	/** \brief Size of render task vao pool. */
	inline int GetSizeVAOPool() const{ return pVAOPoolCount; }
	
	/** \brief Count number of used entries from render task vao pool. */
	int CountUsedVAOPool() const;
	
	/** \brief Size of render task instance pool. */
	inline int GetSizeInstancePool() const{ return pInstancePoolCount; }
	
	/** \brief Count number of used entries from render task instance pool. */
	int CountUsedInstancePool() const;
	
	/** \brief Debug output pool statistics. */
	void DebugPrintPoolStats( deoglRTLogger &rtlogger );
	/*@}*/
	
	
	
private:
	void pCalcSPBInstancesMaxEntries( deoglRenderThread &renderThread );
	void pAssignSPBInstances( deoglRenderThread &renderThread );
	void pUpdateSPBInstances();
	void pCreateSPBInstanceParamBlock( deoglRenderThread &renderThread );
};

#endif
