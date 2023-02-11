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

#ifndef _DEOGLSHAREDBLOCKSPB_H_
#define _DEOGLSHAREDBLOCKSPB_H_

#include "../deoglShaderParameterBlock.h"

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/collection/decPointerList.h>

class deoglSharedBlockSPBElement;
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
class deoglSharedBlockSPB : public deObject{
public:
	typedef deTObjectReference<deoglSharedBlockSPB> Ref;
	
	
	
private:
	const deoglShaderParameterBlock::Ref pParameterBlock;
	decObjectList pElements;
	decPointerList pEmptyElements;
	int pUsedElementCount;
	int pFreeElementCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * Create shared shader parameter block.
	 * 
	 * \warning Do not modify the parameter block after creating the shared object.
	 */
	deoglSharedBlockSPB( const deoglShaderParameterBlock::Ref &parameterBlock );
	
protected:
	/** Clean up shared shader parameter block. */
	virtual ~deoglSharedBlockSPB();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Parameter block storing the elements. */
	inline const deoglShaderParameterBlock::Ref &GetParameterBlock() const{ return pParameterBlock; }
	
	/**
	 * Obtain element. Call deoglSharedBlockSPBElement::Return to return element.
	 */
	deoglSharedBlockSPBElement *GetElement();
	deoglSharedBlockSPBElement *GetElement( int count );
	
	/**
	 * Return element.
	 */
	void ReturnElement( deoglSharedBlockSPBElement *element );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	int pIndexOfEmptyElementWithMinCount( int count );
};

#endif
