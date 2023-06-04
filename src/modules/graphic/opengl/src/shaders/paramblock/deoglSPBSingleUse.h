/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLSPBSINGLEUSE_H_
#define _DEOGLSPBSINGLEUSE_H_

#include <stdint.h>

#include "deoglShaderParameterBlock.h"

#include <dragengine/common/collection/decObjectList.h>


/**
 * Single use shader parameter block. Stores one or more parameter blocks in a ring list.
 * Each time Next() is called the next block from the list is returned. Upon render thread
 * frame counter change the start index is set to the current list position. If Next()
 * would end up at the start index after a full loop a new parameter block is inserted
 * as a clone of the first parameter block in the list.
 */
class deoglSPBSingleUse : public deObject{
public:
	typedef deTObjectReference<deoglSPBSingleUse> Ref;
	
	
	
private:
	const deoglRenderThread &pRenderThread;
	deoglShaderParameterBlock *pInitialBlock;
	decObjectList pBlocks;
	uint32_t pFrameCounter;
	int pStartIndex;
	int pNextIndex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Map block. */
	deoglSPBSingleUse( const deoglRenderThread &renderThread, deoglShaderParameterBlock *block );
	
protected:
	/** Unmap block if mapped. */
	virtual ~deoglSPBSingleUse();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Get next parameter block. Caller does not take over reference */
	deoglShaderParameterBlock *Next();
	/*@}*/
};

#endif
