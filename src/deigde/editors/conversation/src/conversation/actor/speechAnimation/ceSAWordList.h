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

#ifndef _CESAWORDLIST_H_
#define _CESAWORDLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceSAWord;



/**
 * \brief Speech Animation Word List.
 */
class ceSAWordList{
private:
	decObjectOrderedSet pWords;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new word list. */
	ceSAWordList();
	/** Cleans up the word list. */
	~ceSAWordList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of words. */
	int GetCount() const;
	/** Retrieves the word at the given position. */
	ceSAWord *GetAt( int index ) const;
	/** Retrieves the word with the given name or NULL if not found. */
	ceSAWord *GetNamed( const char *name ) const;
	/** Retrieves the index of the given word or -1 if not found. */
	int IndexOf( ceSAWord *word ) const;
	/** Retrieves the index of the word with the given name or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** Determines if a word exists. */
	bool Has( ceSAWord *word ) const;
	/** Determines if a word with the given name exists. */
	bool HasNamed( const char *name ) const;
	/** Adds a word. */
	void Add( ceSAWord *word );
	/** Removes a word. */
	void Remove( ceSAWord *word );
	/** Removes all words. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	ceSAWordList &operator=( const ceSAWordList &list );
	/*@}*/
};

#endif
