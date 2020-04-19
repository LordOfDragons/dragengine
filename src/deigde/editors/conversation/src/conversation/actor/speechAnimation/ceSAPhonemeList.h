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

#ifndef _CESAPHONEMELIST_H_
#define _CESAPHONEMELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceSAPhoneme;



/**
 * \brief Speech Animation Phoneme List.
 */
class ceSAPhonemeList{
private:
	decObjectOrderedSet pPhonemes;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new phoneme list. */
	ceSAPhonemeList();
	/** Cleans up the phoneme list. */
	~ceSAPhonemeList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of phonemes. */
	int GetCount() const;
	/** Retrieves the phoneme at the given position. */
	ceSAPhoneme *GetAt( int index ) const;
	/** Retrieves the phoneme with the given ipa symbol or NULL if not found. */
	ceSAPhoneme *GetIPA( int ipa ) const;
	/** Retrieves the index of the given phoneme or -1 if not found. */
	int IndexOf( ceSAPhoneme *phoneme ) const;
	/** Retrieves the index of the phoneme with the given ipa symbol or -1 if not found. */
	int IndexOfIPA( int ipa ) const;
	/** Determines if a phoneme exists. */
	bool Has( ceSAPhoneme *phoneme ) const;
	/** Determines if a phoneme with the given ipa symbol exists. */
	bool HasIPA( int ipa ) const;
	/** Adds a phoneme. */
	void Add( ceSAPhoneme *phoneme );
	/** Removes a phoneme. */
	void Remove( ceSAPhoneme *phoneme );
	/** Removes all phonemes. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	ceSAPhonemeList &operator=( const ceSAPhonemeList &list );
	/*@}*/
};

#endif
