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

#ifndef _DESYNSYNTHESIZERINSTANCE_H_
#define _DESYNSYNTHESIZERINSTANCE_H_

#include "../desynBasics.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decPointerSet.h>
#include <dragengine/systems/modules/synthesizer/deBaseSynthesizerSynthesizerInstance.h>
#include <dragengine/threading/deMutex.h>

class desynSynthesizer;
class desynSynthesizerController;
class desynSharedBuffer;
class deDESynthesizer;

class deSynthesizerInstance;



/**
 * \brief SynthesizerInstance peer.
 */
class desynSynthesizerInstance : public deBaseSynthesizerSynthesizerInstance{
private:
	deDESynthesizer &pModule;
	deSynthesizerInstance &pSynthesizerInstance;
	
	desynSynthesizer *pSynthesizer;
	unsigned int pSynthesizerUpdateTracker;
	
	desynSynthesizerController *pControllers;
	int pControllerCount;
	
	int pChannelCount;
	int pSampleRate;
	int pBytesPerSample;
	int pSampleCount;
	bool pSilent;
	float pInverseSampleRate;
	int pGenerateSampleSize;
	
	int pBufferSampleCount;
	int pBufferCount;
	
	bool pDirtySynthesizer;
	bool pDirtyControllers;
	bool pDirtyFormat;
	
	char *pStateData;
	int pStateDataSize;
	deMutex pMutex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer instance peer. */
	desynSynthesizerInstance( deDESynthesizer &module, deSynthesizerInstance &instance );
	
	/** \brief Clean up synthesizer instance peer. */
	virtual ~desynSynthesizerInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief OpenAL module. */
	inline deDESynthesizer &GetModule() const{ return pModule; }
	
	/** \brief SynthesizerInstance engine resource. */
	inline deSynthesizerInstance &GetSynthesizerInstance() const{ return pSynthesizerInstance; }
	
	/** \brief Synthesizer or \em NULL if not set. */
	inline desynSynthesizer *GetSynthesizer() const{ return pSynthesizer; }
	
	
	
	/** \brief Controller at index. */
	desynSynthesizerController &GetControllerAt( int index ) const;
	
	
	
	/** \brief Channel count. */
	inline int GetChannelCount() const{ return pChannelCount; }
	
	/** \brief Sample rate. */
	inline int GetSampleRate() const{ return pSampleRate; }
	
	/** \brief Bytes per sample. */
	inline int GetBytesPerSample() const{ return pBytesPerSample; }
	
	/** \brief Number of samples to create. */
	inline int GetSampleCount() const{ return pSampleCount; }
	
	/** \brief Synthesizer is silent. */
	inline bool GetSilent() const{ return pSilent; }
	
	/** \brief Inverse sample rate. */
	inline float GetInverseSampleRate() const{ return pInverseSampleRate; }
	
	/** \brief Number of samples per streaming buffer. */
	inline int GetBufferSampleCount() const{ return pBufferSampleCount; }
	
	/** \brief Number of streaming buffers to use. */
	inline int GetBufferCount() const{ return pBufferCount; }
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Synthesizer changed. */
	virtual void SynthesizerChanged();
	
	/** \brief Controller changed. */
	virtual void ControllerChanged( int index );
	
	/** \brief Play time changed. */
	virtual void PlayTimeChanged();
	/*@}*/
	
	
	
	/** \name Generate sound */
	/*@{*/
	/**
	 * \brief Reset synthesizer playback.
	 * 
	 * \warning Audio module is allowed to call this asynchronously.
	 */
	virtual void Reset();
	
	/**
	 * \brief Generate sound.
	 * 
	 * \warning Audio module is allowed to call this asynchronously.
	 * 
	 * \param[out] buffer Buffer to store samples in.
	 * \param[in] bufferSize Size of buffer in bytes to store samples in. Has to match format.
	 * \param[in] offset Offset in samples to produce sound at.
	 * \param[in] samples Number of samples to produce.
	 * 
	 * \throws EInvalidParam \em bufferSize does not match format.
	 * \throws EInvalidParam \em buffer is NULL.
	 * \throws EInvalidParam Assigned synthesizer object changed while in use.
	 */
	virtual void GenerateSound( void *buffer, int bufferSize, int offset, int samples );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pPrepare();
	void pUpdateFormat();
	
	void pClearControllers();
	void pCreateControllers();
	
	void pGenerateSilence( void *buffer, int samples );
	void pGenerateSound( desynSharedBuffer *sharedBuffer, void *buffer, int samples );
	
	void pUpdateControllerValues( int samples, int offset );
	
	void pCreateStateData();
	void pFreeStateData();
};

#endif
