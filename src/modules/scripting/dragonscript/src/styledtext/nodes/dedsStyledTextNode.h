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
#ifndef _DEDSSTYLEDTEXTNODE_H_
#define _DEDSSTYLEDTEXTNODE_H_

// includes
#include <dragengine/deObject.h>

// predefinitions



/**
 * @brief Styled Text Node.
 * Node in a styled text. No parent node reference is held since
 * this class only stores the definition and working classes
 * derived thereof can find the parent using visiting.
 */
class dedsStyledTextNode : public deObject{
private:
	void *pStyle;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new styled text node. */
	dedsStyledTextNode();
	/** Cleans up the styled text node. */
	virtual ~dedsStyledTextNode();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the style or NULL to use the parent node style. */
	inline void *GetStyle() const{ return pStyle; }
	/** Sets the style or NULL to use the parent node style. */
	void SetStyle( void *style );
	/*@}*/
};

// end of include only once
#endif
