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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoalDebugInfo.h"
#include "deoalAudioThread.h"
#include "deoalATDebug.h"
#include "../deAudioOpenAL.h"
#include "../devmode/deoalDevMode.h"
#include "../environment/deoalEnvironment.h"
#include "../environment/raytrace/parallel/deoalRTParallelEnvProbe.h"
#include "../microphone/deoalMicrophone.h"
#include "../microphone/deoalAMicrophone.h"
#include "../speaker/deoalASpeaker.h"
#include "../world/deoalWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/debug/deDebugBlockInfo.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/world/deWorld.h>



// Class deoalDebugInfo
////////////////////////

// Constructor, destructor
////////////////////////////

deoalDebugInfo::deoalDebugInfo(deoalAudioThread &audioThread) :
pAudioThread(audioThread),

pDIModule(NULL),
pDebugTimeMainThread(29, 2),
pDebugTimeMainThreadWaitFinish(29, 2),
pDebugTimeMainThreadSynchronize(29, 2),
pDebugTimeAudioThread(29, 2),
pDebugTimeAudioThreadPrepare(29, 2),
pDebugTimeAudioThreadProcess(29, 2),
pDebugTimeAudioThreadSpeakersUpdate(29, 2),
pDebugTimeAudioThreadWorldProcess(29, 2),
pDebugTimeAudioThreadSpeakersProcess(29, 2),
pDebugTimeAudioThreadEffectsProcess(29, 2),
pDebugTimeFrameLimiterMain(0.0f),
pDebugTimeFrameLimiterAudio(0.0f),
pDebugTimeFrameLimiterAudioEstimated(0.0f),
pDebugFPSMain(0),
pDebugFPSAudio(0),
pDebugFPSAudioEstimated(0),

pDIActiveMic(NULL),
pDISpeakerAtPosition(NULL),

pModeVisAudSpeakers(0){
}

deoalDebugInfo::~deoalDebugInfo(){
	PrepareDIClosestSpeakers(0);
	PrepareDDClosestSpeakersDirect(0);
	HideDISpeakerAtPosition();
	HideDIActiveMic();
	HideDDActiveMicRays();
	HideDIModule();
}



// Management
///////////////

void deoalDebugInfo::ResetTimersMainThread(){
	if(! pDIModule){
		return;
	}
	
	pDebugTimerMainThread1.Reset();
	pDebugTimerMainThread2.Reset();
}

void deoalDebugInfo::StoreTimeThreadMain(float time){
	if(! pDIModule){
		return;
	}
	
	pDebugTimeMainThread.Add(time);
}

void deoalDebugInfo::StoreTimeThreadMainWaitFinish(){
	if(! pDIModule){
		return;
	}
	
	pDebugTimeMainThreadWaitFinish.Add(pDebugTimerMainThread2.GetElapsedTime());
}

void deoalDebugInfo::StoreTimeThreadMainSynchronize(){
	if(! pDIModule){
		return;
	}
	
	pDebugTimeMainThreadSynchronize.Add(pDebugTimerMainThread2.GetElapsedTime());
	pDebugTimeMainThread.Add(pDebugTimerMainThread1.GetElapsedTime());
}

void deoalDebugInfo::ResetTimersAudioThread(){
	if(! pDIModule){
		return;
	}
	
	pDebugTimerAudioThread1.Reset();
	pDebugTimerAudioThread2.Reset();
	pDebugTimerAudioThread3.Reset();
}

void deoalDebugInfo::StoreTimeAudioThread(){
	if(! pDIModule){
		return;
	}
	
	pDebugTimeAudioThread.Add(pDebugTimerAudioThread1.GetElapsedTime());
}

void deoalDebugInfo::StoreTimeAudioThreadPrepare(){
	if(!pDIModule){
		return;
	}
	
	pDebugTimeAudioThreadPrepare.Add(pDebugTimerAudioThread2.GetElapsedTime());
}

