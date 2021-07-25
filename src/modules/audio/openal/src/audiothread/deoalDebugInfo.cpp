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
#include <dragengine/debug/deDebugBlockInfo.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/world/deWorld.h>



// Class deoalDebugInfo
////////////////////////

// Constructor, destructor
////////////////////////////

deoalDebugInfo::deoalDebugInfo( deoalAudioThread &audioThread ) :
pAudioThread( audioThread ),

pDIModule( NULL ),
pDebugTimeMainThread( 29, 2 ),
pDebugTimeMainThreadWaitFinish( 29, 2 ),
pDebugTimeMainThreadSynchronize( 29, 2 ),
pDebugTimeAudioThread( 29, 2 ),
pDebugTimeAudioThreadAudio( 29, 2 ),
pDebugTimeFrameLimiterMain( 0.0f ),
pDebugTimeFrameLimiterAudio( 0.0f ),
pDebugTimeFrameLimiterAudioEstimated( 0.0f ),
pDebugFPSMain( 0 ),
pDebugFPSAudio( 0 ),
pDebugFPSAudioEstimated( 0 ),

pDIActiveMic( NULL ),
pDISpeakerAtPosition( NULL ){
}

deoalDebugInfo::~deoalDebugInfo(){
	PrepareDIClosestSpeakers( 0 );
	PrepareDDClosestSpeakersDirect( 0 );
	HideDISpeakerAtPosition();
	HideDIActiveMic();
	HideDDActiveMicRays();
	HideDIModule();
}



// Management
///////////////

void deoalDebugInfo::ResetTimersMainThread(){
	if( ! pDIModule ){
		return;
	}
	
	pDebugTimerMainThread1.Reset();
	pDebugTimerMainThread2.Reset();
}

void deoalDebugInfo::StoreTimeThreadMain( float time ){
	if( ! pDIModule ){
		return;
	}
	
	pDebugTimeMainThread.Add( time );
}

void deoalDebugInfo::StoreTimeThreadMainWaitFinish(){
	if( ! pDIModule ){
		return;
	}
	
	pDebugTimeMainThreadWaitFinish.Add( pDebugTimerMainThread2.GetElapsedTime() );
}

void deoalDebugInfo::StoreTimeThreadMainSynchronize(){
	if( ! pDIModule ){
		return;
	}
	
	pDebugTimeMainThreadSynchronize.Add( pDebugTimerMainThread2.GetElapsedTime() );
	pDebugTimeMainThread.Add( pDebugTimerMainThread1.GetElapsedTime() );
}

void deoalDebugInfo::ResetTimersAudioThread(){
	if( ! pDIModule ){
		return;
	}
	
	pDebugTimerAudioThread1.Reset();
	pDebugTimerAudioThread2.Reset();
}

void deoalDebugInfo::StoreTimeAudioThreadAudio(){
	if( ! pDIModule ){
		return;
	}
	
	pDebugTimeAudioThreadAudio.Add( pDebugTimerAudioThread2.GetElapsedTime() );
}

void deoalDebugInfo::StoreTimeAudioThread(){
	if( ! pDIModule ){
		return;
	}
	
	pDebugTimeAudioThread.Add( pDebugTimerAudioThread1.GetElapsedTime() );
}

void deoalDebugInfo::StoreTimeFrameLimiter( const decTimeHistory &main,
const decTimeHistory &audio, const decTimeHistory &audioEstimated ){
	if( ! pDIModule ){
		return;
	}
	
	if( main.HasMetrics() ){
		pDebugTimeFrameLimiterMain = main.GetAverage();
		pDebugFPSMain = ( int )( ( 1.0f / pDebugTimeFrameLimiterMain ) + 0.5f );
	}
	if( audio.HasMetrics() ){
		pDebugTimeFrameLimiterAudio = audio.GetAverage();
		pDebugFPSAudio = ( int )( ( 1.0f / pDebugTimeFrameLimiterAudio ) + 0.5f );
	}
	if( audioEstimated.HasMetrics() ){
		pDebugTimeFrameLimiterAudioEstimated = audioEstimated.GetAverage();
		pDebugFPSAudioEstimated = ( int )( ( 1.0f / pDebugTimeFrameLimiterAudioEstimated ) + 0.5f );
	}
}



