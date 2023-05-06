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

#ifndef _SAEPHONEMELIST_H_
#define _SAEPHONEMELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class saePhoneme;



/**
 * Speech Animation Phoneme List.
 */
class saePhonemeList{
private:
	decObjectOrderedSet pPhonemes;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new phoneme list. */
	saePhonemeList();
	/** Cleans up the phoneme list. */
	~saePhonemeList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of phonemes. */
	int GetCount() const;
	/** Retrieves the phoneme at the given position. */
	saePhoneme *GetAt( int index ) const;
	/** Retrieves the phoneme with the given ipa symbol or NULL if not found. */
	saePhoneme *GetIPA( int ipa ) const;
	/** Retrieves the index of the given phoneme or -1 if not found. */
	int IndexOf( saePhoneme *phoneme ) const;
	/** Retrieves the index of the phoneme with the given ipa symbol or -1 if not found. */
	int IndexOfIPA( int ipa ) const;
	/** Determines if a phoneme exists. */
	bool Has( saePhoneme *phoneme ) const;
	/** Determines if a phoneme with the given ipa symbol exists. */
	bool HasIPA( int ipa ) const;
	/** Adds a phoneme. */
	void Add( saePhoneme *phoneme );
	/** Removes a phoneme. */
	void Remove( saePhoneme *phoneme );
	/** Removes all phonemes. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	saePhonemeList &operator=( const saePhonemeList &list );
	/*@}*/
};

#endif
