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

#ifndef _DEOALASOUND_H_
#define _DEOALASOUND_H_

#include "../deoalBasics.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class deSound;
class deoalAudioThread;



/**
 * \brief audio sound.
 */
class deoalASound : public deObject{
private:
	deoalAudioThread &pAudioThread;
	decString pFilename;
	
	const int pBytesPerSample;
	const int pSampleCount;
	const int pChannelCount;
	const int pSampleRate;
	const float pPlayTime;
	
	ALuint pBuffer;
	ALenum pFormat;
	bool pValid;
	
	char *pStreamData;
	int pStreamDataSize;
	bool pStreaming;
	
	bool pIsUsed;
	bool pIsCached;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sound peer. */
	deoalASound( deoalAudioThread &audioThread, deSound &sound );
	
protected:
	/** \brief Clean up sound peer. */
	virtual ~deoalASound();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief OpenAL module. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	/** \brief Filename of resource. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	
	
	/** \brief Bytes per sample. */
	inline int GetBytesPerSample() const{ return pBytesPerSample; }
	
	/** \brief Sample count. */
	inline int GetSampleCount() const{ return pSampleCount; }
	
	/** \brief Channel count. */
	inline int GetChannelCount() const{ return pChannelCount; }
	
	/** \brief Sample rate. */
	inline int GetSampleRate() const{ return pSampleRate; }
	
	/** \brief Play time. */
	inline float GetPlayTime() const{ return pPlayTime; }
	
	
	
	/** \brief Buffer or 0 if streaming. */
	inline ALuint GetBuffer() const{ return pBuffer; }
	
	/** \brief Buffer format. */
	inline ALenum GetFormat() const{ return pFormat; }
	
	
	
	/** \brief Stream data or \em NULL if not loaded. */
	inline char *GetStreamData() const{ return pStreamData; }
	
	/** \brief Stream data size in bytes or 0 if not loaded. */
	inline int GetStreamDataSize() const{ return pStreamDataSize; }
	
	/** \brief Sound is too large requiring streaming. */
	inline bool GetStreaming() const{ return pStreaming; }
	
	/** \bnrief Sound has been at least once. */
	inline bool IsUsed() const{ return pIsUsed; }
	
	
	
	/**
	 * \brief Prepare sound buffer data if required.
	 * 
	 * Marks sound as used. Used sounds are cached if not streaming. Not used sounds are not
	 * cached and not loaded the next time the game starts. This avoids loading sounds data
	 * from the original resource file if not used. Not used sounds are often used only by
	 * synthesizer modules. This flag is stored in the cache file. Hence sounds stay marked
	 * used even if during one game run they are not used.
	 * 
	 * \warning Called during synchronization time from main thread.
	 */
	void PreloadSound( deSound &sound );
	
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
	 */
	void PrepareBuffers();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pLoadFromCache();
	void pWriteToCache();
	
	void pDetermineStreaming();
	void pDetermineFormat();
	
	void pLoadEntireSound( deSound &sound );
};

#endif
