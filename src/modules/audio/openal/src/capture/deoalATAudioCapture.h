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

#ifndef _DEOALATAUDIOCAPTURE_H_
#define _DEOALATAUDIOCAPTURE_H_

#include <stdint.h>

#include <dragengine/common/collection/decTList.h>
#include <dragengine/systems/deAudioSystem.h>

#include "../deoalBasics.h"

class deoalAudioThread;


/**
 * \brief OpenAL audio capture.
 *
 * Uses OpenAL Capture API (ALC_EXT_capture) to capture audio samples from a recording device.
 * A background thread polls for available samples and delivers them to the audio system.
 */
class deoalATAudioCapture{
private:
	deoalAudioThread &pAudioThread;
	
	ALenum pFormat;
	int pSampleRate, pBitRate;
	int pBufferSampleCount, pBytesPerSample, pBufferSize;
	float pSampleFactor;
	
	ALCdevice *pDevice;
	decTList<uint8_t> pBuffer;
	bool pCapturing;
	
	deAudioSystem::AudioCaptureLevels pLevels;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create audio capture. */
	deoalATAudioCapture(deoalAudioThread &audioThread, ALenum format, int sampleRate);
	
	/** \brief Clean up audio capture. */
	~deoalATAudioCapture();
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Audio thread. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	/** \brief Capture format. */
	void GetFormat(deAudioSystem::AudioCaptureFormat &format) const;
	
	/** \brief Audio levels. */
	void GetLevels(deAudioSystem::AudioCaptureLevels &levels) const;
	
	/** \brief Start capturing audio. */
	void StartCapture();
	
	/** \brief Stop capturing audio. */
	void StopCapture();
	
	/** \brief Is capturing. */
	bool IsCapturing();
	
	/** \brief Capture samples. */
	void CaptureSamples();
	/*@}*/
	
	
private:
	void pStopAndCloseDevice();
};

#endif
