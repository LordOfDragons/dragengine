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

#ifndef _DEOGLPERSISTENTRENDERTASKINSTANCE_H_
#define _DEOGLPERSISTENTRENDERTASKINSTANCE_H_

#include "../../../deoglBasics.h"

#include <dragengine/common/collection/decPointerLinkedList.h>

class deoglPersistentRenderTaskPool;
class deoglPersistentRenderTaskVAO;
class deoglPersistentRenderTaskSubInstance;
class deoglShaderParameterBlock;
class deoglRenderThread;
class deoglSharedSPB;
class deoglSharedSPBRTIGroup;



/**
 * Persistent render task instance.
 */
class deoglPersistentRenderTaskInstance{
private:
	deoglPersistentRenderTaskPool &pPool;
	decPointerLinkedList::cListEntry pLLVAO;
	
	deoglPersistentRenderTaskVAO *pParentVAO;
	const deoglShaderParameterBlock *pParamBlock;
	const deoglShaderParameterBlock *pParamBlockSpecial;
	
	int pFirstPoint;
	int pPointCount;
	int pFirstIndex;
	int pIndexCount;
	GLenum pPrimitiveType;
	int pTessPatchVertexCount;
	
	decPointerLinkedList pSubInstances;
	deoglSharedSPB *pSubInstanceSPB;
	const deoglSharedSPBRTIGroup *pSubInstanceSPBGroup;
	deoglShaderParameterBlock *pSIIndexInstanceSPB;
	int pSIIndexInstanceFirst;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render task instance. */
	deoglPersistentRenderTaskInstance( deoglPersistentRenderTaskPool &pool );
	
	/** Clean up render task instance. */
	~deoglPersistentRenderTaskInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent VAO. */
	inline deoglPersistentRenderTaskVAO *GetParentVAO() const{ return pParentVAO; }
	
	/** Set parent VAO. */
	void SetParentVAO( deoglPersistentRenderTaskVAO *vao );
	
	/** Shader parameter block or \em NULL. */
	inline const deoglShaderParameterBlock *GetParameterBlock() const{ return pParamBlock; }
	
	/** Set shader parameter block or \em NULL. */
	void SetParameterBlock( const deoglShaderParameterBlock *block );
	
	/** Special shader parameter block or \em NULL. */
	inline const deoglShaderParameterBlock *GetParameterBlockSpecial() const{ return pParamBlockSpecial; }
	
	/** Set special shader parameter block or \em NULL. */
	void SetParameterBlockSpecial( const deoglShaderParameterBlock *block );
	
	
	
	/** Index of first point in VAO. */
	inline int GetFirstPoint() const{ return pFirstPoint; }
	
	/** Set index of first point in VAO. */
	void SetFirstPoint( int firstPoint );
	
	/** Number of points to render. */
	inline int GetPointCount() const{ return pPointCount; }
	
	/** Set number of points to render. */
	void SetPointCount( int pointCount );
	
	/** First index in VAO index buffer. */
	inline int GetFirstIndex() const{ return pFirstIndex; }
	
	/** Set first index in VAO index buffer. */
	void SetFirstIndex( int firstIndex );
	
	/** Number of indices in VAO index buffer. */
	inline int GetIndexCount() const{ return pIndexCount; }
	
	/** Set number of indices in VAO index buffer. */
	void SetIndexCount( int indexCount );
	
	/** Primitive type. */
	inline GLenum GetPrimitiveType() const{ return pPrimitiveType; }
	
	/** Set primitive type. */
	void SetPrimitiveType( GLenum primitiveType );
	
	/** Patch verted count. */
	inline int GetTessPatchVertexCount() const{ return pTessPatchVertexCount; }
	
	/** Set patch verted count. */
	void SetTessPatchVertexCount( int count );
	
	
	
	/** Count of sub instances to render. */
	int GetSubInstanceCount() const;
	
	/** Root sub instance. */
	decPointerLinkedList::cListEntry *GetRootSubInstance() const;
	
	/** Add sub instance. */
	deoglPersistentRenderTaskSubInstance *AddSubInstance( int indexInstance, int flags );
	
	/** Remove sub instance. */
	void RemoveSubInstance( deoglPersistentRenderTaskSubInstance *subInstance );
	
	/** Remove all sub instances. */
	void RemoveAllSubInstances();
	
	/** Sub instances SPB or NULL. */
	inline deoglSharedSPB *GetSubInstanceSPB() const{ return pSubInstanceSPB; }
	
	/** Sub instances SPB Group or NULL. */
	inline const deoglSharedSPBRTIGroup *GetSubInstanceSPBGroup() const{ return pSubInstanceSPBGroup; }
	
	/** Set Sub instances SPB and group. */
	void SetSubInstanceSPB( deoglSharedSPB *spb, const deoglSharedSPBRTIGroup *group );
	
	/** Sub instance index SPB. */
	inline deoglShaderParameterBlock *GetSIIndexInstanceSPB() const{ return pSIIndexInstanceSPB; }
	
	/** Index of first sub instance index for instance shared SPB. */
	inline int GetSIIndexInstanceFirst() const{ return pSIIndexInstanceFirst; }
	
	/** Set sub instance SPB information for later update. */
	void SetSIIndexInstanceParam( deoglShaderParameterBlock *paramBlock, int firstIndex );
	
	/** Write sub instance indices for instance shared SPB. */
	void WriteSIIndexInstanceInt( bool useFlags );
	
	/** Write sub instance indices for instance shared SPB. */
	void WriteSIIndexInstanceShort( bool useFlags );
	
	
	
	/** Clear. */
	void Clear();
	
	/** Remove from parent if empty. */
	void RemoveFromParentIfEmpty();
	
	
	
	/** Render task linked list. */
	inline decPointerLinkedList::cListEntry &GetLLVAO(){ return pLLVAO; }
	inline const decPointerLinkedList::cListEntry &GetLLVAO() const{ return pLLVAO; }
	/*@}*/
};

#endif
