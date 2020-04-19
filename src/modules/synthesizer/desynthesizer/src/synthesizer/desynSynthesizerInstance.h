/* 
 * Drag[en]gine Synthesizer Module
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
