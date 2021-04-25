/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLPERSISTENTRENDERTASK_H_
#define _DEOGLPERSISTENTRENDERTASK_H_

#include <dragengine/common/collection/decPointerDictionaryExt.h>
#include <dragengine/common/collection/decPointerLinkedList.h>
#include <dragengine/common/math/decMath.h>

#include "../../../deoglBasics.h"
#include "../../../shaders/paramblock/deoglShaderParameterBlockList.h"

class deoglPersistentRenderTaskPool;
class deoglPersistentRenderTaskInstance;
class deoglPersistentRenderTaskShader;
class deoglPersistentRenderTaskTexture;
class deoglPersistentRenderTaskVAO;
class deoglPersistentRenderTaskOwner;
class deoglRenderThread;
class deoglRTLogger;
class deoglShaderProgram;
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
	decPointerLinkedList pShaders;
	
	
	
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
	
	
	
	/** Number of shaders. */
	int GetShaderCount() const;
	
	/** Root shader. */
	decPointerLinkedList::cListEntry *GetRootShader() const;
	
	/** Shader with shader or NULL. */
	deoglPersistentRenderTaskShader *GetShaderWith( deoglShaderProgram *shader ) const;
	
	/** Add shader. */
	deoglPersistentRenderTaskShader *AddShader( deoglShaderProgram *shader );
	
	/** Remove shader. */
	void RemoveShader( deoglPersistentRenderTaskShader *shader );
	
	/** Remove all shaders. */
	void RemoveAllShaders();
	
	
	
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
