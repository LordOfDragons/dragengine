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

#ifndef _DEOGLSHAREDVBO_H_
#define _DEOGLSHAREDVBO_H_

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/deObject.h>

#include "../deoglBasics.h"
#include "deoglVBOLayout.h"

class deoglSharedVBOBlock;
class deoglSharedVBOList;
class deoglVAO;


/**
 * \brief OpenGL Shared VBO.
 * 
 * Shared VBO contain one or more blocks of data belonging to different resources. Each VBO
 * has one VAO assigned to reduce the VAO switching. Shared VBOs can be used in a static or
 * dynamic way but all data has to fit into one VBO.
 */
class deoglSharedVBO : public deObject{
public:
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
	
	int pMemoryGPUVBO;
	int pMemoryGPUIBO;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new shared vbo. */
	deoglSharedVBO( deoglSharedVBOList *parentList, int size );
	/** Cleans up the shared vbo. */
	virtual ~deoglSharedVBO();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the parent list. */
	inline deoglSharedVBOList *GetParentList() const{ return pParentList; }
	/** Prepare if required. */
	void Prepare();
	/** Retrieves the VBO. */
	inline GLuint GetVBO() const{ return pVBO; }
	/** Retrieves the IBO. */
	inline GLuint GetIBO() const{ return pIBO; }
	/** Retrieves the VAO. */
	inline deoglVAO *GetVAO() const{ return pVAO; }
	/** Retrieves the used size. */
	inline int GetUsedSize() const{ return pUsedSize; }
	/** Retrieves the size. */
	inline int GetSize() const{ return pSize; }
	/** Retrieves the used index size. */
	inline int GetIndexUsedSize() const{ return pIndexUsedSize; }
	/** Retrieves the index size. */
	inline int GetIndexSize() const{ return pIndexSize; }
	/** Updates the used sizes. */
	void UpdateUsedSizes();
	/** Marks the VBO dirty. */
	void MarkDirty();
	
	/** Retrieves the GPU memory consumption for the VBO. */
	inline int GetMemoryConsumptionGPUVBO() const{ return pMemoryGPUVBO; }
	/** Retrieves the GPU memory consumption for the IBO. */
	inline int GetMemoryConsumptionGPUIBO() const{ return pMemoryGPUIBO; }
	/*@}*/
	
	/** \name Data Management */
	/*@{*/
	/** Retrieves the number of blocks. */
	int GetBlockCount() const;
	/** Retrieves the block at the given location. */
	deoglSharedVBOBlock *GetBlockAt( int index ) const;
	/**
	 * Tries to add a block of data to the VBO. Returns the block representing this data if a suitable
	 * location has been found or NULL if there is not enough space left in the VBO.
	 */
	deoglSharedVBOBlock *AddBlock( int size );
	/**
	 * Tries to add a block of data to the VBO. Returns the block representing this data if a suitable
	 * location has been found or NULL if there is not enough space left in the VBO.
	 */
	deoglSharedVBOBlock *AddBlock( int size, int indexCount );
	/** Removes a block of data returning the space to the pool of free space. */
	void RemoveBlock( deoglSharedVBOBlock *block );
	/** Retrieves the index of the first empty block with at least the given amount of size or NULL if not found. */
	int IndexOfEmptyBlockWithMinSize( int size );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
