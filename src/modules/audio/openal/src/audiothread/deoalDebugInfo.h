/* 
 * Drag[en]gine OpenAL Audio Module
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
	decTimeHistory pDebugTimeAudioThreadAudio;
	float pDebugTimeFrameLimiterMain;
	float pDebugTimeFrameLimiterAudio;
	float pDebugTimeFrameLimiterAudioEstimated;
	int pDebugFPSMain;
	int pDebugFPSAudio;
	int pDebugFPSAudioEstimated;
	
	decTimer pDebugTimerMainThread1;
	decTimer pDebugTimerMainThread2;
	
	decTimer pDebugTimerAudioThread1;
	decTimer pDebugTimerAudioThread2;
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
	void StoreTimeAudioThreadAudio();
	void StoreTimeAudioThread();
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
