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

#ifndef _DEOGLDYNAMICTBOSHARED_H_
#define _DEOGLDYNAMICTBOSHARED_H_

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/deObject.h>
#include <dragengine/deObjectReference.h>

#include "../deoglBasics.h"

class deoglDynamicTBO;
class deoglDynamicTBOBlock;


/**
 * Shared TBO containing one or more blocks.
 */
class deoglDynamicTBOShared : public deObject{
public:
	const deObjectReference pTBO;
	decObjectList pBlocks;
	int pUsedSize;
	bool pDirty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shared dynamic TBO. */
	deoglDynamicTBOShared( deoglDynamicTBO *tbo );
	
	/** Cleans up shared dynamic TBO. */
	virtual ~deoglDynamicTBOShared();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** TBO. */
	inline deoglDynamicTBO *GetTBO() const{ return ( deoglDynamicTBO* )( deObject* )pTBO; }
	
	/** Prepare TBO data if required. This does not upload the TBO data just prepare it. */
	void Prepare();
	
	/** Used size in pixel. */
	inline int GetUsedSize() const{ return pUsedSize; }
	
	/** Update used sizes in pixel. */
	void UpdateUsedSizes();
	
	/** Mark TBO dirty. */
	void MarkDirty();
	/*@}*/
	
	
	
	/** Count of blocks. */
	int GetBlockCount() const;
	
	/** Block at index. */
	deoglDynamicTBOBlock *GetBlockAt( int index ) const;
	
	/** Add block. Returns block or NULL if not enough space. */
	deoglDynamicTBOBlock *AddBlock( deoglDynamicTBO *tbo );
	
	/** Remove block returning the space to the pool of free space. */
	void RemoveBlock( deoglDynamicTBOBlock *block );
	
	/** Index of first empty block TBO fits in or NULL if not found. */
	int FirstMatchingBlock( deoglDynamicTBO *tbo );
	/*@}*/
};

#endif
