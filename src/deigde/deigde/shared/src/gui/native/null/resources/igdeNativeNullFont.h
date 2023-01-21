/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDENATIVENULLFONT_H_
#define _IGDENATIVENULLFONT_H_

#include <dragengine/common/math/decMath.h>

class igdeFont;
class deFont;


/**
 * Null font.
 */
class igdeNativeNullFont{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native font. */
	igdeNativeNullFont();
	
	/** \brief Clean up native font. */
	virtual ~igdeNativeNullFont();
	
	/** \brief Create native font. */
	static igdeNativeNullFont* CreateNativeFont( igdeFont &owner );
	
	/** \brief Destroy native font. */
	virtual void DestroyNativeFont();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual deFont *CreateEngineFont();
	
	/** \brief Text size. */
	virtual decPoint TextSize( const char *text ) const;
	/*@}*/
};

typedef igdeNativeNullFont igdeNativeFont;

#endif
