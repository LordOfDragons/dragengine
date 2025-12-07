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

#ifndef _DEOALEFFECTSLOTMANAGER_H_
#define _DEOALEFFECTSLOTMANAGER_H_

#include "../deoalBasics.h"

#include <dragengine/common/collection/decObjectList.h>

class deoalEffectSlot;


/**
 * Keeps track of openal effect slot objects. Effect slots are limited resources
 * and have to be shared.
 */
class deoalEffectSlotManager{
private:
	deoalAudioThread &pAudioThread;
	
	decObjectList pSlots;
	int pMaxCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create openal sources manager. */
	deoalEffectSlotManager(deoalAudioThread &audioThread);
	
	/** Clean up openal sources manager. */
	~deoalEffectSlotManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** OpenAL module. */
	inline deoalAudioThread &GetAudioThread() const{return pAudioThread;}
	
	/**
	 * Bind effect slot. Returns first unbound effect slot. If not existing a new effect slot
	 * is created and bound. If no more effect slots can be created the effect slot with the
	 * lowest importance is rebound unless the owner importance is lower. It is thus possible
	 * the caller gets no effect slot.
	 */
	deoalEffectSlot *Bind(void *owner, float importance);
	
	/** Unbind effect slot. */
	void Unbind(deoalEffectSlot *slot);
	
	/** Update. */
	void Update(float elapsed);
	/*@}*/
	
	
	
private:
	deoalEffectSlot *pBestUnbound() const;
	deoalEffectSlot *pBestKeptAlive() const;
	deoalEffectSlot *pBestRebindable() const;
	deoalEffectSlot *pCreateNew();
};

#endif
