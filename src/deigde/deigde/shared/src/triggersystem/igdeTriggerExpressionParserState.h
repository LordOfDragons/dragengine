/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDETRIGGEREXPRESSIONPARSERSTATE_H_
#define _IGDETRIGGEREXPRESSIONPARSERSTATE_H_

#include <dragengine/common/string/decString.h>



/**
 * \brief Trigger Expression Parser State.
 */
class DE_DLL_EXPORT igdeTriggerExpressionParserState{
private:
	decString pString;
	int pPosition;
	int pLength;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create trigger expression parser state. */
	igdeTriggerExpressionParserState( const char *string );
	
	/** \brief Clean up trigger expression parser state. */
	~igdeTriggerExpressionParserState();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Determines if there are more characters in the string. */
	bool HasMoreCharacters() const;
	
	/** \brief Position. */
	inline int GetPosition() const{ return pPosition; }
	
	/** \brief Next character in the string. */
	char GetNextCharacter();
	
	/** \brief Adjusts the position. */
	void AdjustPosition( int amount );
	/*@}*/
};

#endif
