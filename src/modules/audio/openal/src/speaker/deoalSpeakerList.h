/* 
 * Drag[en]gine OpenAL Audio Module
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

#ifndef _DEOALSPEAKERLIST_H_
#define _DEOALSPEAKERLIST_H_

#include <dragengine/common/collection/decPointerList.h>

class deoalASpeaker;



/**
 * Speaker list.
 */
class deoalSpeakerList{
private:
	decPointerList pSpeakers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create speaker list. */
	deoalSpeakerList();
	
	/** Clean up speaker list. */
	~deoalSpeakerList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Number of speakers. */
	int GetCount() const;
	
	/** Speaker at index. */
	deoalASpeaker *GetAt( int index ) const;
	
	/** Speaker is present. */
	bool Has( deoalASpeaker *speaker ) const;
	
	/** Index of speaker or -1 if absent. */
	int IndexOf( deoalASpeaker *speaker ) const;
	
	/** Add speaker. */
	void Add( deoalASpeaker *speaker );
	
	/** Remove speaker. */
	void Remove( deoalASpeaker *speaker );
	
	/** Remove speaker if present. */
	void RemoveIfExisting( deoalASpeaker *speaker );
	
	/** Remove all speakers. */
	void RemoveAll();
	
	
	
	/** Flag all speakers. */
	void FlagAll( bool flag );
	
	/**
	 * Set enabled state for all speakers.
	 * \warning Called during synchronization time from main thread.
	 */
	void EnableAll( bool enabled );
	
	/**
	 * Set enabled state of all speakers to match their flag state.
	 * \warning Called during synchronization time from main thread.
	 */
	void FlagToEnabledAll();
	
	/** Update all speakers. */
	void UpdateAll();
	
	/** Update effects all speakers. */
	void UpdateEffectsAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Assign list. */
	deoalSpeakerList &operator=( const deoalSpeakerList &list );
	
	/** Append list. */
	deoalSpeakerList &operator+=( const deoalSpeakerList &list );
	
	/** Combined list. */
	deoalSpeakerList operator+( const deoalSpeakerList &list ) const;
	
	/** Lists are equal. */
	bool operator==( const deoalSpeakerList &list ) const;
	
	/** Lists are not equal. */
	bool operator!=( const deoalSpeakerList &list ) const;
	/*@}*/
};

#endif
