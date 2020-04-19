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

#ifndef _DEOALASYNTHESIZERINSTANCE_H_
#define _DEOALASYNTHESIZERINSTANCE_H_

#include <dragengine/deObject.h>
#include <dragengine/threading/deMutex.h>

class deoalAudioThread;
class deSynthesizerInstance;


/**
 * \brief Audio synthesizer instance.
 */
class deoalASynthesizerInstance : public deObject{
private:
	deoalAudioThread &pAudioThread;
	
	int pBytesPerSample;
	int pChannelCount;
	int pSampleRate;
	int pSampleCount;
	
	unsigned int pUpdateTracker;
	
	deSynthesizerInstance *pInstance;
	deMutex pMutex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create microphone. */
	deoalASynthesizerInstance( deoalAudioThread &audioThread, deSynthesizerInstance &instance );
	
protected:
	/** \brief Clean up microphone. */
	virtual ~deoalASynthesizerInstance();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	/**
	 * \brief Update parameters.
	 * \warning Called by main thread during synchronization.
	 */
	void UpdateParameters();
	
	/**
	 * \brief Drop instance.
	 * \warning Called by main thread during synchronization.
	 */
	void DropInstance();
	
	/** \brief Bytes per sample. */
	inline int GetBytesPerSample() const{ return pBytesPerSample; }
	
	/** \brief Channel count. */
	inline int GetChannelCount() const{ return pChannelCount; }
	
	/** \brief Sample rate. */
	inline int GetSampleRate() const{ return pSampleRate; }
	
	/** \brief Sample count. */
	inline int GetSampleCount() const{ return pSampleCount; }
	
	/** \brief Update tracker. */
	inline unsigned int GetUpdateTracker() const{ return pUpdateTracker; }
	
	/** \brief Reset synthesizer playback. */
	void Reset();
	
	/**
	 * \brief Generate sound.
	 * \param[out] buffer Buffer to store samples in.
	 * \param[in] bufferSize Size of buffer in bytes to store samples in. Has to match format.
	 * \param[in] offset Offset in samples to produce sound at.
	 * \param[in] samples Number of samples to produce.
	 * 
	 * \throws EInvalidParam \em bufferSize does not match format.
	 * \throws EInvalidParam \em buffer is NULL.
	 */
	void GenerateSound( void *buffer, int bufferSize, int offset, int samples );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pAdvanceUpdateTracker();
};

#endif
