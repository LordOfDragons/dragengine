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

#ifndef _DEOGLSHAREDVBOBLOCK_H_
#define _DEOGLSHAREDVBOBLOCK_H_

#include <dragengine/deObject.h>

class deoglSharedVBO;



/**
 * \brief OpenGL shared VBO block.
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
	/** \brief Create empty shared vbo block. */
	deoglSharedVBOBlock( deoglSharedVBO *vbo );
	
	/** \brief Create empty shared vbo block. */
	deoglSharedVBOBlock( deoglSharedVBO *vbo, int offset, int size );
	
	/** \brief Create empty shared vbo block. */
	deoglSharedVBOBlock( deoglSharedVBO *vbo, int offset, int size, int indexOffset, int indexCount );
	
	/** \brief Clean up shared vbo. */
	virtual ~deoglSharedVBOBlock();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief VBO this block belongs to. */
	inline deoglSharedVBO *GetVBO() const{ return pVBO; }
	
	
	
	/**
	 * \brief Offset to block (points).
	 * \warning Can potentially change during block lifetime. Do not store in class variables.
	 */
	inline int GetOffset() const{ return pOffset; }
	
	/** \brief Set offset to block (offset to points). */
	void SetOffset( int offset );
	
	/** \brief Size of block. */
	inline int GetSize() const{ return pSize; }
	
	/**
	 * \brief Set size of block.
	 * 
	 * Also sets data pointer to NULL.
	 */
	void SetSize( int size );
	
	/** \brief Data pointer or NULL if block is empty. */
	inline unsigned char *GetData() const{ return pData; }
	
	
	
	/**
	 * \brief Offset to index block.
	 * \warning Can potentially change during block lifetime. Do not store in class variables.
	 */
	inline int GetIndexOffset() const{ return pIndexOffset; }
	
	/** \brief Set offset to index block. */
	void SetIndexOffset( int offset );
	
	/** \brief Number of indices in block. */
	inline int GetIndexCount() const{ return pIndexCount; }
	
	/**
	 * \brief Set number of indices in block.
	 * 
	 * Also sets index data pointer to NULL.
	 */
	void SetIndexCount( int count );
	
	/** \brief Index data pointer or NULL if block is empty. */
	inline unsigned char *GetIndexData() const{ return pIndexData; }
	
	
	
	/** \brief Block is empty. */
	inline bool GetEmpty() const{ return pEmpty; }
	
	/** \brief Set if block is empty. */
	void SetEmpty( bool empty );
	
	/** \brief Block is valid. */
	inline bool GetValid() const{ return pValid; }
	
	/** \brief Set if block is valid. */
	void SetValid( bool valid );
	
	/** \brief Convenience call to prepare the parent VBO if required. */
	void Prepare();
	/*@}*/
	
	
	
private:
	void pReallocData();
};

#endif
