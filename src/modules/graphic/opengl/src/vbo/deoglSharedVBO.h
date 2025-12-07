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

#ifndef _DEOGLSHAREDVBO_H_
#define _DEOGLSHAREDVBO_H_

#include "deoglVBOLayout.h"
#include "../deoglBasics.h"
#include "../memory/consumption/deoglMemoryConsumptionGPUUse.h"

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/deObject.h>

class deoglSharedVBOBlock;
class deoglSharedVBOList;
class deoglVAO;


/**
 * OpenGL Shared VBO.
 * 
 * Shared VBO contain one or more blocks of data belonging to different resources. Each VBO
 * has one VAO assigned to reduce the VAO switching. Shared VBOs can be used in a static or
 * dynamic way but all data has to fit into one VBO.
 */
class deoglSharedVBO : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglSharedVBO> Ref;
	
	
	deoglSharedVBOList *pParentList;
	GLuint pVBO;
	GLuint pIBO;
	deoglVAO *pVAO;
	decObjectList pBlocks;
	int pSize;
	int pUsedSize;
	int pIndexSize;
	int pIndexUsedSize;
	bool pDirty;
	
	deoglMemoryConsumptionGPUUse pMemUseVBO;
	deoglMemoryConsumptionGPUUse pMemUseIBO;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new shared vbo. */
	deoglSharedVBO(deoglSharedVBOList *parentList, int size, int indexSize);
	/** Cleans up the shared vbo. */
	virtual ~deoglSharedVBO();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the parent list. */
	inline deoglSharedVBOList *GetParentList() const{return pParentList;}
	/** Prepare if required. */
	void Prepare();
	/** Retrieves the VBO. */
	inline GLuint GetVBO() const{return pVBO;}
	/** Retrieves the IBO. */
	inline GLuint GetIBO() const{return pIBO;}
	/** Retrieves the VAO. */
	inline deoglVAO *GetVAO() const{return pVAO;}
	/** Retrieves the used size. */
	inline int GetUsedSize() const{return pUsedSize;}
	/** Retrieves the size. */
	inline int GetSize() const{return pSize;}
	/** Retrieves the used index size. */
	inline int GetIndexUsedSize() const{return pIndexUsedSize;}
	/** Retrieves the index size. */
	inline int GetIndexSize() const{return pIndexSize;}
	/** Updates the used sizes. */
	void UpdateUsedSizes();
	/** Marks the VBO dirty. */
	void MarkDirty();
	
	/** Memory consumption. */
	inline const deoglMemoryConsumptionGPUUse &GetMemoryConsumptionVBO() const{return pMemUseVBO;}
	inline const deoglMemoryConsumptionGPUUse &GetMemoryConsumptionIBO() const{return pMemUseIBO;}
	/*@}*/
	
	/** \name Data Management */
	/*@{*/
	/** Retrieves the number of blocks. */
	int GetBlockCount() const;
	/** Retrieves the block at the given location. */
	deoglSharedVBOBlock *GetBlockAt(int index) const;
	
	/**
	 * Tries to add a block of data to the VBO. Returns the block representing this data if a suitable
	 * location has been found or NULL if there is not enough space left in the VBO.
	 */
	deoglSharedVBOBlock *AddBlock(int size, int indexCount = 0);
	/** Removes a block of data returning the space to the pool of free space. */
	void RemoveBlock(deoglSharedVBOBlock *block);
	
	/** Index of first empty block with minimum size and index count available or NULL if not found. */
	int IndexOfEmptyBlockWithMinSize(int size, int indexCount);
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
