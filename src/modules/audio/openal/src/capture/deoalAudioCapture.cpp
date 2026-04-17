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

#include "deoalAudioCapture.h"
#include "deoalATAudioCapture.h"
#include "../deAudioOpenAL.h"
#include "../audiothread/deoalAudioThread.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/systems/deAudioSystem.h>


// Class deoalAudioCapture
////////////////////////////

// Constructor, destructor
////////////////////////////

deoalAudioCapture::deoalAudioCapture(deAudioOpenAL &oal) :
pOal(oal),
pState(esStopped){
}

deoalAudioCapture::~deoalAudioCapture(){
	if(pState != esStopped){
		StopCapture();
	}
}


// Management
///////////////

void deoalAudioCapture::StartCapture(){
	pOal.LogInfoFormat("AudioCapture.StartCapture: state=%d", (int)pState);
	
	switch(pState){
	case esStopped:
		pState = esStarting;
		pOal.GetGameEngine()->GetAudioSystem()->NotifyAudioCaptureStateChanged();
		break;
		
	case esStarting:
	case esRunning:
		break;
		
	case esStopping:
		pState = esRunning;
		pOal.GetGameEngine()->GetAudioSystem()->NotifyAudioCaptureStateChanged();
	}
}

void deoalAudioCapture::StopCapture(){
	pOal.LogInfoFormat("AudioCapture.StopCapture: state=%d", (int)pState);
	
	switch(pState){
	case esStopped:
	case esStopping:
		break;
		
	case esStarting:
		pState = esStopped;
		pOal.GetGameEngine()->GetAudioSystem()->NotifyAudioCaptureStateChanged();
		break;
		
	case esRunning:
		pState = esStopping;
		pOal.GetGameEngine()->GetAudioSystem()->NotifyAudioCaptureStateChanged();
		break;
	}
}

bool deoalAudioCapture::IsCapturing(){
	return pState == esStarting || pState == esRunning;
}

void deoalAudioCapture::Synchronize(){
	switch(pState){
	case esStopped:
		break;
		
	case esStarting:
		try{
			auto ac = deTUniqueReference<deoalATAudioCapture>::New(
				pOal.GetAudioThread(), AL_FORMAT_MONO16, 48000);
			ac->StartCapture();
			ac->GetFormat(pFormat);
			
			pOal.GetAudioThread().GetAudioCapture() = std::move(ac);
			pState = esRunning;
			
		}catch(const deException &e){
			pOal.LogException(e);
			pOal.GetAudioThread().GetAudioCapture().Clear();
			pState = esStopped;
			pOal.GetGameEngine()->GetAudioSystem()->NotifyAudioCaptureStateChanged();
		}
		break;
		
	case esRunning:
		if(pOal.GetAudioThread().GetAudioCapture()){
			pOal.GetAudioThread().GetAudioCapture()->GetLevels(pLevels);
			
		}else{
			pLevels = {};
		}
		break;
		
	case esStopping:
		pLevels = {};
		try{
			pOal.GetAudioThread().GetAudioCapture().Clear();
			pState = esStopped;
			pOal.GetGameEngine()->GetAudioSystem()->NotifyAudioCaptureStateChanged();
			
		}catch(const deException &e){
			pOal.LogException(e);
		}
		break;
	}
}
