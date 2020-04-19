/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CETEXTBOXTEXTLIST_H_
#define _CETEXTBOXTEXTLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceTextBoxText;



/**
 * \brief Text Box Text List.
 */
class ceTextBoxTextList{
private:
	decObjectOrderedSet pTexts;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new text list. */
	ceTextBoxTextList();
	/** Cleans up the text list. */
	~ceTextBoxTextList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of texts. */
	int GetCount() const;
	/** Retrieves the text at the given position. */
	ceTextBoxText *GetAt( int index ) const;
	/** Retrieves the index of the given text or -1 if not found. */
	int IndexOf( ceTextBoxText *text ) const;
	/** Determines if a text exists. */
	bool Has( ceTextBoxText *text ) const;
	/** Adds a text. */
	void Add( ceTextBoxText *text );
	/** Inserts a new text. */
	void InsertAt( ceTextBoxText *text, int index );
	/** Moves a text to a new position. */
	void MoveTo( ceTextBoxText *text, int index );
	/** Removes a text. */
	void Remove( ceTextBoxText *text );
	/** Removes all texts. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	ceTextBoxTextList &operator=( const ceTextBoxTextList &list );
	/*@}*/
};

#endif
