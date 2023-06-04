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

#ifndef _DEOGLSPBMAPBUFFERREAD_H_
#define _DEOGLSPBMAPBUFFERREAD_H_

class deoglSPBlockSSBO;


/**
 * Map shader parameter block for reading while in scope. Allows manual mapping and unmapping.
 */
class deoglSPBMapBufferRead{
private:
	deoglSPBlockSSBO &pBlock;
	const int pElement;
	const int pCount;
	bool pMapped;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Map block. */
	deoglSPBMapBufferRead( deoglSPBlockSSBO &block );
	
	/** Map block. */
	deoglSPBMapBufferRead( deoglSPBlockSSBO &block, int element );
	
	/** Map block. */
	deoglSPBMapBufferRead( deoglSPBlockSSBO &block, int element, int count );
	
	/** Unmap block if mapped. */
	~deoglSPBMapBufferRead();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Map block if not mapped. */
	void Map();
	
	/** Unmap block if mapped. */
	void Unmap();
	/*@}*/
};

#endif
