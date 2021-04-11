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

#ifndef _DEOGLDYNAMICTBOBLOCK_H_
#define _DEOGLDYNAMICTBOBLOCK_H_

#include <dragengine/deObject.h>
#include <dragengine/deObjectReference.h>

class deoglDynamicTBOShared;
class deoglDynamicTBO;


/**
 * Dynamic TBO block. Blocks can be either in use or empty as well as valid or invalid.
 * A block is valid if the data it contains has been copied into the TBO and is ready
 * to be used. If invalid the block data has first to be written to the TBO before it
 * can be used. If the block is not empty a data buffer is provided. In this case the
 * owner has to fill the data into the buffer and invalidate the block using
 * SetValid(false). The offset and size of the block are measured in data units.
 */
class deoglDynamicTBOBlock : public deObject{
public:
	deoglDynamicTBOShared *pSharedTBO;
	int pOffset;
	int pSize;
	deObjectReference pData;
	deObjectReference pData2;
	bool pValid;
	bool pEmpty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create empty dynamic tbo block. */
	deoglDynamicTBOBlock( deoglDynamicTBOShared *tbo );
	
	/** Create empty dynamic tbo block. */
	deoglDynamicTBOBlock( deoglDynamicTBOShared *tbo, int offset, int size );
	
	/** Clean up dynamic tbo block. */
	virtual ~deoglDynamicTBOBlock();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Shared TBO containing block or NULL. */
	inline deoglDynamicTBOShared *GetSharedTBO() const{ return pSharedTBO; }
	
	/** Drop shared TBO. */
	void DropSharedTBO();
	
	
	
	/**
	 * Offset to block measured in units.
	 * \warning Can potentially change during block lifetime.
	 */
	inline int GetOffset() const{ return pOffset; }
	
	/** Set offset to block measured in units. */
	void SetOffset( int offset );
	
	/** Size of block in units. */
	inline int GetSize() const{ return pSize; }
	
	/** Set size of block in units. Also sets data TBO to NULL. */
	void SetSize( int size );
	
	/** TBO containing the data to use for the block or NULL if empty. */
	inline deoglDynamicTBO *GetData() const{ return ( deoglDynamicTBO* )( deObject* )pData; }
	inline deoglDynamicTBO *GetData2() const{ return ( deoglDynamicTBO* )( deObject* )pData2; }
	
	/** Set TBO containing the data to use for the block or NULL if empty. */
	void SetData( deoglDynamicTBO *tbo, deoglDynamicTBO *tbo2 = NULL );
	
	/** Block is empty. */
	inline bool GetEmpty() const{ return pEmpty; }
	
	/** Set if block is empty. */
	void SetEmpty( bool empty );
	
	/** Block is valid. */
	inline bool GetValid() const{ return pValid; }
	
	/** Set if block is valid. */
	void SetValid( bool valid );
	
	/**
	 * Drop block removing it from owner shared TBO returning it to free memory pool.
	 * This call is only valid for non-empty blocks. For empty blocks nothing is done.
	 */
	void Drop();
	/*@}*/
};

#endif
