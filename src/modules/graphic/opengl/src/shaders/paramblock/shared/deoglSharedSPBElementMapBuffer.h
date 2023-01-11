/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLSHAREDSPBELEMENTMAPBUFFER_H_
#define _DEOGLSHAREDSPBELEMENTMAPBUFFER_H_

class deoglSharedSPBElement;
class deoglShaderParameterBlock;


/**
 * Map shader parameter block while in scope. Allows manual mapping and unmapping.
 */
class deoglSharedSPBElementMapBuffer{
private:
	deoglSharedSPBElement &pElement;
	deoglShaderParameterBlock *pBlock;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Map block. */
	deoglSharedSPBElementMapBuffer( deoglSharedSPBElement &element );
	
	/** Unmap block if mapped. */
	~deoglSharedSPBElementMapBuffer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Map block if not mapped. */
	void Map();
	
	/** Block or nullptr if not mapped. */
	inline deoglShaderParameterBlock *GetBlock() const{ return pBlock; }
	
	/** Block or throws exception if not mapped. */
	deoglShaderParameterBlock &GetBlockRef() const;
	
	/** Unmap block if mapped. */
	void Unmap();
	/*@}*/
};

#endif
