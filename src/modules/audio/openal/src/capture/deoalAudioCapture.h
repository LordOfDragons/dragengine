/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEOALAUDIOCAPTURE_H_
#define _DEOALAUDIOCAPTURE_H_

#include "../deoalBasics.h"

#include <dragengine/systems/deAudioSystem.h>

class deAudioOpenAL;


/**
 * \brief OpenAL audio capture.
 */
class deoalAudioCapture{
public:
	/** \brief State. */
	enum eState{
		esStopped,
		esStarting,
		esRunning,
		esStopping
	};
	
	
private:
	deAudioOpenAL &pOal;
	eState pState;
	deAudioSystem::AudioCaptureFormat pFormat;
	deAudioSystem::AudioCaptureLevels pLevels;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create audio capture. */
	explicit deoalAudioCapture(deAudioOpenAL &oal);
	
	/** \brief Clean up audio capture. */
	~deoalAudioCapture();
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief OpenAL module. */
	inline deAudioOpenAL &GetOal() const{ return pOal; }
	
	/** \brief Capture format. */
	inline const deAudioSystem::AudioCaptureFormat &GetFormat() const{ return pFormat; }
	
	/** \brief Capture levels. */
	inline const deAudioSystem::AudioCaptureLevels &GetLevels() const{ return pLevels; }
	
	/** \brief Start capturing audio. */
	void StartCapture();
	
	/** \brief Stop capturing audio. */
	void StopCapture();
	
	/** \brief Is capturing. */
	bool IsCapturing();
	
	
	/** \brief Synchronize. */
	void Synchronize();
	/*@}*/
};

#endif