void deoalDebugInfo::UpdateDebugInfo(){
	if( ! pAudioThread.GetDebug().GetEnabled() ){
		return;
	}
	
	UpdateDIModule();
	UpdateDIActiveMic();
	CaptureActiveMicRays();
	UpdateDISpeakerAtPosition();
	UpdateDIClosestSpeakers();
	CaptureDDClosestSpeakersDirect();
}



enum eDebugInfoModule{
	edimMainThread,
	edimMainThreadWaitFinish,
	edimMainThreadSynchronize,
	edimAudioThread,
	edimAudioThreadAudio,
	edimAudioThreadTraceSound,
	edimAudioThreadListen,
	edimFrameLimiter,
	edimFrameLimiterEstimate,
	edimFrameLimiterFPS
};

void deoalDebugInfo::ShowDIModule(){
	if( pDIModule ){
		return;
	}
	
	pDIModule = new deDebugBlockInfo( *pAudioThread.GetOal().GetGameEngine() );
	pDIModule->SetTitle( "Module" );
	pDIModule->AddEntry( "Main Thread", "" );
	pDIModule->AddEntry( "- Wait Finish", "" );
	pDIModule->AddEntry( "- Synchronize", "" );
	pDIModule->AddEntry( "Audio Thread", "" );
	pDIModule->AddEntry( "- Audio", "" );
	pDIModule->AddEntry( "- Trace Sound", "" );
	pDIModule->AddEntry( "- Listen", "" );
	pDIModule->AddEntry( "Frame Limiter", "" );
	pDIModule->AddEntry( "- Estimate", "" );
	pDIModule->AddEntry( "- FPS", "" );
	pDIModule->UpdateView();
	pDIModule->AddToOverlay();
}

void deoalDebugInfo::HideDIModule(){
	if( ! pDIModule ){
		return;
	}
	
	delete pDIModule;
	pDIModule = NULL;
}

void deoalDebugInfo::UpdateDIModule(){
	if( ! pDIModule ){
		return;
	}
	
	decString text;
	text.Format( "%.2f", pDebugTimeMainThread.GetAverage() * 1000.0f );
	pDIModule->SetEntryText( edimMainThread, text );
	
	text.Format( "%.2f", pDebugTimeMainThreadWaitFinish.GetAverage() * 1000.0f );
	pDIModule->SetEntryText( edimMainThreadWaitFinish, text );
	
	text.Format( "%.2f", pDebugTimeMainThreadSynchronize.GetAverage() * 1000.0f );
	pDIModule->SetEntryText( edimMainThreadSynchronize, text );
	
	text.Format( "%.2f", pDebugTimeAudioThread.GetAverage() * 1000.0f );
	pDIModule->SetEntryText( edimAudioThread, text );
	
	text.Format( "%.2f", pDebugTimeAudioThreadAudio.GetAverage() * 1000.0f );
	pDIModule->SetEntryText( edimAudioThreadAudio, text );
	
	const deoalRTParallelEnvProbe &rtpenv = pAudioThread.GetRTParallelEnvProbe();
	text.Format( "%.1f %.1f %.1f", rtpenv.GetTimeHistoryTraceSoundRays().GetAverage() * 1000.0f,
		rtpenv.GetTimeHistoryTraceSoundRays().GetMinimum() * 1000.0f,
		rtpenv.GetTimeHistoryTraceSoundRays().GetMaximum() * 1000.0f );
	pDIModule->SetEntryText( edimAudioThreadTraceSound, text );
	
	text.Format( "%.1f %.1f %.1f", rtpenv.GetTimeHistoryListen().GetAverage() * 1000.0f,
		rtpenv.GetTimeHistoryListen().GetMinimum() * 1000.0f,
		rtpenv.GetTimeHistoryListen().GetMaximum() * 1000.0f );
	pDIModule->SetEntryText( edimAudioThreadListen, text );
	
	text.Format( "%.1f %.1f %.1f", pDebugTimeFrameLimiterMain * 1000.0f,
		pDebugTimeFrameLimiterAudio * 1000.0f, pDebugTimeFrameLimiterAudioEstimated * 1000.0f );
	pDIModule->SetEntryText( edimFrameLimiterEstimate, text );
	
	text.Format( "%d %d %d", decMath::min( pDebugFPSMain, 999 ),
		decMath::min( pDebugFPSAudio, 999 ), decMath::min( pDebugFPSAudioEstimated, 999 ) );
	pDIModule->SetEntryText( edimFrameLimiterFPS, text );
	
	pDIModule->UpdateView();
}



