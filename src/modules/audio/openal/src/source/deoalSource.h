/* 
 * Drag[en]gine OpenAL Audio Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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
	deoalSource( deoalAudioThread &audioThread );
	
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
	void SetBufferCount( int count );
	
	/** Buffer. */
	ALuint GetBufferAt( int position ) const;
	
	
	
	/** Owner or \em NULL if not bound. */
	inline void *GetOwner() const{ return pOwner; }
	
	/** Set owner or \em NULL if not bound. */
	void SetOwner( void *owner );
	
	/** Importance. */
	inline float GetImportance() const{ return pImportance; }
	
	/** Set importance. */
	void SetImportance( float importance );
	
	
	
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
