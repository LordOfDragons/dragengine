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

#ifndef _DEOGLSHAREDSPB_H_
#define _DEOGLSHAREDSPB_H_

#include "../deoglShaderParameterBlock.h"

class deoglSharedSPBElement;
class deoglShaderParameterBlock;



/**
 * OpenGL Shared Shader Parameter Block.
 * 
 * Shared shader parameter blocks contain one or more elements sharing all the same
 * data layout. Each element can be updated individually in the parameter block.
 * Each shared parameter block can have a maximum number of elements. Elements are
 * added to open slots without moving. Hence the index of an element is not changed
 * after it has been created.
 * 
 * Main purpose of shared parameter blocks is to reduce the state changes and to allow
 * rendering multiple instances with different parameter blocks more efficiently.
 * 
 * This is the basic class. Subclasses define the actual type of backing storage used.
 */
class deoglSharedSPB : public deObject{
public:
	typedef deTObjectReference<deoglSharedSPB> Ref;
	
	
	
private:
	const deoglShaderParameterBlock::Ref pParameterBlock;
	deoglSharedSPBElement **pElements;
	int pSize;
	int pCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * Create shared shader parameter block.
	 * 
	 * \warning Do not modify the parameter block after creating the shared object.
	 */
	deoglSharedSPB( const deoglShaderParameterBlock::Ref &parameterBlock );
	
protected:
	/** Clean up shared shader parameter block. */
	virtual ~deoglSharedSPB();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Parameter block storing the elements. */
	inline const deoglShaderParameterBlock::Ref &GetParameterBlock() const{ return pParameterBlock; }
	
	/** Maximum number of elements. */
	inline int GetSize() const{ return pSize; }
	
	/** Number of stored elements. */
	inline int GetElementCount() const{ return pCount; }
	
	/** Element at index or \em NULL of not assigned. */
	deoglSharedSPBElement *GetElementAt( int index ) const;
	
	/**
	 * Obtain element.
	 * 
	 * Caller obtains reference to the element. Release reference if not used anymore.
	 * Element is removed from the shared parameter block once all references are released.
	 * 
	 * \returns element or \em NULL if the shared parameter block is full.
	 */
	deoglSharedSPBElement *AddElement();
	
	/**
	 * Remove element.
	 * 
	 * \warning For use by deoglSharedSPBElement only.
	 */
	void RemoveElement( int index );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