void deoalDebugInfo::StoreTimeAudioThreadProcess(){
	if(!pDIModule){
		return;
	}
	
	pDebugTimeAudioThreadProcess.Add(pDebugTimerAudioThread2.GetElapsedTime());
}

void deoalDebugInfo::StoreTimeAudioThreadSpeakersUpdate(){
	if(!pDIModule){
		return;
	}
	
	pDebugTimeAudioThreadSpeakersUpdate.Add(pDebugTimerAudioThread3.GetElapsedTime());
}

void deoalDebugInfo::StoreTimeAudioThreadWorldProcess(){
	if(!pDIModule){
		return;
	}
	
	pDebugTimeAudioThreadWorldProcess.Add(pDebugTimerAudioThread3.GetElapsedTime());
}

void deoalDebugInfo::StoreTimeAudioThreadSpeakersProcess(){
	if(!pDIModule){
		return;
	}
	
	pDebugTimeAudioThreadSpeakersProcess.Add(pDebugTimerAudioThread3.GetElapsedTime());
}

void deoalDebugInfo::StoreTimeAudioThreadEffectsProcess(){
	if(!pDIModule){
		return;
	}
	
	pDebugTimeAudioThreadEffectsProcess.Add(pDebugTimerAudioThread3.GetElapsedTime());
}

void deoalDebugInfo::StoreTimeFrameLimiter(const decTimeHistory &main,
const decTimeHistory &audio, const decTimeHistory &audioEstimated){
	if(! pDIModule){
		return;
	}
	
	if(main.HasMetrics()){
		pDebugTimeFrameLimiterMain = main.GetAverage();
		pDebugFPSMain = (int)((1.0f / pDebugTimeFrameLimiterMain) + 0.5f);
	}
	if(audio.HasMetrics()){
		pDebugTimeFrameLimiterAudio = audio.GetAverage();
		pDebugFPSAudio = (int)((1.0f / pDebugTimeFrameLimiterAudio) + 0.5f);
	}
	if(audioEstimated.HasMetrics()){
		pDebugTimeFrameLimiterAudioEstimated = audioEstimated.GetAverage();
		pDebugFPSAudioEstimated = (int)((1.0f / pDebugTimeFrameLimiterAudioEstimated) + 0.5f);
	}
}



void deoalDebugInfo::UpdateDebugInfo(){
	if(! pAudioThread.GetDebug().GetEnabled()){
		return;
	}
	
	UpdateDIModule();
	UpdateDIActiveMic();
	CaptureActiveMicRays();
	UpdateDISpeakerAtPosition();
	UpdateDIClosestSpeakers();
	CaptureDDClosestSpeakersDirect();
	UpdateVisAudSpeakers();
}



enum eDebugInfoModule{
	edimMainThread,
	edimMainThreadWaitFinish,
	edimMainThreadSynchronize,
	edimAudioThread,
	edimAudioThreadAudioPrepare,
	edimAudioThreadAudioProcess,
	edimAudioThreadAudioProcessSpeakersUpdate,
	edimAudioThreadAudioProcessWorldProcess,
	edimAudioThreadAudioProcessSpeakersProcess,
	edimAudioThreadAudioProcessEffects,
	edimAudioThreadTraceSound,
	edimAudioThreadListen,
	edimFrameLimiter,
	edimFrameLimiterEstimate,
	edimFrameLimiterFPS
};

