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

#ifndef _DEOALSYNTHESIZERINSTANCE_H_
#define _DEOALSYNTHESIZERINSTANCE_H_

#include <dragengine/resources/synthesizer/deSynthesizerInstance.h>
#include <dragengine/systems/modules/audio/deBaseAudioSynthesizerInstance.h>

class deAudioOpenAL;
class deoalASynthesizerInstance;

class deSynthesizerInstance;



/**
 * \brief Sound level meter resource peer.
 */
class deoalSynthesizerInstance : public deBaseAudioSynthesizerInstance{
private:
	deAudioOpenAL &pOal;
	deSynthesizerInstance &pInstance;
	deoalASynthesizerInstance *pAInstance;
	unsigned int pUpdateTracker;
	bool pDirtyParameters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deoalSynthesizerInstance(deAudioOpenAL &oal, deSynthesizerInstance &instance);
	
	/** \brief Clean up peer. */
	virtual ~deoalSynthesizerInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deAudioOpenAL &GetOal() const{ return pOal; }
	
	/** \brief Instance. */
	inline deSynthesizerInstance &GetInstance() const{ return pInstance; }
	
	/** \brief Audio peer. */
	inline deoalASynthesizerInstance *GetAInstance() const{ return pAInstance; }
	
	/** \brief Update tracker. */
	inline unsigned int GetUpdateTracker() const{ return pUpdateTracker; }
	
	
	
	/**
	 * \brief Synchronize.
	 * \warning Called during synchronization time from main thread.
	 */
	void Synchronize();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Synthesizer changed. */
	virtual void SynthesizerChanged();
	
	/** \brief Play time changed. */
	virtual void PlayTimeChanged();
	/*@}*/
};

#endif