void deoalDebugInfo::ShowDIActiveMic(){
	if( pDIActiveMic ){
		return;
	}
	
	pDIActiveMic = new deDebugBlockInfo( *pAudioThread.GetOal().GetGameEngine() );
	pDIActiveMic->SetTitle( "Microphone" );
	pDIActiveMic->UpdateView();
	pDIActiveMic->AddToOverlay();
}

void deoalDebugInfo::HideDIActiveMic(){
	if( ! pDIActiveMic ){
		return;
	}
	
	delete pDIActiveMic;
	pDIActiveMic = NULL;
}

void deoalDebugInfo::UpdateDIActiveMic(){
	if( ! pDIActiveMic ){
		return;
	}
	
	deoalAMicrophone * const microphone = pAudioThread.GetActiveMicrophone();
	if( microphone ){
		microphone->DebugUpdateInfo( *pDIActiveMic );
		
	}else{
		const int count = pDIActiveMic->GetEntryCount();
		if( count > 0 ){
			int i;
			for( i=0; i<count; i++ ){
				pDIActiveMic->SetEntryText( i, "-" );
				pDIActiveMic->SetEntryColor( i, decColor( 1.0f, 1.0f, 1.0f ) );
			}
			pDIActiveMic->UpdateView();
		}
	}
}



void deoalDebugInfo::ShowDDActiveMicRays(){
	if( pDDActiveMicRays ){
		return;
	}
	
	pDDActiveMicRays.TakeOver( pAudioThread.GetOal().GetGameEngine()
		->GetDebugDrawerManager()->CreateDebugDrawer() );
	pDDActiveMicRays->SetXRay( false );
	pDDActiveMicRays->SetVisible( false );
}

void deoalDebugInfo::HideDDActiveMicRays(){
	if( ! pDDActiveMicRays ){
		return;
	}
	
	if( pDDActiveMicRays->GetParentWorld() ){
		pDDActiveMicRays->GetParentWorld()->RemoveDebugDrawer( pDDActiveMicRays );
	}
	pDDActiveMicRays = NULL;
}

void deoalDebugInfo::CaptureActiveMicRays(){
	if( ! pDDActiveMicRays || ! pAudioThread.GetOal().GetDevMode()->GetCaptureMicRays() ){
		return;
	}
	
	pAudioThread.GetOal().GetDevMode()->ClearCaptureMicRays();
	
	deoalMicrophone * const microphone = pAudioThread.GetOal().GetActiveMicrophone();
	if( microphone ){
		deWorld * const world = microphone->GetParentWorld()
			? &microphone->GetParentWorld()->GetWorld() : NULL;
		
		if( pDDActiveMicRays->GetParentWorld() != world ){
			if( pDDActiveMicRays->GetParentWorld() ){
				pDDActiveMicRays->GetParentWorld()->RemoveDebugDrawer( pDDActiveMicRays );
			}
			if( world ){
				world->AddDebugDrawer( pDDActiveMicRays );
			}
		}
		microphone->GetAMicrophone()->DebugCaptureRays( pDDActiveMicRays,
			pAudioThread.GetOal().GetDevMode()->GetCaptureMicRaysXRay() );
		
	}else{
		if( pDDActiveMicRays->GetParentWorld() ){
			pDDActiveMicRays->GetParentWorld()->RemoveDebugDrawer( pDDActiveMicRays );
		}
		
		if( pDDActiveMicRays->GetShapeCount() > 0 ){
			pDDActiveMicRays->RemoveAllShapes();
			pDDActiveMicRays->NotifyShapeGeometryChanged();
		}
	}
}



void deoalDebugInfo::ShowDISpeakerAtPosition(){
	if( pDISpeakerAtPosition ){
		return;
	}
	
	pDISpeakerAtPosition = new deDebugBlockInfo( *pAudioThread.GetOal().GetGameEngine() );
	pDISpeakerAtPosition->SetTitle( "Speaker At" );
	pDISpeakerAtPosition->UpdateView();
	pDISpeakerAtPosition->AddToOverlay();
}

void deoalDebugInfo::HideDISpeakerAtPosition(){
	if( ! pDISpeakerAtPosition ){
		return;
	}
	
	delete pDISpeakerAtPosition;
	pDISpeakerAtPosition = NULL;
}