void deoalDebugInfo::ShowDIModule(){
	if(pDIModule){
		return;
	}
	
	pDIModule = new deDebugBlockInfo(*pAudioThread.GetOal().GetGameEngine());
	pDIModule->SetTitle("Module");
	pDIModule->AddEntry("Main Thread", "");
	pDIModule->AddEntry("- Wait Finish", "");
	pDIModule->AddEntry("- Synchronize", "");
	pDIModule->AddEntry("Audio Thread", "");
	pDIModule->AddEntry("- Prepare", "");
	pDIModule->AddEntry("- Process Audio", "");
	pDIModule->AddEntry("  - Update Speakers", "");
	pDIModule->AddEntry("  - World Process", "");
	pDIModule->AddEntry("  - Process Speakers", "");
	pDIModule->AddEntry("  - Effects", "");
	pDIModule->AddEntry("- Trace Sound", "");
	pDIModule->AddEntry("- Listen", "");
	pDIModule->AddEntry("Frame Limiter", "");
	pDIModule->AddEntry("- Estimate", "");
	pDIModule->AddEntry("- FPS", "");
	pDIModule->UpdateView();
	pDIModule->AddToOverlay();
}

void deoalDebugInfo::HideDIModule(){
	if(! pDIModule){
		return;
	}
	
	delete pDIModule;
	pDIModule = NULL;
}

void deoalDebugInfo::UpdateDIModule(){
	if(! pDIModule){
		return;
	}
	
	decString text;
	text.Format("%.2f", pDebugTimeMainThread.GetAverage() * 1000.0f);
	pDIModule->SetEntryText(edimMainThread, text);
	
	text.Format("%.2f", pDebugTimeMainThreadWaitFinish.GetAverage() * 1000.0f);
	pDIModule->SetEntryText(edimMainThreadWaitFinish, text);
	
	text.Format("%.2f", pDebugTimeMainThreadSynchronize.GetAverage() * 1000.0f);
	pDIModule->SetEntryText(edimMainThreadSynchronize, text);
	
	text.Format("%.2f", pDebugTimeAudioThread.GetAverage() * 1000.0f);
	pDIModule->SetEntryText(edimAudioThread, text);
	
	text.Format("%.2f", pDebugTimeAudioThreadPrepare.GetAverage() * 1000.0f);
	pDIModule->SetEntryText(edimAudioThreadAudioPrepare, text);
	
	text.Format("%.2f", pDebugTimeAudioThreadProcess.GetAverage() * 1000.0f);
	pDIModule->SetEntryText(edimAudioThreadAudioProcess, text);
	
	text.Format("%.2f", pDebugTimeAudioThreadSpeakersUpdate.GetAverage() * 1000.0f);
	pDIModule->SetEntryText(edimAudioThreadAudioProcessSpeakersUpdate, text);
	
	text.Format("%.2f", pDebugTimeAudioThreadWorldProcess.GetAverage() * 1000.0f);
	pDIModule->SetEntryText(edimAudioThreadAudioProcessWorldProcess, text);
	
	text.Format("%.2f", pDebugTimeAudioThreadSpeakersProcess.GetAverage() * 1000.0f);
	pDIModule->SetEntryText(edimAudioThreadAudioProcessSpeakersProcess, text);
	
	text.Format("%.2f", pDebugTimeAudioThreadEffectsProcess.GetAverage() * 1000.0f);
	pDIModule->SetEntryText(edimAudioThreadAudioProcessEffects, text);
	
	const deoalRTParallelEnvProbe &rtpenv = pAudioThread.GetRTParallelEnvProbe();
	text.Format("%.1f %.1f %.1f", rtpenv.GetTimeHistoryTraceSoundRays().GetAverage() * 1000.0f,
		rtpenv.GetTimeHistoryTraceSoundRays().GetMinimum() * 1000.0f,
		rtpenv.GetTimeHistoryTraceSoundRays().GetMaximum() * 1000.0f);
	pDIModule->SetEntryText(edimAudioThreadTraceSound, text);
	
	text.Format("%.1f %.1f %.1f", rtpenv.GetTimeHistoryListen().GetAverage() * 1000.0f,
		rtpenv.GetTimeHistoryListen().GetMinimum() * 1000.0f,
		rtpenv.GetTimeHistoryListen().GetMaximum() * 1000.0f);
	pDIModule->SetEntryText(edimAudioThreadListen, text);
	
	text.Format("%.1f %.1f %.1f", pDebugTimeFrameLimiterMain * 1000.0f,
		pDebugTimeFrameLimiterAudio * 1000.0f, pDebugTimeFrameLimiterAudioEstimated * 1000.0f);
	pDIModule->SetEntryText(edimFrameLimiterEstimate, text);
	
	text.Format("%d %d %d", decMath::min(pDebugFPSMain, 999),
		decMath::min(pDebugFPSAudio, 999), decMath::min(pDebugFPSAudioEstimated, 999));
	pDIModule->SetEntryText(edimFrameLimiterFPS, text);
	
	pDIModule->UpdateView();
}



