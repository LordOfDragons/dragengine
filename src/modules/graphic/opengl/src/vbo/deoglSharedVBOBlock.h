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

#ifndef _DEOGLSHAREDVBOBLOCK_H_
#define _DEOGLSHAREDVBOBLOCK_H_

#include <dragengine/deObject.h>

class deoglSharedVBO;



/**
 * OpenGL shared VBO block.
 * 
 * Shared VBO composes of blocks. Blocks can be either in use by an object in the game
 * or it can be empty. Furthermore the block can be valid or invalid. A block is valid
 * if the data it contains has been copied into the VBO and is ready to be used. If invalid
 * the block data has first to be written to the VBO before it can be used. If the block is
 * not empty a data buffer is provided. In this case the owning game object has to fill the
 * data into the buffer and invalidate the block using SetValid(false). The offset and size
 * of the block are measured in points not bytes. Hence the actual byte offset and size is
 * ofset/size * layout.stride().
 */
class deoglSharedVBOBlock : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglSharedVBOBlock> Ref;
	
	
	deoglSharedVBO *pVBO;
	int pOffset;
	int pSize;
	unsigned char *pData;
	int pIndexOffset;
	int pIndexCount;
	unsigned char *pIndexData;
	bool pValid;
	bool pEmpty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create empty shared vbo block. */
	deoglSharedVBOBlock( deoglSharedVBO *vbo, int offset, int size, int indexOffset, int indexCount );
	
	/** Clean up shared vbo. */
	virtual ~deoglSharedVBOBlock();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** VBO this block belongs to. */
	inline deoglSharedVBO *GetVBO() const{ return pVBO; }
	
	/** Drop VBO. For internal use only. */
	void DropVBO();
	
	/** Delayed remove block. */
	void DelayedRemove();
	
	
	
	/** Offset to block (points). */
	inline int GetOffset() const{ return pOffset; }
	
	/** Size of block. */
	inline int GetSize() const{ return pSize; }
	
	/** Set size of block. Sets data pointer to NULL. */
	void SetSize( int size );
	
	/** Data pointer or NULL if block is empty. */
	inline unsigned char *GetData() const{ return pData; }
	
	
	
	/** Offset to index block. */
	inline int GetIndexOffset() const{ return pIndexOffset; }
	
	/** Count of indices in block. */
	inline int GetIndexCount() const{ return pIndexCount; }
	
	/** Set count of indices in block. Sets index data pointer to NULL. */
	void SetIndexCount( int count );
	
	/** Index data pointer or NULL if block is empty. */
	inline unsigned char *GetIndexData() const{ return pIndexData; }
	
	
	
	/** Block is empty. */
	inline bool GetEmpty() const{ return pEmpty; }
	
	/** Set if block is empty. */
	void SetEmpty( bool empty );
	
	/** Block is valid. */
	inline bool GetValid() const{ return pValid; }
	
	/** Set if block is valid. */
	void SetValid( bool valid );
	
	/** Convenience call to prepare the parent VBO if required. */
	void Prepare();
	/*@}*/
	
	
	
private:
	void pReallocData();
};

#endif
