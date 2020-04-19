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
 * \brief Speaker list.
 */
class deoalSpeakerList{
private:
	decPointerList pSpeakers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create speaker list. */
	deoalSpeakerList();
	
	/** \brief Clean up speaker list. */
	~deoalSpeakerList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of speakers. */
	int GetCount() const;
	
	/** \brief Speaker at index. */
	deoalASpeaker *GetAt( int index ) const;
	
	/** \brief Speaker is present. */
	bool Has( deoalASpeaker *speaker ) const;
	
	/** \brief Index of speaker or -1 if absent. */
	int IndexOf( deoalASpeaker *speaker ) const;
	
	/** \brief Add speaker. */
	void Add( deoalASpeaker *speaker );
	
	/** \brief Remove speaker. */
	void Remove( deoalASpeaker *speaker );
	
	/** \brief Remove speaker if present. */
	void RemoveIfExisting( deoalASpeaker *speaker );
	
	/** \brief Remove all speakers. */
	void RemoveAll();
	
	
	
	/** \brief Flag all speakers. */
	void FlagAll( bool flag );
	
	/**
	 * \brief Set enabled state for all speakers.
	 * \warning Called during synchronization time from main thread.
	 */
	void EnableAll( bool enabled );
	
	/**
	 * \brief Set enabled state of all speakers to match their flag state.
	 * \warning Called during synchronization time from main thread.
	 */
	void FlagToEnabledAll();
	
	/** \brief Update all speakers. */
	void UpdateAll();
	
	/** \brief Update effects all speakers. */
	void UpdateEffectsAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Assign list. */
	deoalSpeakerList &operator=( const deoalSpeakerList &list );
	
	/** \brief Append list. */
	deoalSpeakerList &operator+=( const deoalSpeakerList &list );
	
	/** \brief Combined list. */
	deoalSpeakerList operator+( const deoalSpeakerList &list ) const;
	
	/** \brief Lists are equal. */
	bool operator==( const deoalSpeakerList &list ) const;
	
	/** \brief Lists are not equal. */
	bool operator!=( const deoalSpeakerList &list ) const;
	/*@}*/
};

#endif
