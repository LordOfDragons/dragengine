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

#ifndef _DEOGLPERSISTENTRENDERTASK_H_
#define _DEOGLPERSISTENTRENDERTASK_H_

#include <dragengine/common/collection/decPointerDictionaryExt.h>
#include <dragengine/common/collection/decPointerLinkedList.h>
#include <dragengine/common/math/decMath.h>

#include "../../../deoglBasics.h"
#include "../../../shaders/paramblock/deoglShaderParameterBlockList.h"

class deoglPersistentRenderTaskPool;
class deoglPersistentRenderTaskInstance;
class deoglPersistentRenderTaskPipeline;
class deoglPersistentRenderTaskTexture;
class deoglPersistentRenderTaskVAO;
class deoglPersistentRenderTaskOwner;
class deoglRenderThread;
class deoglRTLogger;
class deoglPipeline;
class deoglSPBlockSSBO;
class deoglSPBlockUBO;
class deoglSPBlockUBO;
class deoglTexUnitsConfig;
class deoglVAO;
class deObject;


/**
 * Persistent render task.
 */
class deoglPersistentRenderTask{
private:
	deoglPersistentRenderTaskPool &pPool;
	
	deoglSPBlockUBO *pRenderParamBlock;
	GLuint pTBOInstances;
	deoglShaderParameterBlockList pSPBInstances;
	int pSPBInstanceMaxEntries;
	bool pUseSPBInstanceFlags;
	decPointerLinkedList pOwners;
	decPointerDictionaryExt pOwnersMap;
	decPointerLinkedList pPipelines;
	decPointerDictionaryExt pPipelinesMap;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create persistent render task. */
	deoglPersistentRenderTask( deoglPersistentRenderTaskPool &pool );
	
	/** Clean up render task. */
	~deoglPersistentRenderTask();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Prepare persistent render for rendering. */
	void PrepareForRender( deoglRenderThread &renderThread );
	
	/** Render parameter shader parameter block or \em NULL. */
	inline deoglSPBlockUBO *GetRenderParamBlock() const{ return pRenderParamBlock; }
	
	/** Set render parameter shader parameter block or \em NULL to use none. */
	void SetRenderParamBlock( deoglSPBlockUBO *paramBlock );
	
	/** Instances texture buffer object or \em NULL. */
	inline GLuint GetTBOInstances() const{ return pTBOInstances; }
	
	/** Set instances texture buffer object or \em NULL to use none. */
	void SetTBOInstances( GLuint tbo );
	
	/** Use SPB instance flags. */
	inline bool GetUseSPBInstanceFlags() const{ return pUseSPBInstanceFlags; }
	
	/** Set use instance flags. */
	void SetUseSPBInstanceFlags( bool useFlags );
	
	
	
	/** Number of owners. */
	int GetOwnerCount() const;
	
	/** Get root owner. */
	decPointerLinkedList::cListEntry *GetRootOwner() const;
	
	/** Get matching owner or NULL. */
	deoglPersistentRenderTaskOwner *GetOwnerWith( deObject *owner, unsigned int hash ) const;
	
	/** Add owner. */
	deoglPersistentRenderTaskOwner *AddOwner( deObject *owner, unsigned int hash );
	
	/** Remove owner. */
	void RemoveOwner( deoglPersistentRenderTaskOwner *owner );
	
	/** Remove all owners. */
	void RemoveAllOwners();
	
	
	
	/** Number of pipelines. */
	int GetPipelineCount() const;
	
	/** Root pipeline. */
	decPointerLinkedList::cListEntry *GetRootPipeline() const;
	
	/** Pipeline with pipeline or nullptr. */
	deoglPersistentRenderTaskPipeline *GetPipelineWith( const deoglPipeline *pipeline ) const;
	
	/** Add pipeline. */
	deoglPersistentRenderTaskPipeline *AddPipeline( const deoglPipeline *pipeline );
	
	/** Remove pipeline. */
	void RemovePipeline( deoglPersistentRenderTaskPipeline *pipeline );
	
	/** Remove all pipelines. */
	void RemoveAllPipelines();
	
	
	
	/** Number of points in all steps. */
	int GetTotalPointCount() const;
	
	/** Total amount of textures in this pipeline. */
	int GetTotalTextureCount() const;
	
	/** Total amount of vaos in this pipeline. */
	int GetTotalVAOCount() const;
	
	/** Total amount of instances in this pipeline. */
	int GetTotalInstanceCount() const;
	
	/** Total amount of subinstances in this pipeline. */
	int GetTotalSubInstanceCount() const;
	
	/** Clear. */
	void Clear();
	
	/** Remove elements owned by owner. */
	void RemoveOwnedBy( deoglPersistentRenderTaskOwner &owner );
	/*@}*/

	
	
	
private:
	void pCalcSPBInstancesMaxEntries( deoglRenderThread &renderThread );
	void pAssignSPBInstances( deoglRenderThread &renderThread );
	void pUpdateSPBInstances();
	void pCreateSPBInstanceParamBlock( deoglRenderThread &renderThread );
};

#endif