void deoalDebugInfo::ShowDIActiveMic(){
	if(pDIActiveMic){
		return;
	}
	
	pDIActiveMic = new deDebugBlockInfo(*pAudioThread.GetOal().GetGameEngine());
	pDIActiveMic->SetTitle("Microphone");
	pDIActiveMic->UpdateView();
	pDIActiveMic->AddToOverlay();
}

void deoalDebugInfo::HideDIActiveMic(){
	if(! pDIActiveMic){
		return;
	}
	
	delete pDIActiveMic;
	pDIActiveMic = NULL;
}

void deoalDebugInfo::UpdateDIActiveMic(){
	if(! pDIActiveMic){
		return;
	}
	
	deoalAMicrophone * const microphone = pAudioThread.GetActiveMicrophone();
	if(microphone){
		microphone->DebugUpdateInfo(*pDIActiveMic);
		
	}else{
		const int count = pDIActiveMic->GetEntryCount();
		if(count > 0){
			int i;
			for(i=0; i<count; i++){
				pDIActiveMic->SetEntryText(i, "-");
				pDIActiveMic->SetEntryColor(i, decColor(1.0f, 1.0f, 1.0f));
			}
			pDIActiveMic->UpdateView();
		}
	}
}



void deoalDebugInfo::ShowDDActiveMicRays(){
	if(pDDActiveMicRays){
		return;
	}
	
	pDDActiveMicRays.TakeOver(pAudioThread.GetOal().GetGameEngine()
		->GetDebugDrawerManager()->CreateDebugDrawer());
	pDDActiveMicRays->SetXRay(false);
	pDDActiveMicRays->SetVisible(false);
}

void deoalDebugInfo::HideDDActiveMicRays(){
	if(! pDDActiveMicRays){
		return;
	}
	
	if(pDDActiveMicRays->GetParentWorld()){
		pDDActiveMicRays->GetParentWorld()->RemoveDebugDrawer(pDDActiveMicRays);
	}
	pDDActiveMicRays = NULL;
}

void deoalDebugInfo::CaptureActiveMicRays(){
	if(! pDDActiveMicRays || ! pAudioThread.GetOal().GetDevMode()->GetCaptureMicRays()){
		return;
	}
	
	pAudioThread.GetOal().GetDevMode()->ClearCaptureMicRays();
	
	deoalMicrophone * const microphone = pAudioThread.GetOal().GetActiveMicrophone();
	if(microphone){
		deWorld * const world = microphone->GetParentWorld()
			? &microphone->GetParentWorld()->GetWorld() : NULL;
		
		if(pDDActiveMicRays->GetParentWorld() != world){
			if(pDDActiveMicRays->GetParentWorld()){
				pDDActiveMicRays->GetParentWorld()->RemoveDebugDrawer(pDDActiveMicRays);
			}
			if(world){
				world->AddDebugDrawer(pDDActiveMicRays);
			}
		}
		microphone->GetAMicrophone()->DebugCaptureRays(pDDActiveMicRays,
			pAudioThread.GetOal().GetDevMode()->GetCaptureMicRaysXRay(),
			pAudioThread.GetOal().GetDevMode()->GetCaptureMicRaysVolume() && false);
		
	}else{
		if(pDDActiveMicRays->GetParentWorld()){
			pDDActiveMicRays->GetParentWorld()->RemoveDebugDrawer(pDDActiveMicRays);
		}
		
		if(pDDActiveMicRays->GetShapeCount() > 0){
			pDDActiveMicRays->RemoveAllShapes();
			pDDActiveMicRays->NotifyShapeGeometryChanged();
		}
	}
}



