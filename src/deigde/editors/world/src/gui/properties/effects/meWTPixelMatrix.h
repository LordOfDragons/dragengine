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

#if 0

// include only once
#ifndef _MEWTPIXELMATRIX_H_
#define _MEWTPIXELMATRIX_H_

// includes
#include <dragengine/common/math/decMath.h>



/**
 * @brief Pixel Matrix Template.
 * Pixel Matrix Template for the world editor.
 */
class meWTPixelMatrix{
private:
	char *pName;
	decTexMatrix pPixelMatrix;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new color matrix template object. */
	meWTPixelMatrix( const char *name, const decTexMatrix &matrix );
	/** Cleans up the color matrix template. */
	~meWTPixelMatrix();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the name of the template. */
	inline const char *GetName() const{ return ( const char * )pName; }
	/** Retrieves the color matrix. */
	inline const decTexMatrix &GetPixelMatrix() const{ return pPixelMatrix; }
	/** Sets the color matrix. */
	void SetPixelMatrix( const decTexMatrix &matrix );
	/*@}*/
};

// end of include only once
#endif

#endif
