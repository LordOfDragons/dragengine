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

#ifndef _DEOALSOUND_H_
#define _DEOALSOUND_H_

#include <dragengine/systems/modules/audio/deBaseAudioSound.h>

class deAudioOpenAL;
class deoalASound;

class deSound;



/**
 * \brief Sound resource peer.
 */
class deoalSound : public deBaseAudioSound{
private:
	deAudioOpenAL &pOal;
	deSound &pSound;
	deoalASound *pASound;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sound peer. */
	deoalSound( deAudioOpenAL &oal, deSound &sound );
	
	/** \brief Clean up sound peer. */
	virtual ~deoalSound();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief OpenAL module. */
	inline deAudioOpenAL &GetAudioThread() const{ return pOal; }
	
	/** \brief Sound resource. */
	inline deSound &GetSound() const{ return pSound; }
	
	/** \brief Audio sound. */
	inline deoalASound *GetASound() const{ return pASound; }
	
	
	
	/**
	 * \brief Prepare sound buffer if required.
	 * 
	 * Marks sound as used. Used sounds are cached if not streaming. Not used sounds are not
	 * cached and not loaded the next time the game starts. This avoids loading sounds data
	 * from the original resource file if not used. Not used sounds are often used only by
	 * synthesizer modules. This flag is stored in the cache file. Hence sounds stay marked
	 * used even if during one game run they are not used.
	 * 
	 * This method has to be called whenever a speaker or microphone starts using the sound.
	 * If the buffer is not present and the sound is not streaming the samples are loaded
	 * from the original resource file, the buffer created and the samples cached.
	 * 
	 * \warning This call is allowed from main thread only!
	 */
	void Synchronize();
	/*@}*/
};

#endif
