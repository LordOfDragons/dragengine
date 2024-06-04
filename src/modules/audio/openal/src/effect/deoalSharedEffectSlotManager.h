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
