/* 
 * Drag[en]gine GUI Launcher
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
#ifndef _DEGLSTYLE_H_
#define _DEGLSTYLE_H_

// includes
#include <dragengine/deObject.h>

#include <dragengine/common/math/decMath.h>



/**
 * @brief Style Color.
 * Defines a color in a style. Stores the color in the range from 0 to 1 as
 * float values. Stores red, green, blue and alpha.
 */
class deglStyle : public deObject{
private:
	decColor pColorBackground;
	decColor pColorText;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new style. */
	deglStyle();
	/** Cleans up the style. */
	virtual ~deglStyle();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the background color. */
	inline const decColor &GetColorBackground() const{ return pColorBackground; }
	/** Sets the background color. */
	void SetColorBackground( const decColor &color );
	/** Retrieves the text color. */
	inline const decColor &GetColorText() const{ return pColorText; }
	/** Sets the text color. */
	void SetColorText( const decColor &color );
	/*@}*/
};

// end of include only once
#endif
