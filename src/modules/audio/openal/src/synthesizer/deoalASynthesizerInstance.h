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
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoalASynthesizerInstance> Ref;


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
