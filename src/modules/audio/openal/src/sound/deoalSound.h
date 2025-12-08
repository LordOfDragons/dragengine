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
	deoalASound::Ref pASound;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sound peer. */
	deoalSound(deAudioOpenAL &oal, deSound &sound);
	
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
