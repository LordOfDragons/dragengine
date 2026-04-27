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

#include "deoalATAudioCapture.h"
#include "../deAudioOpenAL.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"
#include "../configuration/deoalConfiguration.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/deAudioSystem.h>
#include <dragengine/threading/deMutexGuard.h>


// Class deoalATAudioCapture
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoalATAudioCapture::deoalATAudioCapture(deoalAudioThread &audioThread, ALenum format, int sampleRate) :
pAudioThread(audioThread),
pFormat(format),
pSampleRate(sampleRate),
pBufferSampleCount(4096),
pDevice(nullptr),
pCapturing(false)
{
	switch(format){
	case AL_FORMAT_MONO8:
		pBitRate = 8;
		pSampleFactor = 1.0f / 128.0f;
		break;
		
	case AL_FORMAT_MONO16:
		pBitRate = 16;
		pSampleFactor = 1.0f / 32768.0f;
		break;
		
	default:
		DETHROW_INFO(deeInvalidParam, "Unsupported capture format");
	}
	
	pBytesPerSample = pBitRate / 8;
	pBufferSize = pBufferSampleCount * pBytesPerSample;
	pBuffer.SetCountDiscard(pBufferSize);
	
	pAudioThread.GetLogger().LogInfoFormat(
		"ATAudioCapture.Create: format=0x%x sampleRate=%d bitRate=%d",
		(int)format, sampleRate, pBitRate);
}

deoalATAudioCapture::~deoalATAudioCapture(){
	pAudioThread.GetLogger().LogInfo("ATAudioCapture.Destroy");
	pStopAndCloseDevice();
}


// Management
///////////////

void deoalATAudioCapture::GetFormat(deAudioSystem::AudioCaptureFormat &format) const{
	format.sampleRate = pSampleRate;
	format.bitRate = pBitRate;
}

void deoalATAudioCapture::GetLevels(deAudioSystem::AudioCaptureLevels &levels) const{
	levels = pLevels;
}

void deoalATAudioCapture::StartCapture(){
	pAudioThread.GetLogger().LogInfo("ATAudioCapture.StartCapture");
	pStopAndCloseDevice();
	
	const ALCchar * const deviceName = nullptr;
	
	pDevice = alcCaptureOpenDevice(deviceName, (ALCuint)pSampleRate, pFormat, pBufferSampleCount * 2);
	if(!pDevice){
		DETHROW_INFO(deeInvalidAction, "Failed to open capture device");
	}
	pAudioThread.GetLogger().LogInfoFormat("ATAudioCapture.StartCapture: device opened: %s",
		alcGetString(pDevice, ALC_CAPTURE_DEVICE_SPECIFIER));
	
	alcCaptureStart(pDevice);
	pCapturing = true;
	pAudioThread.GetLogger().LogInfo("ATAudioCapture.StartCapture: capture started");
}

void deoalATAudioCapture::StopCapture(){
	pAudioThread.GetLogger().LogInfo("ATAudioCapture.StopCapture");
	pStopAndCloseDevice();
	pAudioThread.GetLogger().LogInfo("ATAudioCapture.StopCapture: capture stopped");
}

bool deoalATAudioCapture::IsCapturing(){
	return pCapturing;
}

void deoalATAudioCapture::CaptureSamples(){
	if(!pDevice){
		return;
	}
	
	ALCint available = 0;
	alcGetIntegerv(pDevice, ALC_CAPTURE_SAMPLES, 1, &available);
	if(available == 0){
		return;
	}
	
	available = decMath::min(available, pBufferSampleCount);
	uint8_t * const buffer = pBuffer.GetArrayPointer();
	alcCaptureSamples(pDevice, buffer, available);
	
	const deoalConfiguration &config = pAudioThread.GetConfiguration();
	const float volume = config.GetAudioCaptureVolume();
	const float noiseGate = config.GetAudioCaptureNoiseGate();
	
	float levelPeak = 0.0f;
	double levelRMS = 0.0;
	
	switch(pBitRate){
	case 8:{
		auto f = [&](int8_t &sample){
			const float s = (float)sample * pSampleFactor * volume;
			const float sabs = fabsf(s);
			levelPeak = decMath::max(levelPeak, sabs);
			levelRMS += (double)sabs * (double)sabs;
			const float sscaled = sabs > noiseGate ? s : 0.0f;
			sample = (int8_t)decMath::clamp((int)sscaled, -128, 127);
		};
		
		int8_t * const samples = reinterpret_cast<int8_t*>(buffer);
		for(int i=0; i<available; i++){
			f(samples[i]);
		}
		}break;
		
	case 16:{
		auto f = [&](int16_t &sample){
			const float s = (float)sample * pSampleFactor * volume;
			const float sabs = fabsf(s);
			levelPeak = decMath::max(levelPeak, sabs);
			levelRMS += (double)sabs * (double)sabs;
			const float sscaled = sabs > noiseGate ? s : 0.0f;
			sample = (int16_t)decMath::clamp((int)(sscaled * 32768.0f), -32768, 32767);
		};
		
		int16_t * const samples = reinterpret_cast<int16_t*>(buffer);
		for(int i=0; i<available; i++){
			f(samples[i]);
		}
		}break;
	}
	
	pLevels.peak = levelPeak;
	pLevels.rms = (float)sqrt(levelRMS / (double)available);
	
	pAudioThread.GetOal().GetGameEngine()->GetAudioSystem()->
		NotifyAudioCaptureSamplesCaptured(buffer, available);
}


// Private
////////////

void deoalATAudioCapture::pStopAndCloseDevice(){
	pCapturing = false;
	
	if(pDevice){
		alcCaptureStop(pDevice);
		alcCaptureCloseDevice(pDevice);
		pDevice = nullptr;
	}
}