void deoalDebugInfo::ShowDISpeakerAtPosition(){
	if(pDISpeakerAtPosition){
		return;
	}
	
	pDISpeakerAtPosition = new deDebugBlockInfo(*pAudioThread.GetOal().GetGameEngine());
	pDISpeakerAtPosition->SetTitle("Speaker At");
	pDISpeakerAtPosition->UpdateView();
	pDISpeakerAtPosition->AddToOverlay();
}

void deoalDebugInfo::HideDISpeakerAtPosition(){
	if(! pDISpeakerAtPosition){
		return;
	}
	
	delete pDISpeakerAtPosition;
	pDISpeakerAtPosition = NULL;
}

void deoalDebugInfo::UpdateDISpeakerAtPosition(){
	if(! pDISpeakerAtPosition){
		return;
	}
	
	deoalAMicrophone * const microphone = pAudioThread.GetActiveMicrophone();
	deoalASpeaker *speaker = NULL;
	
	if(microphone){
		const deoalSpeakerList &speakers = microphone->GetActiveSpeakers();
		const decDVector &pos = pAudioThread.GetDebug().GetShowSpeakerEnvInfoAtPosition();
		int i;
		for(i=0; i<speakers.GetCount(); i++){
			deoalASpeaker * const checkSpeaker = speakers.GetAt(i);
			if(checkSpeaker->GetPosition().IsEqualTo(pos, 0.01)){
				speaker = checkSpeaker;
				break;
			}
		}
	}
	
	if(speaker && speaker->GetEnvironment()){
		speaker->GetEnvironment()->DebugUpdateInfo(*pDISpeakerAtPosition);
		
	}else{
		const int count = pDISpeakerAtPosition->GetEntryCount();
		if(count > 0){
			int i;
			for(i=0; i<count; i++){
				pDISpeakerAtPosition->SetEntryText(i, "-");
				pDISpeakerAtPosition->SetEntryColor(i, decColor(1.0f, 1.0f, 1.0f));
			}
			pDISpeakerAtPosition->UpdateView();
		}
	}
}



void deoalDebugInfo::PrepareDIClosestSpeakers(int count){
	while(pDIClosestSpeakers.GetCount() < count){
		deDebugBlockInfo * const dbi = new deDebugBlockInfo(*pAudioThread.GetOal().GetGameEngine());
		decString title;
		title.Format("Speaker #%d", pDIClosestSpeakers.GetCount());
		dbi->SetTitle(title);
		dbi->AddToOverlay();
		pDIClosestSpeakers.Add(dbi);
	}
	
	while(pDIClosestSpeakers.GetCount() > count){
		delete (deDebugBlockInfo*)pDIClosestSpeakers.GetAt(pDIClosestSpeakers.GetCount() - 1);
		pDIClosestSpeakers.RemoveFrom(pDIClosestSpeakers.GetCount() - 1);
	}
}

