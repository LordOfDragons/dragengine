/* 
 * Drag[en]gine OpenAL Audio Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOALSHAREDEFFECTSLOTMANAGER_H_
#define _DEOALSHAREDEFFECTSLOTMANAGER_H_

#include "../deoalBasics.h"

#include <dragengine/common/collection/decObjectList.h>

class deoalSharedEffectSlot;
class deoalASpeaker;
class deoalEnvironment;


/**
 * Shared effect slot manager.
 */
class deoalSharedEffectSlotManager{
private:
	deoalAudioThread &pAudioThread;
	
	decObjectList pSlots;
	int pMaxCount;
	
	decObjectList pSpeakers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create openal sared effect slot manager. */
	deoalSharedEffectSlotManager( deoalAudioThread &audioThread );
	
	/** Clean up openal shared effect slot manager. */
	~deoalSharedEffectSlotManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** OpenAL module. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	/** Maximum shared effect slot count. */
	inline int GetMaxCount() const{ return pMaxCount; }
	
	/** Set maximum shared effect slot count. */
	void SetMaxCount( int count );
	
	/** Add speaker. */
	void AddSpeaker( deoalASpeaker *speaker );
	
	/** Clear speakers. */
	void ClearSpeakers();
	
	/** Assign speakers to shared effect slots. */
	void AssignSpeakers();
	
	/** Best matching shared effect slot or nullptr. */
	deoalSharedEffectSlot *BestMatchingSlot( const deoalEnvironment &environment,
		float &bestDistance ) const;
	
	/** First empty slot or nullptr. */
	deoalSharedEffectSlot *FirstEmptySlot() const;
	
	/** Drop effects. */
	void DropEffects();
	
	/** Debug log state. */
	void DebugLogState();
	/*@}*/
	
	
	
private:
	void pCalcSpeakerParamDistances();
	void pSortSpeakerParamByDistance( int left, int right );
	void pAssignRefSpeaker();
	void pAssignSpeakers();
};

#endif
