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
	deoalSharedEffectSlot( deoalAudioThread &audioThread );
	
protected:
	/** Clean up openal shared effect slot. */
	virtual ~deoalSharedEffectSlot();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** OpenAL module. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	/** Effect slot. */
	const deoalEffectSlot::Ref &GetEffectSlot();
	
	/** Drop effect slot. */
	void DropEffectSlot();
	
	/** Speaker count. */
	int GetSpeakerCount() const;
	
	/** Speaker at index. */
	deoalASpeaker *GetSpeakerAt( int index ) const;
	
	/** Add speaker. */
	void AddSpeaker( deoalASpeaker *speaker );
	
	/** Remove speaker. */
	void RemoveSpeaker( deoalASpeaker *speaker );
	
	/** Remove all speakers. */
	void RemoveAllSpeakers();
	
	/** Move speaker to first position and make it reference. */
	void MoveSpeakerFront( deoalASpeaker *speaker );
	
	/** Reference speaker or nullptr. */
	inline deoalASpeaker *GetReferenceSpeaker() const{ return pRefSpeaker; }
	
	/** Update effect slot. */
	void UpdateEffectSlot();
	
	/** Is empty. */
	bool IsEmpty() const;
	/*@}*/
};

#endif
