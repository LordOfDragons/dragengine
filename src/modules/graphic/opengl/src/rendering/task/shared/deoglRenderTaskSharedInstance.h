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

#ifndef _DEOGLRENDERTASKSHAREDINSTANCE_H_
#define _DEOGLRENDERTASKSHAREDINSTANCE_H_

#include "../../../deoglBasics.h"

#include <dragengine/deObject.h>

class deoglInstanceProgram;
class deoglRenderTaskSharedPool;
class deoglShaderParameterBlock;
class deoglRenderThread;
class deoglSharedSPB;


/**
 * Shared render task instance.
 */
class deoglRenderTaskSharedInstance : public deObject{
public:
	typedef deTObjectReference<deoglRenderTaskSharedInstance> Ref;
	
	
	
private:
	deoglRenderTaskSharedPool &pPool;
	const int pIndex;
	
	deoglShaderParameterBlock *pParamBlock;
	deoglShaderParameterBlock *pParamBlockSpecial;
	
	int pFirstPoint;
	int pPointCount;
	int pFirstIndex;
	int pIndexCount;
	int pSubInstanceCount;
	deoglSharedSPB *pSubInstanceSPB;
	GLenum pPrimitiveType;
	int pTessPatchVertexCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shared render task instance. */
	deoglRenderTaskSharedInstance( deoglRenderTaskSharedPool &pool, int index );
	
protected:
	/** Clean up shared render task instance. */
	virtual ~deoglRenderTaskSharedInstance();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Pool. */
	inline deoglRenderTaskSharedPool &GetPool() const{ return pPool; }
	
	/** Index. */
	inline int GetIndex() const{ return pIndex; }
	
	
	
	/** Shader parameter block or NULL. */
	inline deoglShaderParameterBlock *GetParameterBlock() const{ return pParamBlock; }
	
	/** Set shader parameter block or NULL. */
	void SetParameterBlock( deoglShaderParameterBlock *block );
	
	/** Special shader parameter block or NULL. */
	inline deoglShaderParameterBlock *GetParameterBlockSpecial() const{ return pParamBlockSpecial; }
	
	/** Set special shader parameter block or NULL. */
	void SetParameterBlockSpecial( deoglShaderParameterBlock *block );
	
	
	
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
	
	/** Number of sub instances to render. */
	inline int GetSubInstanceCount() const{ return pSubInstanceCount; }
	
	/** Set number of sub instances to render. */
	void SetSubInstanceCount( int subInstanceCount );
	
	/** Sub instances SPB or NULL. */
	inline deoglSharedSPB *GetSubInstanceSPB() const{ return pSubInstanceSPB; }
	
	/** Set sub instances SPB or NULL. */
	void SetSubInstanceSPB( deoglSharedSPB *element );
	
	/** Primitive type. */
	inline GLenum GetPrimitiveType() const{ return pPrimitiveType; }
	
	/** Set primitive type. */
	void SetPrimitiveType( GLenum primitiveType );
	
	/** Patch verted count. */
	inline int GetTessPatchVertexCount() const{ return pTessPatchVertexCount; }
	
	/** Set patch verted count. */
	void SetTessPatchVertexCount( int count );
	
	
	
	/** Clear. */
	void Clear();
	
	/** Return to pool. */
	void ReturnToPool();
	/*@}*/
};

#endif
