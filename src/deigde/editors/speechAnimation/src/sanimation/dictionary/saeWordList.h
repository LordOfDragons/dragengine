/* 
 * Drag[en]gine IGDE Speech Animation Editor
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

#ifndef _SAEWORDLIST_H_
#define _SAEWORDLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class saeWord;



/**
 * Speech animation word list.
 */
class saeWordList{
private:
	decObjectOrderedSet pWords;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create word list. */
	saeWordList();
	
	/** Create copy of word list. */
	saeWordList( const saeWordList &list );
	
	/** Clean up word list. */
	~saeWordList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Number of words. */
	int GetCount() const;
	
	/** Word at index. */
	saeWord *GetAt( int index ) const;
	
	/** Named word or NULL if absent. */
	saeWord *GetNamed( const char *name ) const;
	
	/** Index of word or -1 if absent. */
	int IndexOf( saeWord *word ) const;
	
	/** Index of named word or -1 if absent. */
	int IndexOfNamed( const char *name ) const;
	
	/** Word is present. */
	bool Has( saeWord *word ) const;
	
	/** Named word is present. */
	bool HasNamed( const char *name ) const;
	
	/** Add word. */
	void Add( saeWord *word );
	
	/** Remove word. */
	void Remove( saeWord *word );
	
	/** Remove all words. */
	void RemoveAll();
	
	/** Set list. */
	saeWordList &operator=( const saeWordList &list );
	/*@}*/
};

#endif
