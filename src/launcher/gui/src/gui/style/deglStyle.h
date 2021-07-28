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

#ifndef _DEGLSTYLE_H_
#define _DEGLSTYLE_H_

#include <dragengine/deObject.h>

#include <dragengine/common/math/decMath.h>


/**
 * Style Color. Defines a color in a style. Stores the color in the range from 0 to 1 as
 * float values. Stores red, green, blue and alpha.
 */
class deglStyle : public deObject{
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<deglStyle> Ref;
	
	
	
private:
	decColor pColorBackground;
	decColor pColorText;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create style. */
	deglStyle();
	
protected:
	/** Clean up style. */
	virtual ~deglStyle();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Background color. */
	inline const decColor &GetColorBackground() const{ return pColorBackground; }
	
	/** Set background color. */
	void SetColorBackground( const decColor &color );
	
	/** Text color. */
	inline const decColor &GetColorText() const{ return pColorText; }
	
	/** Set text color. */
	void SetColorText( const decColor &color );
	/*@}*/
};

#endif
