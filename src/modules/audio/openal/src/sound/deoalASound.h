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
