/* 
 * Drag[en]gine OpenAL Audio Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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
	const int pMaxCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create openal sources manager. */
	deoalEffectSlotManager( deoalAudioThread &audioThread );
	
	/** Clean up openal sources manager. */
	~deoalEffectSlotManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** OpenAL module. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	/**
	 * Bind effect slot. Returns first unbound effect slot. If not existing a new effect slot
	 * is created and bound. If no more effect slots can be created the effect slot with the
	 * lowest importance is rebound unless the owner importance is lower. It is thus possible
	 * the caller gets no effect slot.
	 */
	deoalEffectSlot *Bind( void *owner, float importance );
	
	/** Unbind effect slot. */
	void Unbind( deoalEffectSlot *slot );
	/*@}*/
	
	
	
private:
	deoalEffectSlot *pNextUnbound() const;
	deoalEffectSlot *pBestRebindable() const;
	deoalEffectSlot *pCreateNew();
};

#endif
