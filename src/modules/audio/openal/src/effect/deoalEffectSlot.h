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
	
	ALenum pEffectType;
	bool pParametersChanged;
	
	float pKeepAliveElapsed;
	float pKeepAliveTimeout;
	
	
	
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
	
	
	
	/** Set effect type. */
	void SetEffectType( ALenum type );
	
	
	
	/** Update effect slot with effect parameters if changed and reset keep-alive. */
	void UpdateSlot( float timeout );
	
	
	
	/** Update. */
	void Update( float elapsed );
	
	
	
	/** Bound. */
	inline bool IsBound() const{ return pOwner != nullptr; }
	
	/** Not bound. */
	inline bool IsUnbound() const{ return pOwner == nullptr; }
	
	/** Is kept alive. */
	inline bool IsKeptAlive() const{ return IsUnbound() && pEffectType != AL_EFFECT_NULL; }
	
	/** Elapsed keep-alive time. */
	inline float GetElapsedKeepAliveTime() const{ return pKeepAliveElapsed; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pUpdateSlotParameters();
};

#endif
