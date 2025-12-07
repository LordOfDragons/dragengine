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

#ifndef _DEOALSHAREDEFFECTSLOT_H_
#define _DEOALSHAREDEFFECTSLOT_H_

#include "deoalEffectSlot.h"

#include <dragengine/common/collection/decObjectList.h>


class deoalASpeaker;
class deoalEnvironment;


/**
 * OpenAL shared effect slot.
 */
class deoalSharedEffectSlot : public deObject{
public:
	typedef deTObjectReference<deoalSharedEffectSlot> Ref;
	
	
private:
	deoalAudioThread &pAudioThread;
	
	deoalEffectSlot::Ref pEffectSlot;
	
	decObjectList pSpeakers;
	deoalASpeaker *pRefSpeaker;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create openal shared effect slot. */
	deoalSharedEffectSlot(deoalAudioThread &audioThread);
	
protected:
	/** Clean up openal shared effect slot. */
	virtual ~deoalSharedEffectSlot();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** OpenAL module. */
	inline deoalAudioThread &GetAudioThread() const{return pAudioThread;}
	
	/** Effect slot. */
	const deoalEffectSlot::Ref &GetEffectSlot();
	
	/** Drop effect slot. */
	void DropEffectSlot();
	
	/** Speaker count. */
	int GetSpeakerCount() const;
	
	/** Speaker at index. */
	deoalASpeaker *GetSpeakerAt(int index) const;
	
	/** Add speaker. */
	void AddSpeaker(deoalASpeaker *speaker);
	
	/** Remove speaker. */
	void RemoveSpeaker(deoalASpeaker *speaker);
	
	/** Remove all speakers. */
	void RemoveAllSpeakers();
	
	/** Move speaker to first position and make it reference. */
	void MoveSpeakerFront(deoalASpeaker *speaker);
	
	/** Reference speaker or nullptr. */
	inline deoalASpeaker *GetReferenceSpeaker() const{return pRefSpeaker;}
	
	/** Update effect slot. */
	void UpdateEffectSlot();
	
	/** Is empty. */
	bool IsEmpty() const;
	/*@}*/
};

#endif
