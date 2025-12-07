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

#ifndef _DEOALSOURCE_H_
#define _DEOALSOURCE_H_

#include "../deoalBasics.h"

class deoalEffectSlot;


/**
 * OpenAL source object.
 * 
 * Keeps track of an openal source object. Sources are limited resources on the hardware and
 * have to be reused as much as possible to avoid running out of openal memory. A source
 * object is either unbound if not used by an engine resource or bound if in use. The
 * importance number is used to determine what source to reassign in case not enough sources
 * are available by the hardware and is located in the range from 0 to 1 where 1 is the most
 * important. The owner of an openal source object has to check before each access to the
 * source if it is still the owner. This avoids the need for callback mechanisms. Source
 * objects are only held by the deoalSourceManager. Users only store a pointer. The owner
 * is identified using a memory pointer location. If the owner releases the source it has to
 * set the owner pointer to NULL.
 */
class deoalSource{
public:
	/** Play state. */
	enum eState {
		epsPlaying,
		epsPaused,
		epsStopped
	};
	
	
	
private:
	deoalAudioThread &pAudioThread;
	
	eState pState;
	
	ALuint pSource;
	
	ALuint *pBuffers;
	int pBufferCount;
	
	void *pOwner;
	float pImportance;
	
	ALuint pFilter;
	deoalEffectSlot *pEffectSlot;
	ALuint pEffectSlotFilter;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create openal buffer. */
	deoalSource(deoalAudioThread &audioThread);
	
	/** Clean up openal buffer. */
	~deoalSource();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** OpenAL module. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	/** Source. */
	inline ALuint GetSource() const{ return pSource; }
	
	
	
	/** Number of streaming buffers. */
	inline int GetBufferCount() const{ return pBufferCount; }
	
	/** Set number of streaming buffers. */
	void SetBufferCount(int count);
	
	/** Buffer. */
	ALuint GetBufferAt(int position) const;
	
	
	
	/** Owner or \em NULL if not bound. */
	inline void *GetOwner() const{ return pOwner; }
	
	/** Set owner or \em NULL if not bound. */
	void SetOwner(void *owner);
	
	/** Importance. */
	inline float GetImportance() const{ return pImportance; }
	
	/** Set importance. */
	void SetImportance(float importance);
	
	
	
	/** Source is bound. */
	bool IsBound() const;
	
	/** Source is not bound. */
	bool IsUnbound() const;
	
	
	
	/** Play state. */
	inline eState GetState() const{ return pState; }
	
	/** Start playing back. */
	void Play();
	
	/** Pause playing back. */
	void Pause();
	
	/** Stop playing back. */
	void Stop();
	
	
	
	/** Filter creating it if not present. */
	ALuint GetFilter();
	
	/**
	 * Assign source filter if present.
	 * 
	 * Call this whenever filter changed.
	 */
	void AssignFilter();
	
	/** Remove filter from source but keep filter object alive. */
	void ClearFilter();
	
	
	
	/** Effect slot or nullptr. */
	deoalEffectSlot *GetEffectSlot();
	
	/** Drop effect slot. */
	void DropEffectSlot();
	
	/** Effect slot filter creating it if not present. */
	ALuint GetEffectSlotFilter();
	
	
	
	/** Reset to be used again. */
	void Reset();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
