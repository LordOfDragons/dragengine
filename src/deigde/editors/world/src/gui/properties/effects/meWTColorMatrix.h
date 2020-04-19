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
#ifndef _MEWTCOLORMATRIX_H_
#define _MEWTCOLORMATRIX_H_

// includes
#include <dragengine/common/math/decMath.h>



/**
 * @brief Color Matrix Template.
 * Color Matrix Template for the world editor.
 */
class meWTColorMatrix{
private:
	char *pName;
	decColorMatrix pColorMatrix;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new color matrix template object. */
	meWTColorMatrix( const char *name, const decColorMatrix &matrix );
	/** Cleans up the color matrix template. */
	~meWTColorMatrix();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the name of the template. */
	inline const char *GetName() const{ return ( const char * )pName; }
	/** Retrieves the color matrix. */
	inline const decColorMatrix &GetColorMatrix() const{ return pColorMatrix; }
	/** Sets the color matrix. */
	void SetColorMatrix( const decColorMatrix &colorMatrix );
	/*@}*/
};

// end of include only once
#endif

#endif