void deoalDebugInfo::UpdateDISpeakerAtPosition(){
	if( ! pDISpeakerAtPosition ){
		return;
	}
	
	deoalAMicrophone * const microphone = pAudioThread.GetActiveMicrophone();
	deoalASpeaker *speaker = NULL;
	
	if( microphone ){
		const deoalSpeakerList &speakers = microphone->GetActiveSpeakers();
		const decDVector &pos = pAudioThread.GetDebug().GetShowSpeakerEnvInfoAtPosition();
		int i;
		for( i=0; i<speakers.GetCount(); i++ ){
			deoalASpeaker * const checkSpeaker = speakers.GetAt( i );
			if( checkSpeaker->GetPosition().IsEqualTo( pos, 0.01 ) ){
				speaker = checkSpeaker;
				break;
			}
		}
	}
	
	if( speaker && speaker->GetEnvironment() ){
		speaker->GetEnvironment()->DebugUpdateInfo( *pDISpeakerAtPosition );
		
	}else{
		const int count = pDISpeakerAtPosition->GetEntryCount();
		if( count > 0 ){
			int i;
			for( i=0; i<count; i++ ){
				pDISpeakerAtPosition->SetEntryText( i, "-" );
				pDISpeakerAtPosition->SetEntryColor( i, decColor( 1.0f, 1.0f, 1.0f ) );
			}
			pDISpeakerAtPosition->UpdateView();
		}
	}
}



void deoalDebugInfo::PrepareDIClosestSpeakers( int count ){
	while( pDIClosestSpeakers.GetCount() < count ){
		deDebugBlockInfo * const dbi = new deDebugBlockInfo( *pAudioThread.GetOal().GetGameEngine() );
		decString title;
		title.Format( "Speaker #%d", pDIClosestSpeakers.GetCount() );
		dbi->SetTitle( title );
		dbi->AddToOverlay();
		pDIClosestSpeakers.Add( dbi );
	}
	
	while( pDIClosestSpeakers.GetCount() > count ){
		delete ( deDebugBlockInfo* )pDIClosestSpeakers.GetAt( pDIClosestSpeakers.GetCount() - 1 );
		pDIClosestSpeakers.RemoveFrom( pDIClosestSpeakers.GetCount() - 1 );
	}
}

void deoalDebugInfo::UpdateDIClosestSpeakers(){
	if( pDIClosestSpeakers.GetCount() == 0 ){
		return;
	}
	
	const deoalAMicrophone * const microphone = pAudioThread.GetActiveMicrophone();
	deoalSpeakerList speakers;
	
	if( microphone ){
		int closestCount = pDIClosestSpeakers.GetCount();
		const deoalSpeakerList &activeSpeakers = microphone->GetActiveSpeakers();
		const decDVector micPos = microphone->GetPosition();
		const int count = activeSpeakers.GetCount();
		double lastBestDistance = -1.0;
		int i;
		
		while( closestCount > 0 ){
			double bestDistance = lastBestDistance;
			deoalASpeaker *bestSpeaker = NULL;
			
			for( i=0; i<count; i++ ){
				deoalASpeaker * const speaker = activeSpeakers.GetAt( i );
				if( ! speaker->GetPlaying() || ! speaker->GetEnvironment() ){
					continue;
				}
				
				const double distance = ( speaker->GetPosition() - micPos ).Length();
				if( distance <= lastBestDistance ){
					continue;
				}
				
				if( ! bestSpeaker || distance < bestDistance ){
					bestSpeaker = speaker;
					bestDistance = distance;
				}
			}
			
			if( bestSpeaker ){
				speakers.Add( bestSpeaker );
				lastBestDistance = bestDistance;
				closestCount--;
				
			}else{
				break;
			}
		}
	}
	
	const int count = pDIClosestSpeakers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deDebugBlockInfo &dbi = *( ( deDebugBlockInfo* )pDIClosestSpeakers.GetAt( i ) );
		
		if( i < speakers.GetCount() ){
			decString title;
			title.Format( "Speaker #%d (%.1fm)", i,
				( speakers.GetAt( i )->GetPosition() - microphone->GetPosition() ).Length() );
			dbi.SetTitle( title );
			
			speakers.GetAt( i )->GetEnvironment()->DebugUpdateInfo( dbi );
			
		}else{
			const int entryCount = dbi.GetEntryCount();
			if( entryCount > 0 ){
				int j;
				for( j=0; j<entryCount; j++ ){
					dbi.SetEntryText( j, "-" );
					dbi.SetEntryColor( j, decColor( 1.0f, 1.0f, 1.0f ) );
				}
				dbi.UpdateView();
			}
		}
	}
}



