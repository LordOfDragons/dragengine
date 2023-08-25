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

#ifndef _DEOALEFFECTSLOT_H_
#define _DEOALEFFECTSLOT_H_

#include "../deoalBasics.h"

#include <dragengine/deObject.h>


/**
 * OpenAL effect slot object.
 * 
 * Keeps track of an openal effect slot object. Effect slots are limited resources and
 * have to be shared.
 */
class deoalEffectSlot : public deObject{
public:
	typedef deTObjectReference<deoalEffectSlot> Ref;
	
	
	
private:
	deoalAudioThread &pAudioThread;
	
	ALuint pSlot;
	ALuint pEffect;
	
	void *pOwner;
	float pImportance;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create openal effect slot. */
	deoalEffectSlot( deoalAudioThread &audioThread );
	
protected:
	/** Clean up openal effect slot. */
	virtual ~deoalEffectSlot();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** OpenAL module. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	/** Slot. */
	inline ALuint GetSlot() const{ return pSlot; }
	
	/** Effect. */
	inline ALuint GetEffect() const{ return pEffect; }
	
	
	
	/** Owner or nullptr if not bound. */
	inline void *GetOwner() const{ return pOwner; }
	
	/** Assign owner and clear effect. */
	void AssignOwner( void *owner, float importance );
	
	/** Clear owner and clear effect. */
	void ClearOwner();
	
	/** Importance. */
	inline float GetImportance() const{ return pImportance; }
	
	/** Set importance. */
	void SetImportance( float importance );
	
	/** Clear effect. */
	void ClearEffect();
	
	/** Update effect slot with effect parameters. */
	void UpdateSlot() const;
	
	
	
	/** Bound. */
	inline bool IsBound() const{ return pOwner != nullptr; }
	
	/** Not bound. */
	inline bool IsUnbound() const{ return pOwner == nullptr; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
