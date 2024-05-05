/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