void deoalDebugInfo::PrepareDDClosestSpeakersDirect( int count ){
	while( pDDClosestSpeakersDirect.GetCount() < count ){
		deObjectReference dd;
		dd.TakeOver( pAudioThread.GetOal().GetGameEngine()->GetDebugDrawerManager()->CreateDebugDrawer() );
		pDDClosestSpeakersDirect.Add( dd );
	}
	
	while( pDDClosestSpeakersDirect.GetCount() > count ){
		deDebugDrawer * const dd = ( deDebugDrawer* )
			pDDClosestSpeakersDirect.GetAt( pDDClosestSpeakersDirect.GetCount() - 1 );
		if( dd->GetParentWorld() ){
			dd->GetParentWorld()->RemoveDebugDrawer( dd );
		}
		pDDClosestSpeakersDirect.RemoveFrom( pDDClosestSpeakersDirect.GetCount() - 1 );
	}
	
	deoalMicrophone * const microphone = pAudioThread.GetOal().GetActiveMicrophone();
	int i;
	
	if( microphone ){
		deWorld * const world = microphone->GetParentWorld()
			? &microphone->GetParentWorld()->GetWorld() : NULL;
		
		for( i=0; i<count; i++ ){
			deDebugDrawer * const dd = ( deDebugDrawer* )pDDClosestSpeakersDirect.GetAt( i );
			if( dd->GetParentWorld() != world ){
				if( dd->GetParentWorld() ){
					dd->GetParentWorld()->RemoveDebugDrawer( dd );
				}
				if( world ){
					world->AddDebugDrawer( dd );
				}
			}
		}
		
	}else{
		for( i=0; i<count; i++ ){
			deDebugDrawer * const dd = ( deDebugDrawer* )pDDClosestSpeakersDirect.GetAt( i );
			if( dd->GetParentWorld() ){
				dd->GetParentWorld()->RemoveDebugDrawer( dd );
			}
		}
	}
}

void deoalDebugInfo::CaptureDDClosestSpeakersDirect(){
	const int captureCount = pAudioThread.GetOal().GetDevMode()->GetCaptureSpeakerClosestDirect();
	if( captureCount == 0 ){
		return;
	}
	
	PrepareDDClosestSpeakersDirect( captureCount );
	pAudioThread.GetOal().GetDevMode()->ClearCaptureSpeakerClosestDirect();
	
	const deoalAMicrophone * const microphone = pAudioThread.GetActiveMicrophone();
	deoalSpeakerList speakers;
	
	if( microphone ){
		int closestCount = captureCount;
		const deoalSpeakerList &activeSpeakers = microphone->GetActiveSpeakers();
		const decDVector micPos = microphone->GetPosition();
		const int count = activeSpeakers.GetCount();
		double lastBestDistance = -1.0;
		int i;
		
		while( closestCount > 0 ){
			double bestDistance = lastBestDistance;
			deoalASpeaker *bestSpeaker = NULL;
			
			for( i=0; i<count; i++ ){
				deoalASpeaker * const speaker = activeSpeakers.GetAt( i );
				if( ! speaker->GetPlaying() || ! speaker->GetEnvironment() ){
					continue;
				}
				
				const double distance = ( speaker->GetPosition() - micPos ).Length();
				if( distance <= lastBestDistance ){
					continue;
				}
				
				if( ! bestSpeaker || distance < bestDistance ){
					bestSpeaker = speaker;
					bestDistance = distance;
				}
			}
			
			if( bestSpeaker ){
				speakers.Add( bestSpeaker );
				lastBestDistance = bestDistance;
				closestCount--;
				
			}else{
				break;
			}
		}
	}
	
	int i;
	for( i=0; i<captureCount; i++ ){
		deDebugDrawer &dd = *( ( deDebugDrawer* )pDDClosestSpeakersDirect.GetAt( i ) );
		dd.RemoveAllShapes();
		if( i < speakers.GetCount() && microphone ){
			speakers.GetAt( i )->GetEnvironment()->DebugUpdateDirect( dd, *microphone );
		}
	}
}