void deoalDebugInfo::UpdateDIClosestSpeakers(){
	if(pDIClosestSpeakers.GetCount() == 0){
		return;
	}
	
	const deoalAMicrophone * const microphone = pAudioThread.GetActiveMicrophone();
	deoalSpeakerList speakers;
	
	if(microphone){
		int closestCount = pDIClosestSpeakers.GetCount();
		const deoalSpeakerList &activeSpeakers = microphone->GetActiveSpeakers();
		const decDVector micPos = microphone->GetPosition();
		const int count = activeSpeakers.GetCount();
		double lastBestDistance = -1.0;
		int i;
		
		while(closestCount > 0){
			double bestDistance = lastBestDistance;
			deoalASpeaker *bestSpeaker = NULL;
			
			for(i=0; i<count; i++){
				deoalASpeaker * const speaker = activeSpeakers.GetAt(i);
				if(! speaker->GetPlaying() || ! speaker->GetEnvironment()){
					continue;
				}
				
				const double distance = (speaker->GetPosition() - micPos).Length();
				if(distance <= lastBestDistance){
					continue;
				}
				
				if(! bestSpeaker || distance < bestDistance){
					bestSpeaker = speaker;
					bestDistance = distance;
				}
			}
			
			if(bestSpeaker){
				speakers.Add(bestSpeaker);
				lastBestDistance = bestDistance;
				closestCount--;
				
			}else{
				break;
			}
		}
	}
	
	const int count = pDIClosestSpeakers.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deDebugBlockInfo &dbi = *((deDebugBlockInfo*)pDIClosestSpeakers.GetAt(i));
		
		if(i < speakers.GetCount()){
			decString title;
			title.Format("Speaker #%d (%.1fm)", i,
				(speakers.GetAt(i)->GetPosition() - microphone->GetPosition()).Length());
			dbi.SetTitle(title);
			
			speakers.GetAt(i)->GetEnvironment()->DebugUpdateInfo(dbi);
			
		}else{
			const int entryCount = dbi.GetEntryCount();
			if(entryCount > 0){
				int j;
				for(j=0; j<entryCount; j++){
					dbi.SetEntryText(j, "-");
					dbi.SetEntryColor(j, decColor(1.0f, 1.0f, 1.0f));
				}
				dbi.UpdateView();
			}
		}
	}
}



void deoalDebugInfo::PrepareDDClosestSpeakersDirect(int count){
	while(pDDClosestSpeakersDirect.GetCount() < count){
		pDDClosestSpeakersDirect.Add(deDebugDrawer::Ref::New(
			pAudioThread.GetOal().GetGameEngine()->GetDebugDrawerManager()->CreateDebugDrawer()));
	}
	
	while(pDDClosestSpeakersDirect.GetCount() > count){
		deDebugDrawer * const dd = (deDebugDrawer*)
			pDDClosestSpeakersDirect.GetAt(pDDClosestSpeakersDirect.GetCount() - 1);
		if(dd->GetParentWorld()){
			dd->GetParentWorld()->RemoveDebugDrawer(dd);
		}
		pDDClosestSpeakersDirect.RemoveFrom(pDDClosestSpeakersDirect.GetCount() - 1);
	}
	
	deoalMicrophone * const microphone = pAudioThread.GetOal().GetActiveMicrophone();
	int i;
	
	if(microphone){
		deWorld * const world = microphone->GetParentWorld()
			? &microphone->GetParentWorld()->GetWorld() : NULL;
		
		for(i=0; i<count; i++){
			deDebugDrawer * const dd = (deDebugDrawer*)pDDClosestSpeakersDirect.GetAt(i);
			if(dd->GetParentWorld() != world){
				if(dd->GetParentWorld()){
					dd->GetParentWorld()->RemoveDebugDrawer(dd);
				}
				if(world){
					world->AddDebugDrawer(dd);
				}
			}
		}
		
	}else{
		for(i=0; i<count; i++){
			deDebugDrawer * const dd = (deDebugDrawer*)pDDClosestSpeakersDirect.GetAt(i);
			if(dd->GetParentWorld()){
				dd->GetParentWorld()->RemoveDebugDrawer(dd);
			}
		}
	}
}

