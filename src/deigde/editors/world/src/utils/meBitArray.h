/* 
 * Drag[en]gine IGDE World Editor
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

// include only once
#ifndef _MEBITARRAY_H_
#define _MEBITARRAY_H_

// includes
#include "dragengine/common/math/decMath.h"



/**
 * @brief Bit Array.
 *
 * Stores a rectangular grid of boolean values in form of a bit array.
 */
class meBitArray{
private:
	int pColons;
	int pRows;
	unsigned char *pBytes;
	int pByteCount;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new bit array. */
	meBitArray( int colons, int rows );
	/** Cleans up the bit array. */
	~meBitArray();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of colons. */
	inline int GetColons() const{ return pColons; }
	/** Retrieves the number of rows. */
	inline int GetRows() const{ return pRows; }
	/** Retrieves the boolean value at the given location. */
	bool GetValueAt( int x, int y ) const;
	/** Sets the boolean value at the given location. */
	void SetValueAt( int x, int y, bool value );
	/** Copies the values to another bit array. */
	void CopyTo( meBitArray &bitArray ) const;
	/** Clears all values. */
	void ClearTo( bool value );
	/*@}*/
};

// end of include only once
#endif
