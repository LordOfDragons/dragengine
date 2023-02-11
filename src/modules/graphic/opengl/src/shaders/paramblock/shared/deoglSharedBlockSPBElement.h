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

#ifndef _DEOGLSHAREDBLOCKSPBELEMENT_H_
#define _DEOGLSHAREDBLOCKSPBELEMENT_H_

#include <dragengine/deObject.h>

class deoglSharedBlockSPB;
class deoglShaderParameterBlock;



/**
 * OpenGL shared shader parameter block element.
 */
class deoglSharedBlockSPBElement : public deObject{
public:
	typedef deTObjectReference<deoglSharedBlockSPBElement> Ref;
	
	
	
private:
	deoglSharedBlockSPB &pSPB;
	int pIndex;
	int pCount;
	bool pEmpty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shared shader parameter block element. */
	deoglSharedBlockSPBElement( deoglSharedBlockSPB &spb, int index, int count );
	
	/** Clean up shared shader parameter block element. */
	virtual ~deoglSharedBlockSPBElement();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Shader parameter block this element belongs to. */
	inline deoglSharedBlockSPB &GetSPB() const{ return pSPB; }
	
	/** Element index in the shader parameter block. */
	inline int GetIndex() const{ return pIndex; }
	
	/** Count of elements. */
	inline int GetCount() const{ return pCount; }
	
	/** Set count of elements. */
	void SetCount( int count );
	
	/** Block is empty. */
	inline bool GetEmpty() const{ return pEmpty; }
	
	/** Set if block is empty. */
	void SetEmpty( bool empty );
	
	/**
	 * Map buffer for element.
	 * 
	 * Call UnmapBuffer() on the shader parameter block once finished. You can call
	 * all methods without explicit element index to write into the mapped element
	 * while the buffer is mapped.
	 */
	deoglShaderParameterBlock &MapBuffer() const;
	
	/** Return to shared SPB. */
	void Return();
	/*@}*/
};

#endif
