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

#ifndef _DEOGLRENDERTASKINSTANCE_H_
#define _DEOGLRENDERTASKINSTANCE_H_

#include "../../deoglBasics.h"

#include <dragengine/common/collection/decIntList.h>

class deoglShaderParameterBlock;
class deoglRenderThread;
class deoglSharedSPB;
class deoglRenderTaskInstanceGroup;



/**
 * \brief Render task instance.
 */
class deoglRenderTaskInstance{
private:
	deoglShaderParameterBlock *pParamBlock;
	deoglShaderParameterBlock *pParamBlockSpecial;
	
	int pFirstPoint;
	int pPointCount;
	int pFirstIndex;
	int pIndexCount;
	int pSubInstanceCount;
	deoglSharedSPB *pSubInstanceSPB;
	deoglRenderTaskInstanceGroup *pGroup;
	bool pDoubleSided;
	GLenum pPrimitiveType;
	int pTessPatchVertexCount;
	
	decIntList pSIIndexInstance;
	decIntList pSIFlags;
	deoglShaderParameterBlock *pSIIndexInstanceSPB;
	int pSIIndexInstanceFirst;
	
	deoglRenderTaskInstance *pNextInstance;
	
	deoglRenderTaskInstance *pLLNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render task instance. */
	deoglRenderTaskInstance();
	
	/** \brief Clean up render task instance. */
	~deoglRenderTaskInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Shader parameter block or \em NULL. */
	inline deoglShaderParameterBlock *GetParameterBlock() const{ return pParamBlock; }
	
	/** \brief Set shader parameter block or \em NULL. */
	void SetParameterBlock( deoglShaderParameterBlock *block );
	
	/** \brief Special shader parameter block or \em NULL. */
	inline deoglShaderParameterBlock *GetParameterBlockSpecial() const{ return pParamBlockSpecial; }
	
	/** \brief Set special shader parameter block or \em NULL. */
	void SetParameterBlockSpecial( deoglShaderParameterBlock *block );
	
	
	
	/** \brief Index of first point in VAO. */
	inline int GetFirstPoint() const{ return pFirstPoint; }
	
	/** \brief Set index of first point in VAO. */
	void SetFirstPoint( int firstPoint );
	
	/** \brief Number of points to render. */
	inline int GetPointCount() const{ return pPointCount; }
	
	/** \brief Set number of points to render. */
	void SetPointCount( int pointCount );
	
	/** \brief First index in VAO index buffer. */
	inline int GetFirstIndex() const{ return pFirstIndex; }
	
	/** \brief Set first index in VAO index buffer. */
	void SetFirstIndex( int firstIndex );
	
	/** \brief Number of indices in VAO index buffer. */
	inline int GetIndexCount() const{ return pIndexCount; }
	
	/** \brief Set number of indices in VAO index buffer. */
	void SetIndexCount( int indexCount );
	
	/** \brief Number of sub instances to render. */
	inline int GetSubInstanceCount() const{ return pSubInstanceCount; }
	
	/** \brief Set number of sub instances to render. */
	void SetSubInstanceCount( int subInstanceCount );
	
	/** \brief Sub instances SPB or \em NULL. */
	inline deoglSharedSPB *GetSubInstanceSPB() const{ return pSubInstanceSPB; }
	
	/** \brief Set sub instances SPB or \em NULL. */
	void SetSubInstanceSPB( deoglSharedSPB *element );
	
	/** \brief Group or \em NULL. */
	inline deoglRenderTaskInstanceGroup *GetGroup() const{ return pGroup; }
	
	/** \brief Set group or \em NULL. */
	void SetGroup( deoglRenderTaskInstanceGroup *group );
	
	/** \brief Double sided. */
	inline bool GetDoubleSided() const{ return pDoubleSided; }
	
	/** \brief Set double sided. */
	void SetDoubleSided( bool doubleSided );
	
	/** \brief Primitive type. */
	inline GLenum GetPrimitiveType() const{ return pPrimitiveType; }
	
	/** \brief Set primitive type. */
	void SetPrimitiveType( GLenum primitiveType );
	
	/** \brief Patch verted count. */
	inline int GetTessPatchVertexCount() const{ return pTessPatchVertexCount; }
	
	/** \brief Set patch verted count. */
	void SetTessPatchVertexCount( int count );
	
	
	
	/** \brief Add sub instance. */
	void AddSubInstance( int indexInstance, int flags );
	
	/** \brief Sub instance indices for instance shared SPB. */
	inline const decIntList &GetSIIndexInstance() const{ return pSIIndexInstance; }
	
	/** \brief Sub instance flags. */
	inline const decIntList &GetSIFlags() const{ return pSIFlags; }
	
	/** \brief Sub instance index SPB. */
	inline deoglShaderParameterBlock *GetSIIndexInstanceSPB() const{ return pSIIndexInstanceSPB; }
	
	/** \brief Index of first sub instance index for instance shared SPB. */
	inline int GetSIIndexInstanceFirst() const{ return pSIIndexInstanceFirst; }
	
	/** \brief Set sub instance SPB information for later update. */
	void SetSIIndexInstanceParam( deoglShaderParameterBlock *paramBlock, int firstIndex );
	
	/** \brief Write sub instance indices for instance shared SPB. */
	void WriteSIIndexInstanceInt( bool useFlags );
	
	/** \brief Write sub instance indices for instance shared SPB. */
	void WriteSIIndexInstanceShort( bool useFlags );
	
	
	
	/** \brief Clear. */
	void Clear();
	
	
	
	/** \brief Next instance or \em NULL. */
	inline deoglRenderTaskInstance *GetNextInstance() const{ return pNextInstance; }
	
	/** \brief Set next instance or \em NULL. */
	void SetNextInstance( deoglRenderTaskInstance *instance );
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Next instance in pool or \em NULL. */
	inline deoglRenderTaskInstance *GetLLNext() const{ return pLLNext; }
	
	/** \brief Set next instance in pool or \em NULL. */
	void SetLLNext( deoglRenderTaskInstance *instance );
	/*@}*/
};

#endif