void deoalDebugInfo::CaptureDDClosestSpeakersDirect(){
	const int captureCount = pAudioThread.GetOal().GetDevMode()->GetCaptureSpeakerClosestDirect();
	if(captureCount == 0){
		return;
	}
	
	PrepareDDClosestSpeakersDirect(captureCount);
	pAudioThread.GetOal().GetDevMode()->ClearCaptureSpeakerClosestDirect();
	
	const deoalAMicrophone * const microphone = pAudioThread.GetActiveMicrophone();
	deoalSpeakerList speakers;
	
	if(microphone){
		int closestCount = captureCount;
		const deoalSpeakerList &activeSpeakers = microphone->GetActiveSpeakers();
		const decDVector micPos = microphone->GetPosition();
		const int count = activeSpeakers.GetCount();
		double lastBestDistance = -1.0;
		int i;
		
		while(closestCount > 0){
			double bestDistance = lastBestDistance;
			deoalASpeaker *bestSpeaker = NULL;
			
			for(i=0; i<count; i++){
				deoalASpeaker * const speaker = activeSpeakers.GetAt(i);
				if(! speaker->GetPlaying() || ! speaker->GetEnvironment()){
					continue;
				}
				
				const double distance = (speaker->GetPosition() - micPos).Length();
				if(distance <= lastBestDistance){
					continue;
				}
				
				if(! bestSpeaker || distance < bestDistance){
					bestSpeaker = speaker;
					bestDistance = distance;
				}
			}
			
			if(bestSpeaker){
				speakers.Add(bestSpeaker);
				lastBestDistance = bestDistance;
				closestCount--;
				
			}else{
				break;
			}
		}
	}
	
	int i;
	for(i=0; i<captureCount; i++){
		deDebugDrawer &dd = *((deDebugDrawer*)pDDClosestSpeakersDirect.GetAt(i));
		dd.RemoveAllShapes();
		if(i < speakers.GetCount() && microphone){
			speakers.GetAt(i)->GetEnvironment()->DebugUpdateDirect(dd, *microphone);
		}
	}
}

void deoalDebugInfo::VisAudSpeakers(int mode){
	pModeVisAudSpeakers = mode;
	
	if(mode == 0){
		const int count = pDDVisAudSpeakers.GetCount();
		int i;
		for(i=0; i<count; i++){
			deDebugDrawer * const dd = (deDebugDrawer*)pDDVisAudSpeakers.GetAt(i);
			if(dd->GetParentWorld()){
				dd->GetParentWorld()->RemoveDebugDrawer(dd);
			}
		}
		pDDVisAudSpeakers.RemoveAll();
	}
}

