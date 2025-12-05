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
