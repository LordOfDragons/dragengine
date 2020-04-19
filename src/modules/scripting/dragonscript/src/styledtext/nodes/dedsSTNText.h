/* 
 * Drag[en]gine DragonScript Script Module
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
#ifndef _DEDSSTNTEXT_H_
#define _DEDSSTNTEXT_H_

// includes
#include "dedsStyledTextNode.h"

// predefinitions



/**
 * @brief Styled Text Node Text.
 * Text node in a styled text object storing a single text string.
 * Line breaks are honored.
 */
class dedsSTNText : public dedsStyledTextNode{
private:
	char *pText;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new text node. */
	dedsSTNText();
	/** Creates a new text node with the given text. */
	dedsSTNText( const char *text );
	/** Cleans up the text node. */
	virtual ~dedsSTNText();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the text. */
	inline const char *GetText() const{ return ( const char * )pText; }
	/** Sets the text. */
	void SetText( const char *text );
	/*@}*/
};

// end of include only once
#endif