void deoalDebugInfo::UpdateVisAudSpeakers(){
	if(pModeVisAudSpeakers == 0){
		return;
	}
	
	deoalMicrophone * const engMicrophone = pAudioThread.GetOal().GetActiveMicrophone();
	deoalAMicrophone * const microphone = pAudioThread.GetActiveMicrophone();
	int count = 0;
	
	if(engMicrophone && microphone && engMicrophone->GetParentWorld()){
		deWorld &world = engMicrophone->GetParentWorld()->GetWorld();
		const deoalSpeakerList &speakers = microphone->GetActiveSpeakers();
		const bool useEnv = pAudioThread.GetConfiguration().GetAuralizationMode()
			!= deoalConfiguration::eAuralizationModes::eamDisabled 
			&& pAudioThread.GetConfiguration().GetEnableEFX();
		const decColor soundHigh(1.0f, 0.0f, 0.0f);
		const decColor soundLow(0.0f, 0.5f, 0.0f);
		float factor;
		int i;
		
		for(i=0; i<speakers.GetCount(); i++){
			const deoalASpeaker &speaker = *speakers.GetAt(i);
			if(! speaker.GetEnabled() || ! speaker.GetPlaying()){
				continue;
			}
			
			if(useEnv && (! speaker.GetEnvironment() || ! speaker.GetEnvironment()->GetValid())){
				continue;
			}
			
			if(count == pDDVisAudSpeakers.GetCount()){
				const deDebugDrawer::Ref dd(deDebugDrawer::Ref::New(pAudioThread.GetOal().
					GetGameEngine()->GetDebugDrawerManager()->CreateDebugDrawer()));
				dd->SetXRay(true);
				
				deDebugDrawerShape * const shape = new deDebugDrawerShape;
				shape->GetShapeList().Add(new decShapeSphere(0.05f));
				dd->AddShape(shape);
				
				world.AddDebugDrawer(dd);
				pDDVisAudSpeakers.Add(dd);
			}
			
			deDebugDrawer &dd = *((deDebugDrawer*)pDDVisAudSpeakers.GetAt(count++));
			
			if(speaker.GetEnvironment() || speaker.GetEnvironment()->GetValid()){
				const deoalEnvironment &env = *speaker.GetEnvironment();
				float low, high;
				
				switch(pModeVisAudSpeakers){
				case 1:
					factor = speaker.GetAttenuatedGain();
					break;
					
				case 2:
					low = env.GetBandPassGain() * env.GetBandPassGainLF();
					high = env.GetBandPassGain() * env.GetBandPassGainHF();
					factor = decMath::max(low, high);
					break;
					
				case 3:
					low = env.GetReverbGain() * env.GetReverbGainLF() * env.GetReverbReflectionGain();
					high = env.GetReverbGain() * env.GetReverbGainHF() * env.GetReverbReflectionGain();
					factor = decMath::max(low, high);
					break;
					
				case 4:
					low = env.GetReverbGain() * env.GetReverbGainLF() * env.GetReverbLateReverbGain();
					high = env.GetReverbGain() * env.GetReverbGainHF() * env.GetReverbLateReverbGain();
					factor = decMath::max(low, high);
					break;
					
				case 5:
					low = env.GetBandPassGain() * env.GetBandPassGainLF()
						+ env.GetReverbGain() * env.GetReverbGainLF() * env.GetReverbReflectionGain()
						+ env.GetReverbGain() * env.GetReverbGainLF() * env.GetReverbLateReverbGain();
					high = env.GetBandPassGain() * env.GetBandPassGainHF()
						+ env.GetReverbGain() * env.GetReverbGainHF() * env.GetReverbReflectionGain()
						+ env.GetReverbGain() * env.GetReverbGainHF() * env.GetReverbLateReverbGain();
					factor = speaker.GetAttenuatedGain() + decMath::max(low, high);
					break;
					
				case 6:
					low = env.GetBandPassGain() * env.GetBandPassGainLF();
					high = env.GetBandPassGain() * env.GetBandPassGainHF();
					factor = speaker.GetFinalGain() * decMath::max(low, high);
					
					low = env.GetReverbGain() * env.GetReverbGainLF() * env.GetReverbReflectionGain()
						+ env.GetReverbGain() * env.GetReverbGainLF() * env.GetReverbLateReverbGain();
					high = env.GetReverbGain() * env.GetReverbGainHF() * env.GetReverbReflectionGain()
						+ env.GetReverbGain() * env.GetReverbGainHF() * env.GetReverbLateReverbGain();
					factor += decMath::max(low, high);
					break;
					
				default:
					factor = 0.0f;
				}
				factor = decMath::clamp(factor, 0.0f, 1.0f);
				
			}else{
				factor = 1.0f;
			}
			
			dd.GetShapeAt(0)->SetEdgeColor(soundLow * (1.0f - factor) + soundHigh * factor);
			dd.NotifyShapeColorChanged();
			dd.SetPosition(speaker.GetPosition());
		}
	}
	
	while(pDDVisAudSpeakers.GetCount() > count){
		const int index = pDDVisAudSpeakers.GetCount() - 1;
		deDebugDrawer * const dd = (deDebugDrawer*)pDDVisAudSpeakers.GetAt(index);
		if(dd->GetParentWorld()){
			dd->GetParentWorld()->RemoveDebugDrawer(dd);
		}
		pDDVisAudSpeakers.RemoveFrom(index);
	}
}
