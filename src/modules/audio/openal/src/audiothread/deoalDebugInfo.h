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

#ifndef _DEOALDEBUGINFO_H_
#define _DEOALDEBUGINFO_H_

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/utils/decTimeHistory.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/resources/debug/deDebugDrawerReference.h>

class deoalAudioThread;

class deDebugBlockInfo;



/**
 * \brief Audio thread debugging information.
 */
class deoalDebugInfo{
private:
	deoalAudioThread &pAudioThread;
	
	deDebugBlockInfo *pDIModule;
	decTimeHistory pDebugTimeMainThread;
	decTimeHistory pDebugTimeMainThreadWaitFinish;
	decTimeHistory pDebugTimeMainThreadSynchronize;
	decTimeHistory pDebugTimeAudioThread;
	decTimeHistory pDebugTimeAudioThreadPrepare;
	decTimeHistory pDebugTimeAudioThreadProcess;
	decTimeHistory pDebugTimeAudioThreadSpeakersUpdate;
	decTimeHistory pDebugTimeAudioThreadWorldProcess;
	decTimeHistory pDebugTimeAudioThreadSpeakersProcess;
	decTimeHistory pDebugTimeAudioThreadEffectsProcess;
	float pDebugTimeFrameLimiterMain;
	float pDebugTimeFrameLimiterAudio;
	float pDebugTimeFrameLimiterAudioEstimated;
	int pDebugFPSMain;
	int pDebugFPSAudio;
	int pDebugFPSAudioEstimated;
	
	decTimer pDebugTimerMainThread1, pDebugTimerMainThread2;
	
	decTimer pDebugTimerAudioThread1, pDebugTimerAudioThread2, pDebugTimerAudioThread3;
	decTimer pDebugTimerRenderThread3;
	
	deDebugBlockInfo *pDIActiveMic;
	deDebugDrawerReference pDDActiveMicRays;
	
	deDebugBlockInfo *pDISpeakerAtPosition;
	decPointerList pDIClosestSpeakers;
	decObjectList pDDClosestSpeakersDirect;
	
	int pModeVisAudSpeakers;
	decObjectList pDDVisAudSpeakers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render thread debugging information. */
	deoalDebugInfo( deoalAudioThread &audioThread );
	
	/** \brief Clean up render thread debugging information. */
	~deoalDebugInfo();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	void ResetTimersMainThread();
	void StoreTimeThreadMain( float time );
	void StoreTimeThreadMainWaitFinish();
	void StoreTimeThreadMainSynchronize();
	void ResetTimersAudioThread();
	void StoreTimeAudioThread();
	void StoreTimeAudioThreadPrepare();
	void StoreTimeAudioThreadProcess();
	void StoreTimeAudioThreadSpeakersUpdate();
	void StoreTimeAudioThreadWorldProcess();
	void StoreTimeAudioThreadSpeakersProcess();
	void StoreTimeAudioThreadEffectsProcess();
	void StoreTimeFrameLimiter( const decTimeHistory &main, const decTimeHistory &audio,
		const decTimeHistory &audioEstimated );
	
	
	
	void ShowDIModule();
	void HideDIModule();
	void UpdateDIModule();
	
	void ShowDIActiveMic();
	void HideDIActiveMic();
	void UpdateDIActiveMic();
	
	void ShowDDActiveMicRays();
	void HideDDActiveMicRays();
	void CaptureActiveMicRays();
	
	void ShowDISpeakerAtPosition();
	void HideDISpeakerAtPosition();
	void UpdateDISpeakerAtPosition();
	
	void PrepareDIClosestSpeakers( int count );
	void UpdateDIClosestSpeakers();
	
	void PrepareDDClosestSpeakersDirect( int count );
	void CaptureDDClosestSpeakersDirect();
	
	void VisAudSpeakers( int mode );
	void UpdateVisAudSpeakers();
	
	void UpdateDebugInfo();
	/*@}*/
};

#endif
