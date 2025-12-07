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
	deoglPersistentRenderTaskInstance(deoglPersistentRenderTaskPool &pool);
	
	/** Clean up render task instance. */
	~deoglPersistentRenderTaskInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent VAO. */
	inline deoglPersistentRenderTaskVAO *GetParentVAO() const{return pParentVAO;}
	
	/** Set parent VAO. */
	void SetParentVAO(deoglPersistentRenderTaskVAO *vao);
	
	/** Shader parameter block or \em NULL. */
	inline const deoglShaderParameterBlock *GetParameterBlock() const{return pParamBlock;}
	
	/** Set shader parameter block or \em NULL. */
	void SetParameterBlock(const deoglShaderParameterBlock *block);
	
	/** Special shader parameter block or \em NULL. */
	inline const deoglShaderParameterBlock *GetParameterBlockSpecial() const{return pParamBlockSpecial;}
	
	/** Set special shader parameter block or \em NULL. */
	void SetParameterBlockSpecial(const deoglShaderParameterBlock *block);
	
	
	
	/** Index of first point in VAO. */
	inline int GetFirstPoint() const{return pFirstPoint;}
	
	/** Set index of first point in VAO. */
	void SetFirstPoint(int firstPoint);
	
	/** Number of points to render. */
	inline int GetPointCount() const{return pPointCount;}
	
	/** Set number of points to render. */
	void SetPointCount(int pointCount);
	
	/** First index in VAO index buffer. */
	inline int GetFirstIndex() const{return pFirstIndex;}
	
	/** Set first index in VAO index buffer. */
	void SetFirstIndex(int firstIndex);
	
	/** Number of indices in VAO index buffer. */
	inline int GetIndexCount() const{return pIndexCount;}
	
	/** Set number of indices in VAO index buffer. */
	void SetIndexCount(int indexCount);
	
	/** Primitive type. */
	inline GLenum GetPrimitiveType() const{return pPrimitiveType;}
	
	/** Set primitive type. */
	void SetPrimitiveType(GLenum primitiveType);
	
	/** Patch verted count. */
	inline int GetTessPatchVertexCount() const{return pTessPatchVertexCount;}
	
	/** Set patch verted count. */
	void SetTessPatchVertexCount(int count);
	
	
	
	/** Count of sub instances to render. */
	int GetSubInstanceCount() const;
	
	/** Root sub instance. */
	decPointerLinkedList::cListEntry *GetRootSubInstance() const;
	
	/** Add sub instance. */
	deoglPersistentRenderTaskSubInstance *AddSubInstance(int indexInstance, int flags);
	
	/** Remove sub instance. */
	void RemoveSubInstance(deoglPersistentRenderTaskSubInstance *subInstance);
	
	/** Remove all sub instances. */
	void RemoveAllSubInstances();
	
	/** Sub instances SPB or NULL. */
	inline deoglSharedSPB *GetSubInstanceSPB() const{return pSubInstanceSPB;}
	
	/** Sub instances SPB Group or NULL. */
	inline const deoglSharedSPBRTIGroup *GetSubInstanceSPBGroup() const{return pSubInstanceSPBGroup;}
	
	/** Set Sub instances SPB and group. */
	void SetSubInstanceSPB(deoglSharedSPB *spb, const deoglSharedSPBRTIGroup *group);
	
	/** Sub instance index SPB. */
	inline deoglShaderParameterBlock *GetSIIndexInstanceSPB() const{return pSIIndexInstanceSPB;}
	
	/** Index of first sub instance index for instance shared SPB. */
	inline int GetSIIndexInstanceFirst() const{return pSIIndexInstanceFirst;}
	
	/** Set sub instance SPB information for later update. */
	void SetSIIndexInstanceParam(deoglShaderParameterBlock *paramBlock, int firstIndex);
	
	/** Write sub instance indices for instance shared SPB. */
	void WriteSIIndexInstanceInt(bool useFlags);
	
	/** Write sub instance indices for instance shared SPB. */
	void WriteSIIndexInstanceShort(bool useFlags);
	
	
	
	/** Clear. */
	void Clear();
	
	/** Remove from parent if empty. */
	void RemoveFromParentIfEmpty();
	
	
	
	/** Render task linked list. */
	inline decPointerLinkedList::cListEntry &GetLLVAO(){return pLLVAO;}
	inline const decPointerLinkedList::cListEntry &GetLLVAO() const{return pLLVAO;}
	/*@}*/
};

#endif
