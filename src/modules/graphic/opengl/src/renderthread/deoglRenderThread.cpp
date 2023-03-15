/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "deoglRenderThread.h"
#include "deoglRTBufferObject.h"
#include "deoglRTChoices.h"
#include "deoglRTContext.h"
#include "deoglRTDebug.h"
#include "deoglRTDefaultTextures.h"
#include "deoglRTFramebuffer.h"
#include "deoglRTLogger.h"
#include "deoglRTRenderers.h"
#include "deoglRTShader.h"
#include "deoglRTTexture.h"
#include "deoglRTUniqueKey.h"
#include "../deGraphicOpenGl.h"
#include "../deoglBasics.h"
#include "../debug/deoglDebugTraceGroup.h"
#include "../canvas/deoglCanvasView.h"
#include "../canvas/capture/deoglRCaptureCanvas.h"
#include "../canvas/render/deoglRCanvasView.h"
#include "../capabilities/deoglCapabilities.h"
#include "../configuration/deoglConfiguration.h"
#include "../debug/deoglDebugInformation.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../devmode/deoglDeveloperMode.h"
#include "../envmap/deoglEnvMapSlotManager.h"
#include "../extensions/deoglExtensions.h"
#include "../gi/deoglGI.h"
#include "../light/deoglLightBoundaryMap.h"
#include "../occlusiontest/deoglOcclusionTestPool.h"
#include "../occquery/deoglOcclusionQueryManager.h"
#include "../optimizer/deoglOptimizerManager.h"
#include "../pipeline/deoglPipelineManager.h"
#include "../rendering/deoglRenderCanvas.h"
#include "../rendering/defren/deoglDeferredRendering.h"
#include "../rendering/task/persistent/deoglPersistentRenderTaskPool.h"
#include "../rendering/task/shared/deoglRenderTaskSharedPool.h"
#include "../shadow/deoglShadowMapper.h"
#include "../texture/deoglTextureStageManager.h"
#include "../triangles/deoglTriangleSorter.h"
#include "../utils/deoglQuickSorter.h"
#include "../vbo/deoglSharedVBOListList.h"
#include "../vr/deoglVR.h"
#include "../window/deoglRenderWindow.h"
#include "../window/deoglRRenderWindow.h"
#include "../world/deoglRCamera.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOS.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/systems/deScriptingSystem.h>

#ifdef ANDROID
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/deGraphicSystem.h>
#endif



// Class deoglRenderThread
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderThread::deoglRenderThread( deGraphicOpenGl &ogl ) :
pOgl( ogl ),

pAsyncRendering( true ),
pConfigChanged( true ),
pFrameCounter( 0 ),
pVRCamera( nullptr ),

pLeakTracker( *this ),

pCanvasInputOverlay( nullptr ),
pCanvasDebugOverlay( nullptr ),

pChoices( nullptr ),
pBufferObject( nullptr ),
pContext( nullptr ),
pDebug( nullptr ),
pDefaultTextures( nullptr ),
pFramebuffer( nullptr ),
pLogger( nullptr ),
pRenderers( nullptr ),
pShader( nullptr ),
pTexture( nullptr ),

pCapabilities( nullptr ),
pDeferredRendering( nullptr ),
pDelayedOperations( nullptr ),
pEnvMapSlotManager( nullptr ),
pExtensions( nullptr ),
pLightBoundarybox( nullptr ),
pOccQueryMgr( nullptr ),
pGI( nullptr ),
pShadowMapper( nullptr ),
pTriangleSorter( nullptr ),
pPersistentRenderTaskPool( nullptr ),
pRenderTaskSharedPool( nullptr ),
pUniqueKey( nullptr ),
pOcclusionTestPool( nullptr ),

pTimeHistoryMain( 29, 2 ),
pTimeHistoryRender( 29, 2 ),
pTimeHistoryFrame( 29, 2 ),
pEstimatedRenderTime( 0.0f ),
pAccumulatedMainTime( 0.0f ),
pFrameTimeLimit( 1.0f / 30.0f ),

pMainThreadShowDebugInfoModule( false ),

pDebugTimeThreadMainWaitFinish( 0.0f ),

pDebugTimeThreadRenderSyncGpu( 0.0f ),
pDebugTimeThreadRenderBegin( 0.0f ),
pDebugTimeThreadRenderWindows( 0.0f ),
pDebugTimeThreadRenderWindowsPrepare( 0.0f ),
pDebugTimeThreadRenderWindowsRender( 0.0f ),
pDebugTimeThreadRenderCapture( 0.0f ),
pDebugTimeThreadRenderSwap( 0.0f ),
pDebugCountThreadWindows( 0 ),

// deprecated
pQuickSorter( nullptr ),
pOptimizerManager( nullptr ),

// thread control parameters
pInitialRenderWindow( nullptr ), // temporary variable for thread initialization
pThreadState( etsStopped ),
pThreadFailure( false ),
pBarrierSyncIn( 2 ),
pBarrierSyncOut( 2 ),
pSignalSemaphoreSyncVR( false )
{
	try{
		pConfiguration = ogl.GetConfiguration();
		pConfiguration.SetDirty( false );
		ogl.GetConfiguration().SetDirty( false );
		
		pAsyncRendering = ogl.GetConfiguration().GetAsyncRendering();
		
		pLogger = new deoglRTLogger( *this );
		
		#ifdef OS_BEOS
		SetName( "OpenGL Render" );
		#endif
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderThread::~deoglRenderThread(){
	pCleanUp();
}



// Management
///////////////

deoglLightBoundaryMap &deoglRenderThread::GetLightBoundaryMap( int size ){
	if( size > pLightBoundarybox->GetSize() ){
		int bitCount = 0;
		int temp = size;
		
		while( temp > 0 ){
			if( ( temp & 1 ) == 1 ){
				bitCount++;
			}
			temp >>= 1;
		}
		
		if( bitCount != 1 ){
			DETHROW( deeInvalidParam );
		}
		
		delete pLightBoundarybox;
		pLightBoundarybox = NULL;
		
		pLightBoundarybox = new deoglLightBoundaryMap( *this, size );
	}
	
	return *pLightBoundarybox;
}



bool deoglRenderThread::HasContext() const{
	return pContext != NULL;
}



void deoglRenderThread::SetVRCamera( deoglRCamera *camera ){
	pVRCamera = camera;
}

void deoglRenderThread::SetCanvasInputOverlay( deoglRCanvasView *canvas ){
	if( canvas == pCanvasInputOverlay ){
		return;
	}
	
	if( pCanvasInputOverlay ){
		pCanvasInputOverlay->FreeReference();
	}
	
	pCanvasInputOverlay = canvas;
	
	if( canvas ){
		canvas->AddReference();
	}
}

void deoglRenderThread::SetCanvasDebugOverlay( deoglRCanvasView *canvas ){
	if( canvas == pCanvasDebugOverlay ){
		return;
	}
	
	if( pCanvasDebugOverlay ){
		pCanvasDebugOverlay->FreeReference();
	}
	
	pCanvasDebugOverlay = canvas;
	
	if( canvas ){
		canvas->AddReference();
	}
}



// #define DEBUG_SYNC 1

#ifdef DEBUG_SYNC
	#define DEBUG_SYNC_RT_FAILURE pOgl.LogInfoFormat("RT(%i): set failure to %d\n", __LINE__, pThreadFailure);
	#define DEBUG_SYNC_RT_WAIT(b) pOgl.LogInfoFormat("RT(%i): wait on barrier %s\n", __LINE__, b);
	#define DEBUG_SYNC_RT_PASS(b) pOgl.LogInfoFormat("RT(%i): passed barrier %s\n", __LINE__, b);
	#define DEBUG_SYNC_MT_STATE pOgl.LogInfoFormat("MT(%i): state changed to %i\n", __LINE__, pThreadState);
	#define DEBUG_SYNC_MT_WAIT(b) pOgl.LogInfoFormat("MT(%i): wait on barrier %s\n", __LINE__, b);
	#define DEBUG_SYNC_MT_PASS(b) pOgl.LogInfoFormat("MT(%i): passed barrier %s\n", __LINE__, b);
#else
	#define DEBUG_SYNC_RT_FAILURE
	#define DEBUG_SYNC_RT_WAIT(b)
	#define DEBUG_SYNC_RT_PASS(b)
	#define DEBUG_SYNC_MT_STATE
	#define DEBUG_SYNC_MT_WAIT(b)
	#define DEBUG_SYNC_MT_PASS(b)
#endif

void deoglRenderThread::Init( deRenderWindow *renderWindow ){
	OGL_INIT_MAIN_THREAD_CHECK;
	
	pConfigChanged = true;
	pInitialRenderWindow = renderWindow;
	pUpdateConfigFrameLimiter();
	
	if( pAsyncRendering ){
		pLogger->LogInfoFormat( "Init rendering using asynchronous rendering" );
		
		// phase 1
		try{
			pInitThreadPhase1();
			pLogger->Synchronize(); // send asynchronous logs to game logger
			
		}catch( const deException & ){
			pLogger->Synchronize(); // send asynchronous logs to game logger
			throw;
		}
		
		// phase 2
		pThreadState = etsInitialize;
		DEBUG_SYNC_MT_STATE
		
		Start();
		
		DEBUG_SYNC_MT_WAIT("out")
		pBarrierSyncOut.Wait();
		DEBUG_SYNC_MT_PASS("out")
		
		pLogger->Synchronize(); // send asynchronous logs to game logger
		
		if( pThreadFailure ){
			pThreadState = etsStopped;
			DEBUG_SYNC_MT_STATE
			WaitForExit();
			DETHROW( deeInvalidAction );
		}
		
		// phase 3
		try{
			pInitThreadPhase3();
			pLogger->Synchronize(); // send asynchronous logs to game logger
			
		}catch( const deException & ){
			pLogger->Synchronize(); // send asynchronous logs to game logger
			throw;
		}
		
		// phase 4
		pThreadState = etsInitialize;
		DEBUG_SYNC_MT_STATE
		
		DEBUG_SYNC_MT_WAIT("in")
		pBarrierSyncIn.Wait();
		DEBUG_SYNC_MT_PASS("in")
		
		DEBUG_SYNC_MT_WAIT("out")
		pBarrierSyncOut.Wait();
		DEBUG_SYNC_MT_PASS("out")
		
		pLogger->Synchronize(); // send asynchronous logs to game logger
		
		if( pThreadFailure ){
			pThreadState = etsStopped;
			DEBUG_SYNC_MT_STATE
			WaitForExit();
			DETHROW( deeInvalidAction );
		}
		
		// ready
		pThreadState = etsSynchronize;
		DEBUG_SYNC_MT_STATE
		
		pTimerMain.Reset();
		
	}else{
		pLogger->LogInfoFormat( "Init rendering using synchronous rendering" );
		OGL_INIT_THREAD_CHECK;
		pInitThreadPhase1();
		pInitThreadPhase2();
		pInitThreadPhase3();
		pInitThreadPhase4();
	}
}

void deoglRenderThread::CleanUp(){
	if( pAsyncRendering ){
		if( pThreadState == etsStopped ){
			return;
		}
		
		WaitFinishRendering();
		
		pThreadState = etsCleaningUp;
		DEBUG_SYNC_MT_STATE
		DEBUG_SYNC_MT_WAIT("out")
		pBarrierSyncOut.Wait();
		DEBUG_SYNC_MT_PASS("out")
		WaitForExit();
		
		pLogger->Synchronize(); // send asynchronous logs to game logger
		if( pThreadFailure ){
			pThreadState = etsStopped;
			DEBUG_SYNC_MT_STATE
			Stop();
			DETHROW( deeInvalidAction );
		}
		
	}else{
		pCleanUpThread();
	}
}

#ifdef ANDROID
void deoglRenderThread::InitAppWindow(){
	if( pAsyncRendering ){
		if( pThreadState != etsWindowDown ){
			return;
		}
		
		pThreadState = etsWindowInit;
		DEBUG_SYNC_MT_STATE
		
		DEBUG_SYNC_MT_WAIT("out");
		pBarrierSyncOut.Wait();
		DEBUG_SYNC_MT_PASS("out");
		
	}else{
		pContext->InitAppWindow();
	}
}

void deoglRenderThread::TerminateAppWindow(){
	if( pAsyncRendering ){
		if( pThreadState != etsRendering ){
			return;
		}
		
		WaitFinishRendering();
		
		pThreadState = etsWindowTerminate;
		DEBUG_SYNC_MT_STATE
		
		DEBUG_SYNC_MT_WAIT("out");
		pBarrierSyncOut.Wait();
		DEBUG_SYNC_MT_PASS("out");
		
		WaitFinishRendering();
		pLogger->Synchronize();
		
		pThreadState = etsWindowDown;
		DEBUG_SYNC_MT_STATE
		
	}else{
		pContext->TerminateAppWindow();
	}
}
#endif



void deoglRenderThread::Run(){
	OGL_INIT_THREAD_CHECK;
	
	// initialize
	try{
		// phase 1
		pInitThreadPhase2();
		pThreadFailure = false;
		DEBUG_SYNC_RT_FAILURE
		
		DEBUG_SYNC_RT_WAIT("out")
		pBarrierSyncOut.Wait();
		DEBUG_SYNC_RT_PASS("out")
		
		// phase 2
		DEBUG_SYNC_RT_WAIT("in")
		pBarrierSyncIn.Wait();
		DEBUG_SYNC_RT_PASS("in")
		
		pInitThreadPhase4();
		pThreadFailure = false;
		DEBUG_SYNC_RT_FAILURE
		
	}catch( const deException &exception ){
		pLogger->LogException( exception );
		
		pThreadFailure = true;
		DEBUG_SYNC_RT_FAILURE
		
		DEBUG_SYNC_RT_WAIT("out")
		pBarrierSyncOut.Wait();
		DEBUG_SYNC_RT_PASS("out")
		
		try{
			pCleanUpThread();
			
		}catch( const deException &exception2 ){
			pLogger->LogException( exception2 );
		}
		
		return;
	}
	
	DEBUG_SYNC_RT_WAIT("out")
	pBarrierSyncOut.Wait();
	DEBUG_SYNC_RT_PASS("out")
	
	// render loop
	pTimerFrameUpdate.Reset();
	pTimerVRFrameUpdate.Reset();
	while( true ){
		// wait for entering synchronize
		DEBUG_SYNC_RT_WAIT("in")
		if( pBarrierSyncIn.TryWait( 250 ) ){
			DEBUG_SYNC_RT_PASS("in")
			
			// main thread is messing with our state here. proceed to next barrier doing nothing
			// except alter the estimated render time. this value is used by the main thread
			// only outside the synchronization part so we can update it here
			pEstimatedRenderTime = decMath::max( pTimeHistoryRender.GetAverage(), pFrameTimeLimit );
			
			// wait for leaving synchronize
			DEBUG_SYNC_RT_WAIT("out")
			pBarrierSyncOut.Wait();
			DEBUG_SYNC_RT_PASS("out")
			
		}else{
			DEBUG_SYNC_RT_PASS("in timeout")
			
			// main thread did not synchronize in time. render another time using the old state
			// then wait for synchronization again. we still have to update the estimated render
			// time though
			pEstimatedRenderTime = decMath::max( pTimeHistoryRender.GetAverage(), pFrameTimeLimit );
		}
		
		// exit if shutting down
		if( pThreadState == etsCleaningUp ){
			break;
			
		// render a single frame if ready
		}else if( pThreadState == etsRendering ){
			float elapsedRender = 0.0f;
			
			try{
				pTimerRender.Reset();
				
				pRenderSingleFrame();
				pThreadFailure = false;
				DEBUG_SYNC_RT_FAILURE
				
				elapsedRender = pTimerRender.GetElapsedTime();
				pTimeHistoryRender.Add( elapsedRender );
				//pLogger->LogInfo( decString("TimeHistory Render: ") + pTimeHistoryRender.DebugPrint() );
// 				pLogger->LogInfoFormat( "RenderThread Elapsed %.1fms (FPS %.1f)", elapsed * 1000.0f, 1.0f / decMath::max( elapsed, 0.001f ) );
				
				pLimitFrameRate( elapsedRender );
				pThreadFailure = false;
				DEBUG_SYNC_RT_FAILURE
				
			}catch( const deException &exception ){
				pLogger->LogException( exception );
				pTimerRender.Reset();
				
				if( pSignalSemaphoreSyncVR ){
					pSignalSemaphoreSyncVR = false;
					pSemaphoreSyncVR.Signal();
				}
				
				pThreadFailure = true;
				DEBUG_SYNC_RT_FAILURE
			}
			
			const float elapsedFrameUpdate = pTimerFrameUpdate.GetElapsedTime();
			pTimeHistoryFrame.Add( elapsedFrameUpdate );
			
			/*
			pLogger->LogInfoFormat( "RenderThread render=%.1fms frameUpdate=%.1fms fps=%.1f",
				elapsedRender * 1000.0f, elapsedFrameUpdate * 1000.0f,
				1.0f / decMath::max( elapsedFrameUpdate, 0.001f ) );
			*/
			
		#ifdef ANDROID
		}else if( pThreadState == etsWindowTerminate ){
			try{
				pContext->TerminateAppWindow();
				pThreadFailure = false;
				DEBUG_SYNC_RT_FAILURE
				
			}catch( const deException &exception ){
				pLogger->LogException( exception );
				pThreadFailure = true;
				DEBUG_SYNC_RT_FAILURE
			}
			
		}else if( pThreadState == etsWindowInit ){
			try{
				pContext->InitAppWindow();
				pThreadFailure = false;
				DEBUG_SYNC_RT_FAILURE
				
			}catch( const deException &exception ){
				pLogger->LogException( exception );
				pThreadFailure = true;
				DEBUG_SYNC_RT_FAILURE
			}
			
		#endif
		
		// everything else take another turn
		}
	}
	
	// clean up
	try{
		pCleanUpThread();
		pThreadFailure = false;
		DEBUG_SYNC_RT_FAILURE
		
	}catch( const deException &exception ){
		pLogger->LogException( exception );
		pThreadFailure = true;
		DEBUG_SYNC_RT_FAILURE
	}
}

void deoglRenderThread::FinalizeAsyncResLoading(){
	pDelayedOperations->ProcessAsyncResInitOperations();
}

void deoglRenderThread::Synchronize(){
	pLogger->Synchronize(); // send asynchronous logs to game logger
	
	#ifdef ANDROID
	if( pContext->GetScreenResized() ){
		pContext->ClearScreenResized();
		
		deGraphicSystem &graSys = *pOgl.GetGameEngine()->GetGraphicSystem();
		const int width = pContext->GetScreenWidth();
		const int height = pContext->GetScreenHeight();
		
		graSys.GetRenderWindow()->SetSize( width, height );
		if( graSys.GetInputOverlayCanvas() ){
			graSys.GetInputOverlayCanvas()->SetSize( decPoint( width, height ) );
			pOgl.GetGameEngine()->GetInputSystem()->ScreenSizeChanged();
		}
	}
	#endif
	
	pDelayedOperations->ProcessSynchronizeOperations();
	
	pSyncConfiguration();
	
	if( pMainThreadShowDebugInfoModule ){
		const float timeSync = pDebugTimerMainThread2.GetElapsedTime();
		const float timeMain = pDebugTimerMainThread1.GetElapsedTime();
		
		pDebugInfoThreadMain->Clear();
		pDebugInfoThreadMain->IncrementElapsedTime( timeMain );
		
		pDebugInfoThreadMainWaitFinish->Clear();
		pDebugInfoThreadMainWaitFinish->IncrementElapsedTime( pDebugTimeThreadMainWaitFinish );
		
		pDebugInfoThreadMainSynchronize->Clear();
		pDebugInfoThreadMainSynchronize->IncrementElapsedTime( timeSync );
	}
	
	if( pDebugInfoFrameLimiter->GetVisible() ){
		pDebugInfoFLEstimMain->Clear();
		pDebugInfoFLFrameRateMain->Clear();
		
		if( pTimeHistoryMain.HasMetrics() ){
			pDebugInfoFLEstimMain->IncrementElapsedTime( pTimeHistoryMain.GetAverage() );
			pDebugInfoFLFrameRateMain->IncrementCounter( ( int )( 1.0f / pTimeHistoryMain.GetAverage() ) );
		}
	}
	
	pMainThreadShowDebugInfoModule = pDebugInfoModule->GetVisible();
	
	pFPSRate = 0;
	if( pTimeHistoryFrame.HasMetrics() ){
		pFPSRate = ( int )( 1.0f / pTimeHistoryFrame.GetAverage() );
	}
	
	if( pAsyncRendering ){
		// begin rendering next frame unless thread is not active
		if( pThreadState == etsStopped ){
			return;
		}
		
		const bool hasVR = pVRCamera && pVRCamera->GetVR();
		if( hasVR ){
			pSignalSemaphoreSyncVR = true;
		}
		
		pThreadState = etsRendering;
		DEBUG_SYNC_MT_STATE
		
		DEBUG_SYNC_MT_WAIT("out");
		pBarrierSyncOut.Wait();
		DEBUG_SYNC_MT_PASS("out");
		
		if( pThreadFailure ){
			DETHROW_INFO( deeInvalidAction, "Render thread failed. See logs" );
		}
		
		pTimerMain.Reset();
		
		if( hasVR ){
			pSemaphoreSyncVR.Wait();
		}
		
	}else{
		// render thread part here run synchronous
		pRenderSingleFrame();
	}
	
	// from event loop processing
	pOgl.GetOS()->SetAppActive( pContext->GetAppActivated() );
	
	if( pContext->GetUserRequestedQuit() ){
		pOgl.GetGameEngine()->GetScriptingSystem()->UserRequestQuit();
	}
}

void deoglRenderThread::WaitFinishRendering(){
	switch( pThreadState ){
	case etsStopped:
	case etsFinishedRendering:
	case etsFrozen:
		return;
		
	default:
		break;
	}
	
	DEBUG_SYNC_MT_WAIT("in")
	pBarrierSyncIn.Wait();
	DEBUG_SYNC_MT_PASS("in")
	
	pThreadState = etsFinishedRendering;
	DEBUG_SYNC_MT_STATE
}

bool deoglRenderThread::MainThreadWaitFinishRendering(){
	if( pAsyncRendering ){
		const float gameTime = pTimerMain.GetElapsedTime();
		
		pTimeHistoryMain.Add( gameTime );
		//pLogger->LogInfo( decString("TimeHistory Main: ") + pTimeHistoryMain.DebugPrint() );
		
		if( pTimeHistoryMain.HasMetrics() ){
			pAccumulatedMainTime += gameTime;
			
			const float ratioTimes = pOgl.GetConfiguration().GetAsyncRenderSkipSyncTimeRatio();
			const float remainingTime = pEstimatedRenderTime - pAccumulatedMainTime;
			const float estimatedGameTime = decMath::max( pTimeHistoryMain.GetAverage(), 0.001f ); // stay above 1ms
			
			/*
			pLogger->LogInfoFormat( "SkinSync: skip=%d: estimRenTime=%.1fms estimGameTime=%.1fms remainTime=%.1fms ratio=%.3f threshold=%f",
				remainingTime / estimatedGameTime >= ratioTimes, pEstimatedRenderTime * 1e3f, estimatedGameTime * 1e3f,
				remainingTime * 1e3f, remainingTime / estimatedGameTime, ratioTimes );
			*/
			
			const bool hasVR = pVRCamera && pVRCamera->GetVR();
			
			if( remainingTime / estimatedGameTime >= ratioTimes && ! hasVR ){
				return false; // enough time to do another game frame update
			}
		}
		
		// not enough time for another game frame update. do synchronization
		pAccumulatedMainTime = 0.0f;
	}
	
	if( pMainThreadShowDebugInfoModule ){
		pDebugTimerMainThread1.Reset();
		pDebugTimerMainThread2.Reset();
	}
	
	WaitFinishRendering();
	
	if( pMainThreadShowDebugInfoModule ){
		pDebugTimeThreadMainWaitFinish = pDebugTimerMainThread2.GetElapsedTime();
	}
	
	return true;
}

void deoglRenderThread::Freeze(){
	if( pThreadState != etsRendering ){
		return;
	}
	
	WaitFinishRendering();
	// wait for all pending parallel tasks to finish running
	
	pThreadState = etsFrozen;
	DEBUG_SYNC_MT_STATE
}

void deoglRenderThread::Unfreeze(){
	if( pThreadState != etsFrozen ){
		return;
	}
	
	pThreadState = etsSynchronize; // do not render, go to synchronize in barrier again
	DEBUG_SYNC_MT_STATE
	
	DEBUG_SYNC_MT_WAIT("out");
	pBarrierSyncOut.Wait();
	DEBUG_SYNC_MT_PASS("out");
}

void deoglRenderThread::CreateRenderWindow( deoglRRenderWindow *window ){
	if( ! window ){
		DETHROW( deeInvalidParam );
	}
	
	switch( pThreadState ){
	case etsInitialize: 
		// special handling during initialization
		OGL_ON_MAIN_THREAD // make sure this is never called on the render thread
		window->CreateWindow();
		break;
		
	case etsStopped:
		// thread is stopped or synchronous rendering is used which is the same
		OGL_ON_MAIN_THREAD // make sure this is never called on the render thread
		window->CreateWindow();
		break;
		
	default:
		// otherwise wait until the render thread is idle to safely create render window
		DEBUG_SYNC_MT_WAIT("in")
		pBarrierSyncIn.Wait();
		DEBUG_SYNC_MT_PASS("in")
		
		pThreadState = etsSynchronize; // do not render, go to synchronize in next barrier
		DEBUG_SYNC_MT_STATE
		
		try{
			window->CreateWindow();
			
		}catch( const deException & ){
			DEBUG_SYNC_MT_WAIT("out");
			pBarrierSyncOut.Wait();
			DEBUG_SYNC_MT_PASS("out");
			
			throw;
		}
		
		DEBUG_SYNC_MT_WAIT("out");
		pBarrierSyncOut.Wait();
		DEBUG_SYNC_MT_PASS("out");
	}
}



// Debugging
//////////////

void deoglRenderThread::SampleDebugTimerRenderThreadRenderWindowsPrepare(){
	if( ! pDebugInfoModule->GetVisible() ){
		return;
	}
	pDebugTimeThreadRenderWindowsPrepare = pDebugTimerRenderThread3.GetElapsedTime();
}

void deoglRenderThread::SampleDebugTimerRenderThreadRenderWindowsRender(){
	if( ! pDebugInfoModule->GetVisible() ){
		return;
	}
	pDebugTimeThreadRenderWindowsRender = pDebugTimerRenderThread3.GetElapsedTime();
	pDebugCountThreadWindows++;
}

void deoglRenderThread::DevModeDebugInfoChanged(){
	const deoglDeveloperMode &devmode = pDebug->GetDeveloperMode();
	
	pDebugInfoModule->SetVisible( devmode.GetEnabled() && devmode.GetShowDebugInfo() &&
		( devmode.GetDebugInfoDetails() & deoglDeveloperMode::edimModule ) == deoglDeveloperMode::edimModule );
	
	pDebugInfoFrameLimiter->SetVisible( devmode.GetEnabled() && devmode.GetShowDebugInfo() &&
		( devmode.GetDebugInfoDetails() & deoglDeveloperMode::edimFrameLimiter ) == deoglDeveloperMode::edimFrameLimiter );
	
	pRenderers->DevModeDebugInfoChanged();
	
	pDebugTimerRenderThread1.Reset();
	pDebugTimerRenderThread2.Reset();
}



// Private Functions
//////////////////////

void deoglRenderThread::pCleanUp(){
	if( pLogger ){
		delete pLogger;
		pLogger = NULL;
	}
}



void deoglRenderThread::pInitThreadPhase1(){
	pContext = new deoglRTContext( *this );
	pContext->InitPhase1( pInitialRenderWindow );
}

void deoglRenderThread::pInitThreadPhase2(){
	pContext->InitPhase2( pInitialRenderWindow );
}

void deoglRenderThread::pInitThreadPhase3(){
	pContext->InitPhase3( pInitialRenderWindow );
}

// #define DO_VULKAN_TEST

#ifdef DO_VULKAN_TEST
#include <queue/devkCommandPool.h>
#include <buffer/devkBuffer.h>
#include <descriptor/devkDescriptorPool.h>
#include <descriptor/devkDescriptorSet.h>
#include <descriptor/devkDescriptorSetLayout.h>
#include <descriptor/devkDescriptorSetLayoutConfiguration.h>
#include <pipeline/devkPipelineCompute.h>
#include <pipeline/devkPipelineConfiguration.h>
#include <queue/devkCommandBuffer.h>
#include <shader/devkShaderModule.h>
#include <renderpass/devkRenderPass.h>
#include <renderpass/devkRenderPassConfiguration.h>
#include <image/devkImage.h>
#include <image/devkImageConfiguration.h>
#include <image/devkImageView.h>
#include <framebuffer/devkFramebuffer.h>
#include <framebuffer/devkFramebufferConfiguration.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileReader.h>
#include <dragengine/common/file/decMemoryFileWriter.h>
#endif

void deoglRenderThread::pInitThreadPhase4(){
	pContext->InitPhase4( pInitialRenderWindow );
	pInitialRenderWindow = NULL;
	
	// deprecated
	pQuickSorter = new deoglQuickSorter;
	pOptimizerManager = new deoglOptimizerManager;
	// deprecated
	
	// required to be present before anything else
	pUniqueKey = new deoglRTUniqueKey;
	pRenderTaskSharedPool = new deoglRenderTaskSharedPool( *this );
	
	// init extensions
	pInitExtensions();
	
	// below depends on extensions to be initialized first
	pDebug = new deoglRTDebug( *this );
	pTexture = new deoglRTTexture( *this );
	pFramebuffer = new deoglRTFramebuffer( *this );
	pShader = new deoglRTShader( *this );
	pDelayedOperations = new deoglDelayedOperations( *this );
	pPipelineManager.TakeOver( new deoglPipelineManager( *this ) );
	
	pInitCapabilities();
	
	// verify hardware support
	bool verified = true;
	verified &= pCapabilities->Verify();
	verified &= pExtensions->VerifyPresence(); // capabilities possibly disabled extensions
	if( ! verified ){
		DETHROW_INFO( deeInvalidAction, "Required hardware support missing. Please see logs" );
	}
	
	// debug information
	const decColor colorText( 1.0f, 1.0f, 1.0f, 1.0f );
	const decColor colorBg( 0.0f, 0.0f, 0.25f, 0.75f );
	const decColor colorBgSub( 0.05f, 0.05f, 0.05f, 0.75f );
	const decColor colorBgSub2( 0.1f, 0.1f, 0.1f, 0.75f );
	const decColor colorBgSub3( 0.15f, 0.15f, 0.15f, 0.75f );
	
	pDebugInfoModule.TakeOver( new deoglDebugInformation( "Module", colorText, colorBg ) );
	pDebugInfoModule->SetVisible( false );
	pDebug->GetDebugInformationList().Add( pDebugInfoModule );
 	
	pDebugInfoThreadMain.TakeOver( new deoglDebugInformation( "Main Thread", colorText, colorBgSub ) );
	pDebugInfoModule->GetChildren().Add( pDebugInfoThreadMain );
		
		pDebugInfoThreadMainWaitFinish.TakeOver( new deoglDebugInformation( "Wait Finish", colorText, colorBgSub2 ) );
		pDebugInfoThreadMain->GetChildren().Add( pDebugInfoThreadMainWaitFinish );
		
		pDebugInfoThreadMainSynchronize.TakeOver( new deoglDebugInformation( "Synchronize", colorText, colorBgSub2 ) );
		pDebugInfoThreadMain->GetChildren().Add( pDebugInfoThreadMainSynchronize );
	
	pDebugInfoThreadRender.TakeOver( new deoglDebugInformation( "Render Thread", colorText, colorBgSub ) );
	pDebugInfoModule->GetChildren().Add( pDebugInfoThreadRender );
		
		pDebugInfoThreadRenderSyncGpu.TakeOver( new deoglDebugInformation( "Sync GPU", colorText, colorBgSub2 ) );
		pDebugInfoThreadRender->GetChildren().Add( pDebugInfoThreadRenderSyncGpu );
		
		pDebugInfoThreadRenderBegin.TakeOver( new deoglDebugInformation( "Begin", colorText, colorBgSub2 ) );
		pDebugInfoThreadRender->GetChildren().Add( pDebugInfoThreadRenderBegin );
		
		pDebugInfoThreadRenderWindows.TakeOver( new deoglDebugInformation( "Windows", colorText, colorBgSub2 ) );
		pDebugInfoThreadRender->GetChildren().Add( pDebugInfoThreadRenderWindows );
			
			pDebugInfoThreadRenderWindowsPrepare.TakeOver( new deoglDebugInformation( "Prepare", colorText, colorBgSub3 ) );
			pDebugInfoThreadRenderWindows->GetChildren().Add( pDebugInfoThreadRenderWindowsPrepare );
			
			pDebugInfoThreadRenderWindowsRender.TakeOver( new deoglDebugInformation( "Render", colorText, colorBgSub3 ) );
			pDebugInfoThreadRenderWindows->GetChildren().Add( pDebugInfoThreadRenderWindowsRender );
		
		pDebugInfoThreadRenderCapture.TakeOver( new deoglDebugInformation( "Capture", colorText, colorBgSub2 ) );
		pDebugInfoThreadRender->GetChildren().Add( pDebugInfoThreadRenderCapture );
		
		pDebugInfoThreadRenderEnd.TakeOver( new deoglDebugInformation( "End", colorText, colorBgSub2 ) );
		pDebugInfoThreadRender->GetChildren().Add( pDebugInfoThreadRenderEnd );
		
		pDebugInfoThreadRenderSwap.TakeOver( new deoglDebugInformation( "Swap Buffers", colorText, colorBgSub2 ) );
		pDebugInfoThreadRender->GetChildren().Add( pDebugInfoThreadRenderSwap );
	
	pDebugInfoFrameLimiter.TakeOver( new deoglDebugInformation( "Frame Limiter", colorText, colorBg ) );
	pDebugInfoFrameLimiter->SetVisible( false );
	pDebug->GetDebugInformationList().Add( pDebugInfoFrameLimiter );
		
		pDebugInfoFLEstimMain.TakeOver( new deoglDebugInformation( "Estimate main", colorText, colorBgSub ) );
		pDebugInfoFrameLimiter->GetChildren().Add( pDebugInfoFLEstimMain );
		
		pDebugInfoFLEstimRender.TakeOver( new deoglDebugInformation( "Estimate Render", colorText, colorBgSub ) );
		pDebugInfoFrameLimiter->GetChildren().Add( pDebugInfoFLEstimRender );
		
		pDebugInfoFLFrameRateMain.TakeOver( new deoglDebugInformation( "FPS Main", colorText, colorBgSub ) );
		pDebugInfoFrameLimiter->GetChildren().Add( pDebugInfoFLFrameRateMain );
		
		pDebugInfoFLFrameRateRender.TakeOver( new deoglDebugInformation( "FPS Render", colorText, colorBgSub ) );
		pDebugInfoFrameLimiter->GetChildren().Add( pDebugInfoFLFrameRateRender );
	
	// below depends on capabilities being known
	pPipelineManager->GetState().Reset();
	
	pChoices = new deoglRTChoices( *this );
	
	pBufferObject = new deoglRTBufferObject( *this );
	pBufferObject->Init();
	
	pTriangleSorter = new deoglTriangleSorter;
	pOcclusionTestPool = new deoglOcclusionTestPool( *this );
	pPersistentRenderTaskPool = new deoglPersistentRenderTaskPool;
	pShadowMapper = new deoglShadowMapper( *this );
	pDeferredRendering = new deoglDeferredRendering( *this );
	pEnvMapSlotManager = new deoglEnvMapSlotManager( *this );
	
	pOccQueryMgr = new deoglOcclusionQueryManager( *this );
	pLightBoundarybox = new deoglLightBoundaryMap( *this,
		deoglShadowMapper::ShadowMapSize( pConfiguration ) >> 1 );
	
	pRenderers = new deoglRTRenderers( *this );
	pGI = new deoglGI( *this );
	pDefaultTextures = new deoglRTDefaultTextures( *this );
	
	// load vulkan and create device if supported
	try{
		pVulkan.TakeOver( new deSharedVulkan( pOgl ) );
		pVulkanDevice = pVulkan->GetInstance().CreateDeviceHeadlessGraphic( 0 );
		// pVulkanDevice = pVulkan->GetInstance().CreateDeviceHeadlessComputeOnly( 0 );
		
	}catch( const deException &e ){
		pLogger->LogException( e );
		pVulkanDevice = nullptr;
		pVulkan = nullptr;
	}
	
#ifdef DO_VULKAN_TEST
	if( pVulkan ){
		#define VKTLOG(cmd,s) timer.Reset(); cmd; pLogger->LogInfoFormat("Vulkan Test: " s " %dys", (int)(timer.GetElapsedTime()*1e6));
		devkQueue &queue = pVulkanDevice->GetComputeQueue();
		const devkCommandPool::Ref commandPool( queue.CreateCommandPool() );
		const int inputDataCount = 32;
		devkBuffer::Ref bufferInput( devkBuffer::Ref::New(
			new devkBuffer( pVulkanDevice, sizeof( uint32_t ) * inputDataCount, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT ) ) );
		int i;
		uint32_t bufferInputData[ inputDataCount ];
		for( i=0; i<inputDataCount; i++ ){
			bufferInputData[ i ] = i;
		}
		decTimer timer;
		VKTLOG( bufferInput->SetData( bufferInputData ), "Buffer SetData")
		VKTLOG( bufferInput->TransferToDevice( commandPool, queue ), "Buffer TransferToDevice")
		VKTLOG( bufferInput->Wait(), "Buffer Wait")
		
		devkDescriptorSetLayoutConfiguration dslSSBOConfig;
		dslSSBOConfig.SetShaderStageFlags( VK_DESCRIPTOR_TYPE_STORAGE_BUFFER );
		dslSSBOConfig.SetLayoutBindingCount( 1 );
		dslSSBOConfig.SetLayoutBindingAt( 0, 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT );
		
		devkDescriptorSetLayout * const dslSSBO = pVulkanDevice->GetDescriptorSetLayoutManager().GetWith( dslSSBOConfig );
		
		devkDescriptorPool::Ref dpSSBO;
		dpSSBO.TakeOver( new devkDescriptorPool( pVulkanDevice, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, dslSSBO ) );
		
		devkDescriptorSet::Ref dsSSBO;
		VKTLOG( dsSSBO.TakeOver( new devkDescriptorSet( dpSSBO ) ), "DescriptorSet SSBO")
		
		dsSSBO->SetBinding( 0, bufferInput );
		VKTLOG( dsSSBO->Update(), "DescriptorSet SSBO Update" );
		
		const uint32_t test1_spv_data[] = {
			0x07230203,0x00010000,0x0008000a,0x00000027,0x00000000,0x00020011,0x00000001,0x0006000b,
			0x00000002,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
			0x0006000f,0x00000005,0x00000005,0x6e69616d,0x00000000,0x0000000c,0x00060010,0x00000005,
			0x00000011,0x00000001,0x00000001,0x00000001,0x00050007,0x00000001,0x74736574,0x6f632e31,
			0x0000706d,0x008b0003,0x00000002,0x000001c2,0x00000001,0x4f202f2f,0x646f4d70,0x50656c75,
			0x65636f72,0x64657373,0x746e6520,0x702d7972,0x746e696f,0x69616d20,0x2f2f0a6e,0x4d704f20,
			0x6c75646f,0x6f725065,0x73736563,0x64206465,0x6e696665,0x616d2d65,0x206f7263,0x544c554d,
			0x494c5049,0x323d5245,0x202f2f0a,0x6f4d704f,0x656c7564,0x636f7250,0x65737365,0x6c632064,
			0x746e6569,0x6c757620,0x316e616b,0x2f0a3030,0x704f202f,0x75646f4d,0x7250656c,0x7365636f,
			0x20646573,0x67726174,0x652d7465,0x7620766e,0x616b6c75,0x302e316e,0x202f2f0a,0x6f4d704f,
			0x656c7564,0x636f7250,0x65737365,0x6e652064,0x2d797274,0x6e696f70,0x616d2074,0x230a6e69,
			0x656e696c,0x230a3120,0x73726576,0x206e6f69,0x0a303534,0x79616c0a,0x2874756f,0x646e6962,
			0x20676e69,0x2930203d,0x66756220,0x20726566,0x61746144,0x200a7b20,0x69752020,0x7620746e,
			0x65756c61,0x3b5d5b73,0x0a3b7d0a,0x79616c0a,0x2074756f,0x636f6c28,0x735f6c61,0x5f657a69,
			0x203d2078,0x6c202c31,0x6c61636f,0x7a69735f,0x20795f65,0x2c31203d,0x636f6c20,0x735f6c61,
			0x5f657a69,0x203d207a,0x69202931,0x0a0a3b6e,0x6f79616c,0x28207475,0x736e6f63,0x746e6174,
			0x2064695f,0x2930203d,0x6e6f6320,0x75207473,0x20746e69,0x6c615663,0x6f436575,0x20746e75,
			0x3233203d,0x760a0a3b,0x2064696f,0x6e69616d,0x0a7b2928,0x6e6f6309,0x75207473,0x20746e69,
			0x65646e69,0x203d2078,0x475f6c67,0x61626f6c,0x766e496c,0x7461636f,0x496e6f69,0x3b782e44,
			0x6669090a,0x6e692028,0x20786564,0x63203d3e,0x756c6156,0x756f4365,0x2920746e,0x09090a7b,
			0x75746572,0x0a3b6e72,0x090a7d09,0x6176090a,0x7365756c,0x6e69205b,0x20786564,0x3d2a205d,
			0x4c554d20,0x4c504954,0x3b524549,0x000a7d0a,0x00040005,0x00000005,0x6e69616d,0x00000000,
			0x00040005,0x00000009,0x65646e69,0x00000078,0x00080005,0x0000000c,0x475f6c67,0x61626f6c,
			0x766e496c,0x7461636f,0x496e6f69,0x00000044,0x00050005,0x00000012,0x6c615663,0x6f436575,
			0x00746e75,0x00040005,0x00000019,0x61746144,0x00000000,0x00050006,0x00000019,0x00000000,
			0x756c6176,0x00007365,0x00030005,0x0000001b,0x00000000,0x00040047,0x0000000c,0x0000000b,
			0x0000001c,0x00040047,0x00000012,0x00000001,0x00000000,0x00040047,0x00000018,0x00000006,
			0x00000004,0x00050048,0x00000019,0x00000000,0x00000023,0x00000000,0x00030047,0x00000019,
			0x00000003,0x00040047,0x0000001b,0x00000022,0x00000000,0x00040047,0x0000001b,0x00000021,
			0x00000000,0x00040047,0x00000026,0x0000000b,0x00000019,0x00020013,0x00000003,0x00030021,
			0x00000004,0x00000003,0x00040015,0x00000007,0x00000020,0x00000000,0x00040020,0x00000008,
			0x00000007,0x00000007,0x00040017,0x0000000a,0x00000007,0x00000003,0x00040020,0x0000000b,
			0x00000001,0x0000000a,0x0004003b,0x0000000b,0x0000000c,0x00000001,0x0004002b,0x00000007,
			0x0000000d,0x00000000,0x00040020,0x0000000e,0x00000001,0x00000007,0x00040032,0x00000007,
			0x00000012,0x00000020,0x00020014,0x00000013,0x0003001d,0x00000018,0x00000007,0x0003001e,
			0x00000019,0x00000018,0x00040020,0x0000001a,0x00000002,0x00000019,0x0004003b,0x0000001a,
			0x0000001b,0x00000002,0x00040015,0x0000001c,0x00000020,0x00000001,0x0004002b,0x0000001c,
			0x0000001d,0x00000000,0x0004002b,0x00000007,0x0000001f,0x00000002,0x00040020,0x00000020,
			0x00000002,0x00000007,0x0004002b,0x00000007,0x00000025,0x00000001,0x0006002c,0x0000000a,
			0x00000026,0x00000025,0x00000025,0x00000025,0x00050036,0x00000003,0x00000005,0x00000000,
			0x00000004,0x000200f8,0x00000006,0x0004003b,0x00000008,0x00000009,0x00000007,0x00040008,
			0x00000001,0x0000000c,0x00000000,0x00050041,0x0000000e,0x0000000f,0x0000000c,0x0000000d,
			0x0004003d,0x00000007,0x00000010,0x0000000f,0x0003003e,0x00000009,0x00000010,0x00040008,
			0x00000001,0x0000000d,0x00000000,0x0004003d,0x00000007,0x00000011,0x00000009,0x000500ae,
			0x00000013,0x00000014,0x00000011,0x00000012,0x000300f7,0x00000016,0x00000000,0x000400fa,
			0x00000014,0x00000015,0x00000016,0x000200f8,0x00000015,0x00040008,0x00000001,0x0000000e,
			0x00000000,0x000100fd,0x000200f8,0x00000016,0x00040008,0x00000001,0x00000011,0x00000000,
			0x0004003d,0x00000007,0x0000001e,0x00000009,0x00060041,0x00000020,0x00000021,0x0000001b,
			0x0000001d,0x0000001e,0x0004003d,0x00000007,0x00000022,0x00000021,0x00050084,0x00000007,
			0x00000023,0x00000022,0x0000001f,0x00060041,0x00000020,0x00000024,0x0000001b,0x0000001d,
			0x0000001e,0x0003003e,0x00000024,0x00000023,0x000100fd,0x00010038
		};
		decMemoryFile::Ref mfshader( decMemoryFile::Ref::New( new decMemoryFile( "/shaders/vulkantest.spv" ) ) );
		decBaseFileWriter::Ref::New( new decMemoryFileWriter( mfshader, false ) )->Write( test1_spv_data, sizeof( test1_spv_data ) );
		
		devkShaderModule::Ref shader;
		VKTLOG( shader.TakeOver( new devkShaderModule( pVulkanDevice, "/shaders/vulkantest.spv",
			decBaseFileReader::Ref::New( new decMemoryFileReader( mfshader ) ) ) ), "LoadShader");
		
		devkPipelineConfiguration pipelineConfig;
		pipelineConfig.SetDescriptorSetLayout( dslSSBO );
		pipelineConfig.SetType( devkPipelineConfiguration::etCompute );
		pipelineConfig.SetShaderCompute( shader );
		
		const struct ShaderConfig{
			uint32_t valueCount = inputDataCount;
		} shaderConfig;
		
		devkSpecialization::Ref specialization;
		specialization.TakeOver( new devkSpecialization( &shaderConfig, sizeof( shaderConfig ), 1 ) );
		specialization->SetEntryUIntAt( 0, 0, offsetof( ShaderConfig, valueCount ) );
		pipelineConfig.SetSpecialization( specialization );
		
		devkPipeline *pipeline;
		VKTLOG( pipeline = pVulkanDevice->GetPipelineManager().GetWith( pipelineConfig ), "PipelineCompute")
		
		devkCommandBuffer::Ref cmdbuf( commandPool->GetCommandBuffer() );
		VKTLOG( cmdbuf->Begin(), "CmdBuf Begin")
		VKTLOG( cmdbuf->BarrierHostShader( bufferInput, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT ), "CmdBuf BarrierShader")
		VKTLOG( cmdbuf->BindPipeline( *pipeline ), "CmdBuf BindPipeline")
		VKTLOG( cmdbuf->BindDescriptorSet( 0, dsSSBO ), "CmdBuf BindDescriptorSet")
		VKTLOG( cmdbuf->DispatchCompute( shaderConfig.valueCount, 1, 1 ), "CmdBuf DispatchCompute")
		VKTLOG( cmdbuf->BarrierShaderTransfer( bufferInput, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT ), "CmdBuf BarrierHost")
		VKTLOG( cmdbuf->ReadBuffer( bufferInput ), "CmdBuf ReadBuffer")
		VKTLOG( cmdbuf->BarrierTransferHost( bufferInput ), "CmdBuf BarrierTransferHost")
		VKTLOG( cmdbuf->End(), "CmdBuf End")
		VKTLOG( cmdbuf->Submit( queue ), "CmdBuf Submit")
		
		VKTLOG( cmdbuf->Wait(), "CmdBuf Wait")
		
		VKTLOG( bufferInput->GetData( bufferInputData ), "Buffer GetData")
		decString string( "Computed: [" );
		for( i=0; i<inputDataCount; i++ ){
			string.AppendFormat( "%s%d", i > 0 ? ", " : "", bufferInputData[ i ] );
		}
		string.Append( "]" );
		pLogger->LogInfo( string );
		
		// graphic test
		devkRenderPassConfiguration renPassConfig;
		
		renPassConfig.SetAttachmentCount( 1 );
		renPassConfig.SetColorAttachmentAt( 0, VK_FORMAT_R8G8B8A8_UNORM /* VK_FORMAT_R8G8B8_UNORM not working? */,
			devkRenderPassConfiguration::eitClear, devkRenderPassConfiguration::eotReadBack );
		renPassConfig.SetClearValueColorFloatAt( 0, 0, 0, 0, 0 );
		
		renPassConfig.SetSubPassCount( 1 );
		renPassConfig.SetSubPassAt( 0, -1, 0 );
		
		devkRenderPass::Ref renderPass;
		VKTLOG( renderPass.TakeOver( new devkRenderPass( pVulkanDevice, renPassConfig ) ), "Create Render Pass" )
		
		devkImageConfiguration imageConfig;
		imageConfig.Set2D( decPoint( 64, 32 ), VK_FORMAT_R8G8B8A8_UNORM );
		imageConfig.EnableColorAttachment( true );
		imageConfig.EnableTransferSource( true );
		devkImage::Ref image;
		VKTLOG( image.TakeOver( new devkImage( pVulkanDevice, imageConfig ) ), "Create Image" )
		
		devkImageView::Ref imageView;
		VKTLOG( imageView.TakeOver( new devkImageView( image ) ), "Create Image View" )
		
		devkFramebufferConfiguration framebufferConfig;
		framebufferConfig.SetAttachmentCount( 1 );
		framebufferConfig.SetAttachmentAt( 0, imageView );
		framebufferConfig.SetSize( decPoint( 64, 32 ) );
		
		devkFramebuffer::Ref framebuffer;
		VKTLOG( framebuffer.TakeOver( new devkFramebuffer( renderPass, framebufferConfig ) ), "Create Framebuffer" )
		
		const uint32_t test2_vert_spv_data[] = {
			0x07230203,0x00010000,0x0008000a,0x00000022,0x00000000,0x00020011,0x00000001,0x0006000b,
			0x00000002,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
			0x000a000f,0x00000000,0x00000005,0x6e69616d,0x00000000,0x0000000e,0x00000013,0x0000001d,
			0x0000001e,0x00000021,0x00050007,0x00000001,0x74736574,0x65762e32,0x00007472,0x00a20003,
			0x00000002,0x000001c2,0x00000001,0x4f202f2f,0x646f4d70,0x50656c75,0x65636f72,0x64657373,
			0x746e6520,0x702d7972,0x746e696f,0x69616d20,0x2f2f0a6e,0x4d704f20,0x6c75646f,0x6f725065,
			0x73736563,0x64206465,0x6e696665,0x616d2d65,0x206f7263,0x544c554d,0x494c5049,0x323d5245,
			0x202f2f0a,0x6f4d704f,0x656c7564,0x636f7250,0x65737365,0x6c632064,0x746e6569,0x6c757620,
			0x316e616b,0x2f0a3030,0x704f202f,0x75646f4d,0x7250656c,0x7365636f,0x20646573,0x67726174,
			0x652d7465,0x7620766e,0x616b6c75,0x302e316e,0x202f2f0a,0x6f4d704f,0x656c7564,0x636f7250,
			0x65737365,0x6e652064,0x2d797274,0x6e696f70,0x616d2074,0x230a6e69,0x656e696c,0x230a3120,
			0x73726576,0x206e6f69,0x0a303534,0x79616c0a,0x2074756f,0x636f6c28,0x6f697461,0x203d206e,
			0x69202930,0x6576206e,0x69203363,0x736f506e,0x6f697469,0x6c0a3b6e,0x756f7961,0x6c282074,
			0x7461636f,0x206e6f69,0x2931203d,0x206e6920,0x33636576,0x436e6920,0x726f6c6f,0x616c0a3b,
			0x74756f79,0x6f6c2820,0x69746163,0x3d206e6f,0x20293220,0x76206e69,0x20346365,0x61506e69,
			0x64656b63,0x6c0a0a3b,0x756f7961,0x6c282074,0x7461636f,0x206e6f69,0x2930203d,0x74756f20,
			0x63657620,0x43762033,0x726f6c6f,0x760a0a3b,0x2064696f,0x6e69616d,0x0a7b2928,0x0a2a2f09,
			0x6e6f6309,0x76207473,0x20336365,0x69736f70,0x6e6f6974,0x33205b73,0x3d205d20,0x63657620,
			0x33205b33,0x0a285d20,0x65760909,0x20283363,0x31202c31,0x2030202c,0x090a2c29,0x63657609,
			0x2d202833,0x31202c31,0x2030202c,0x090a2c29,0x63657609,0x30202833,0x312d202c,0x2030202c,
			0x29090a29,0x0a090a3b,0x5f6c6709,0x69736f50,0x6e6f6974,0x76203d20,0x28346365,0x736f7020,
			0x6f697469,0x205b736e,0x565f6c67,0x65747265,0x646e4978,0x5d207865,0x2031202c,0x090a3b29,
			0x090a2f2a,0x6c67090a,0x736f505f,0x6f697469,0x203d206e,0x34636576,0x6e692028,0x69736f50,
			0x6e6f6974,0x2031202c,0x090a3b29,0x6c6f4376,0x3d20726f,0x436e6920,0x726f6c6f,0x0a7d0a3b,
			0x00000000,0x00040005,0x00000005,0x6e69616d,0x00000000,0x00060005,0x0000000c,0x505f6c67,
			0x65567265,0x78657472,0x00000000,0x00060006,0x0000000c,0x00000000,0x505f6c67,0x7469736f,
			0x006e6f69,0x00070006,0x0000000c,0x00000001,0x505f6c67,0x746e696f,0x657a6953,0x00000000,
			0x00070006,0x0000000c,0x00000002,0x435f6c67,0x4470696c,0x61747369,0x0065636e,0x00070006,
			0x0000000c,0x00000003,0x435f6c67,0x446c6c75,0x61747369,0x0065636e,0x00030005,0x0000000e,
			0x00000000,0x00050005,0x00000013,0x6f506e69,0x69746973,0x00006e6f,0x00040005,0x0000001d,
			0x6c6f4376,0x0000726f,0x00040005,0x0000001e,0x6f436e69,0x00726f6c,0x00050005,0x00000021,
			0x61506e69,0x64656b63,0x00000000,0x00050048,0x0000000c,0x00000000,0x0000000b,0x00000000,
			0x00050048,0x0000000c,0x00000001,0x0000000b,0x00000001,0x00050048,0x0000000c,0x00000002,
			0x0000000b,0x00000003,0x00050048,0x0000000c,0x00000003,0x0000000b,0x00000004,0x00030047,
			0x0000000c,0x00000002,0x00040047,0x00000013,0x0000001e,0x00000000,0x00040047,0x0000001d,
			0x0000001e,0x00000000,0x00040047,0x0000001e,0x0000001e,0x00000001,0x00040047,0x00000021,
			0x0000001e,0x00000002,0x00020013,0x00000003,0x00030021,0x00000004,0x00000003,0x00030016,
			0x00000007,0x00000020,0x00040017,0x00000008,0x00000007,0x00000004,0x00040015,0x00000009,
			0x00000020,0x00000000,0x0004002b,0x00000009,0x0000000a,0x00000001,0x0004001c,0x0000000b,
			0x00000007,0x0000000a,0x0006001e,0x0000000c,0x00000008,0x00000007,0x0000000b,0x0000000b,
			0x00040020,0x0000000d,0x00000003,0x0000000c,0x0004003b,0x0000000d,0x0000000e,0x00000003,
			0x00040015,0x0000000f,0x00000020,0x00000001,0x0004002b,0x0000000f,0x00000010,0x00000000,
			0x00040017,0x00000011,0x00000007,0x00000003,0x00040020,0x00000012,0x00000001,0x00000011,
			0x0004003b,0x00000012,0x00000013,0x00000001,0x0004002b,0x00000007,0x00000015,0x3f800000,
			0x00040020,0x0000001a,0x00000003,0x00000008,0x00040020,0x0000001c,0x00000003,0x00000011,
			0x0004003b,0x0000001c,0x0000001d,0x00000003,0x0004003b,0x00000012,0x0000001e,0x00000001,
			0x00040020,0x00000020,0x00000001,0x00000008,0x0004003b,0x00000020,0x00000021,0x00000001,
			0x00050036,0x00000003,0x00000005,0x00000000,0x00000004,0x000200f8,0x00000006,0x00040008,
			0x00000001,0x00000014,0x00000000,0x0004003d,0x00000011,0x00000014,0x00000013,0x00050051,
			0x00000007,0x00000016,0x00000014,0x00000000,0x00050051,0x00000007,0x00000017,0x00000014,
			0x00000001,0x00050051,0x00000007,0x00000018,0x00000014,0x00000002,0x00070050,0x00000008,
			0x00000019,0x00000016,0x00000017,0x00000018,0x00000015,0x00050041,0x0000001a,0x0000001b,
			0x0000000e,0x00000010,0x0003003e,0x0000001b,0x00000019,0x00040008,0x00000001,0x00000015,
			0x00000000,0x0004003d,0x00000011,0x0000001f,0x0000001e,0x0003003e,0x0000001d,0x0000001f,
			0x000100fd,0x00010038
		};
		
		mfshader.TakeOver( decMemoryFile::Ref::New( new decMemoryFile( "/shaders/vulkantest2_vert.spv" ) ) );
		decBaseFileWriter::Ref::New( new decMemoryFileWriter( mfshader, false ) )->Write( test2_vert_spv_data, sizeof( test2_vert_spv_data ) );
		
		devkShaderModule::Ref shaderVert;
		VKTLOG( shaderVert.TakeOver( new devkShaderModule( pVulkanDevice, "/shaders/vulkantest2_vert.spv",
			decBaseFileReader::Ref::New( new decMemoryFileReader( mfshader ) ) ) ), "LoadShader");
		
		const uint32_t test2_frag_spv_data[] = {
			0x07230203,0x00010000,0x0008000a,0x00000014,0x00000000,0x00020011,0x00000001,0x0006000b,
			0x00000002,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
			0x0007000f,0x00000004,0x00000005,0x6e69616d,0x00000000,0x0000000a,0x0000000d,0x00030010,
			0x00000005,0x00000007,0x00050007,0x00000001,0x74736574,0x72662e32,0x00006761,0x00840003,
			0x00000002,0x000001c2,0x00000001,0x4f202f2f,0x646f4d70,0x50656c75,0x65636f72,0x64657373,
			0x746e6520,0x702d7972,0x746e696f,0x69616d20,0x2f2f0a6e,0x4d704f20,0x6c75646f,0x6f725065,
			0x73736563,0x64206465,0x6e696665,0x616d2d65,0x206f7263,0x544c554d,0x494c5049,0x323d5245,
			0x202f2f0a,0x6f4d704f,0x656c7564,0x636f7250,0x65737365,0x6c632064,0x746e6569,0x6c757620,
			0x316e616b,0x2f0a3030,0x704f202f,0x75646f4d,0x7250656c,0x7365636f,0x20646573,0x67726174,
			0x652d7465,0x7620766e,0x616b6c75,0x302e316e,0x202f2f0a,0x6f4d704f,0x656c7564,0x636f7250,
			0x65737365,0x6e652064,0x2d797274,0x6e696f70,0x616d2074,0x230a6e69,0x656e696c,0x230a3120,
			0x73726576,0x206e6f69,0x0a303534,0x79616c0a,0x2074756f,0x636f6c28,0x6f697461,0x203d206e,
			0x69202930,0x6576206e,0x76203363,0x6f6c6f43,0x0a0a3b72,0x6f79616c,0x28207475,0x61636f6c,
			0x6e6f6974,0x30203d20,0x756f2029,0x65762074,0x6f203463,0x6f437475,0x3b726f6c,0x6f760a0a,
			0x6d206469,0x286e6961,0x0a7b2029,0x6f2f2f09,0x6f437475,0x20726f6c,0x6576203d,0x20283463,
			0x465f6c67,0x43676172,0x64726f6f,0x2f20782e,0x2c333620,0x5f6c6720,0x67617246,0x726f6f43,
			0x20792e64,0x3133202f,0x2c30202c,0x29203120,0x2f2f0a3b,0x756f0920,0x6c6f4374,0x3d20726f,
			0x63657620,0x67202834,0x72465f6c,0x6f436761,0x2e64726f,0x202f2079,0x202c3133,0x30202c30,
			0x2031202c,0x2f0a3b29,0x6f09202f,0x6f437475,0x20726f6c,0x6576203d,0x31283463,0x2c30202c,
			0x202c3020,0x0a3b2931,0x74756f09,0x6f6c6f43,0x203d2072,0x34636576,0x43762028,0x726f6c6f,
			0x2031202c,0x7d0a3b29,0x0000000a,0x00040005,0x00000005,0x6e69616d,0x00000000,0x00050005,
			0x0000000a,0x4374756f,0x726f6c6f,0x00000000,0x00040005,0x0000000d,0x6c6f4376,0x0000726f,
			0x00040047,0x0000000a,0x0000001e,0x00000000,0x00040047,0x0000000d,0x0000001e,0x00000000,
			0x00020013,0x00000003,0x00030021,0x00000004,0x00000003,0x00030016,0x00000007,0x00000020,
			0x00040017,0x00000008,0x00000007,0x00000004,0x00040020,0x00000009,0x00000003,0x00000008,
			0x0004003b,0x00000009,0x0000000a,0x00000003,0x00040017,0x0000000b,0x00000007,0x00000003,
			0x00040020,0x0000000c,0x00000001,0x0000000b,0x0004003b,0x0000000c,0x0000000d,0x00000001,
			0x0004002b,0x00000007,0x0000000f,0x3f800000,0x00050036,0x00000003,0x00000005,0x00000000,
			0x00000004,0x000200f8,0x00000006,0x00040008,0x00000001,0x0000000b,0x00000000,0x0004003d,
			0x0000000b,0x0000000e,0x0000000d,0x00050051,0x00000007,0x00000010,0x0000000e,0x00000000,
			0x00050051,0x00000007,0x00000011,0x0000000e,0x00000001,0x00050051,0x00000007,0x00000012,
			0x0000000e,0x00000002,0x00070050,0x00000008,0x00000013,0x00000010,0x00000011,0x00000012,
			0x0000000f,0x0003003e,0x0000000a,0x00000013,0x000100fd,0x00010038
		};
		
		mfshader.TakeOver( decMemoryFile::Ref::New( new decMemoryFile( "/shaders/vulkantest2_frag.spv" ) ) );
		decBaseFileWriter::Ref::New( new decMemoryFileWriter( mfshader, false ) )->Write( test2_frag_spv_data, sizeof( test2_frag_spv_data ) );
		
		devkShaderModule::Ref shaderFrag;
		VKTLOG( shaderFrag.TakeOver( new devkShaderModule( pVulkanDevice, "/shaders/vulkantest2_frag.spv",
			decBaseFileReader::Ref::New( new decMemoryFileReader( mfshader ) ) ) ), "LoadShader");
		
		struct sVertex{
			struct{
				float x, y, z;
			} position;
			struct{
				float r, g, b;
			} color;
			uint32_t packed;
		};
		const sVertex vertices[ 3 ] = {
			{ {  1,  1, 0 }, { 1, 0, 0 }, 0 },
			{ { -1,  1, 0 }, { 1, 0, 0 }, 0 },
			{ {  0, -1, 0 }, { 0, 0, 0 }, 0 }
		};
		
		bufferInput.TakeOver( devkBuffer::Ref::New( new devkBuffer( pVulkanDevice,
			sizeof( vertices ), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT ) ) );
		VKTLOG( bufferInput->SetData( vertices ), "Buffer SetData" )
		VKTLOG( bufferInput->TransferToDevice( commandPool, queue ), "Buffer TransferToDevice" )
		VKTLOG( bufferInput->Wait(), "Buffer Wait" )
		
		pipelineConfig = devkPipelineConfiguration();
		pipelineConfig.SetType( devkPipelineConfiguration::etGraphics );
		pipelineConfig.SetRenderPass( renderPass );
		pipelineConfig.SetShaderVertex( shaderVert );
		pipelineConfig.SetShaderFragment( shaderFrag );
		
		pipelineConfig.SetBindingCount( 1 );
		pipelineConfig.SetBindingAt( 0, 0, sizeof( sVertex ) );
		
		pipelineConfig.SetAttributeCount( 3 );
		pipelineConfig.SetAttributeAt( 0, 0, 0, devkPipelineConfiguration::eafFloat3, offsetof( sVertex, position ) );
		pipelineConfig.SetAttributeAt( 1, 1, 0, devkPipelineConfiguration::eafFloat3, offsetof( sVertex, color ) );
		pipelineConfig.SetAttributeAt( 2, 2, 0, devkPipelineConfiguration::eafUIntNormA8B8G8R8, offsetof( sVertex, packed ) );
		
		VKTLOG( pipeline = pVulkanDevice->GetPipelineManager().GetWith( pipelineConfig ), "PipelineGraphic" )
		
		VKTLOG( cmdbuf->Begin(), "CmdBuf Begin" )
		VKTLOG( cmdbuf->BarrierHostShader( bufferInput, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT ), "CmdBuf BarrierShader" )
		VKTLOG( cmdbuf->BeginRenderPass( renderPass, framebuffer ), "CmdBuf BeginRenderPass" )
		VKTLOG( cmdbuf->BindPipeline( *pipeline ), "CmdBuf BindPipeline" )
		VKTLOG( cmdbuf->BindVertexBuffer( 0, bufferInput ), "CmdBuf BindVertexBuffer" )
		VKTLOG( cmdbuf->Draw( 3, 1 ), "CmdBuf Draw" )
		VKTLOG( cmdbuf->EndRenderPass(), "CmdBuf EndRenderPass" )
		VKTLOG( cmdbuf->BarrierShaderTransfer( image, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT ), "CmdBuf BarrierHost" )
		VKTLOG( cmdbuf->ReadImage( image ), "CmdBuf ReadImage" )
		VKTLOG( cmdbuf->BarrierTransferHost( image ), "CmdBuf BarrierTransferHost" )
		VKTLOG( cmdbuf->End(), "CmdBuf End" )
		VKTLOG( cmdbuf->Submit( queue ), "CmdBuf Submit" )
		
		VKTLOG( cmdbuf->Wait(), "CmdBuf Wait" )
		
		oglRGBA imgdata[ 64 * 32 ];
		VKTLOG( image->GetData( imgdata ), "Image GetData" )
		int x, y;
		for( y=0; y<32; y++ ){
			decString string( "Drawn: [" );
			for( x=0; x<64; x++ ){
				string.AppendCharacter( 'a' + ( int )imgdata[ 64 * y + x ].r * 25 / 255 );
			}
			string.Append( "]" );
			pLogger->LogInfo( string );
		}
	}
#endif
	
	// initial notification. required for testing if devode config is hardcoded in constructor
	DevModeDebugInfoChanged();
	
	// some final preparations. is this really required?
	pTexture->GetStages().DisableAllStages();
	pPipelineManager->GetState().Reset();
	
	#ifdef OS_ANDROID
	DevModeDebugInfoChanged(); // to enable debug stuff if enabled
	#endif
}

void deoglRenderThread::pInitExtensions(){
	const deoglConfiguration &configuration = pOgl.GetConfiguration();
	
	// load extensions
	pExtensions = new deoglExtensions( *this );
	pExtensions->Initialize();
	
	//if( configuration.GetDoLogDebug() ){
		pExtensions->PrintSummary(); // print this always to help debug customer problems
	//}
	
	if( ! pExtensions->VerifyPresence() ){
		DETHROW_INFO( deeInvalidAction,
			"Required extensions or functions could not be found. Please see logs" );
	}
	
	// enable states never touched again later (and that can not be changed)
	if( pExtensions->GetHasSeamlessCubeMap() ){
		// on nVidia this crashes due to a bug with linear filtering on float textures
		// TODO is this still affecting the newer driver versions?
		if( configuration.GetDisableCubeMapLinearFiltering() ){
			OGL_CHECK( *this, glDisable( GL_TEXTURE_CUBE_MAP_SEAMLESS ) );
			
		}else{
			OGL_CHECK( *this, glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS ) );
		}
	}
}

void deoglRenderThread::pInitCapabilities(){
	// query hardware information
	pCapabilities = new deoglCapabilities( *this );
	pCapabilities->DetectCapabilities();
	
	#ifdef ANDROID
	pLogger->LogInfo( "Hardware information:" );
	
	int i, j;
	decString text;
	GLint resultsInt[ 2 ];
	const int capabilityCount = 18;
	const struct sCapability{
		GLint id;
		const char *name;
		int valueCount;
	} capabilities[ capabilityCount ] = {
		{ GL_MAX_TEXTURE_SIZE, "GL: Maximum texture size", 1 },
		{ GL_MAX_VIEWPORT_DIMS, "GL: Maximum viewport size", 2 },
		
		{ GL_MAX_TEXTURE_IMAGE_UNITS, "Shaders: Maximum number of texture image units", 1 },
		{ GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, "Shaders: Maximum number of combined texture image units", 1 },
		
		{ GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, "Vertex Shader: Maximum number of texture image units", 1 },
		{ GL_MAX_VERTEX_ATTRIBS, "Vertex Shader: Maximum number of vertex attributes", 1 },
		{ GL_MAX_VARYING_FLOATS, "Vertex Shader: Maximum number of varying floats", 1 },
		{ GL_MAX_VERTEX_UNIFORM_COMPONENTS, "Vertex Shader: Maximum number of vertex uniform components", 1 },
		
		{ GL_MAX_ARRAY_TEXTURE_LAYERS, "Texture Array: Maximum number of texture layers", 1 },
		
		{ GL_MAX_VERTEX_UNIFORM_BLOCKS, "Uniform Buffer Object: Maximum vertex uniform blocks", 1 },
		{ GL_MAX_FRAGMENT_UNIFORM_BLOCKS, "Uniform Buffer Object: Maximum fragment uniform blocks", 1 },
		{ GL_MAX_COMBINED_UNIFORM_BLOCKS, "Uniform Buffer Object: Maximum combined uniform blocks", 1 },
		{ GL_MAX_UNIFORM_BUFFER_BINDINGS, "Uniform Buffer Object: Maximum bindings", 1 },
		{ GL_MAX_UNIFORM_BLOCK_SIZE, "Uniform Buffer Object: Maximum block size", 1 },
		{ GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS, "Uniform Buffer Object: Maximum vertex components", 1 },
		{ GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS, "Uniform Buffer Object: Maximum fragment components", 1 },
		
		{ GL_MAX_DRAW_BUFFERS, "Maximum number of draw buffers", 1 },
		{ GL_MAX_COLOR_ATTACHMENTS, "Maximum color attachments", 1 }
	};
	/* not supported by android
	GL_MAX_TEXTURE_COORDS
	GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS
	GL_MAX_GEOMETRY_VARYING_COMPONENTS
	GL_MAX_VERTEX_VARYING_COMPONENTS
	GL_MAX_GEOMETRY_UNIFORM_COMPONENTS
	GL_MAX_GEOMETRY_OUTPUT_VERTICES
	GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS
	GL_MAX_GEOMETRY_UNIFORM_BLOCKS
	GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS
	*/
	
	for( i=0; i<capabilityCount; i++ ){
		OGL_CHECK( *this, glGetIntegerv( capabilities[ i ].id, &resultsInt[ 0 ] ) );
		
		text.Format( "- %s:", capabilities[ i ].name );
		for( j=0; j<capabilities[ i ].valueCount; j++ ){
			text.AppendFormat( " %i", resultsInt[ j ] );
		}
		pLogger->LogInfo( text );
	}
	#endif
}



void deoglRenderThread::pRenderSingleFrame(){
	const deoglDebugTraceGroup debugTrace( *this, "RenderSingleFrame" );
	if( pConfigChanged ){
		pConfigChanged = false;
		
		pGI->GetTraceRays().UpdateFromConfig();
	}
	
	//const deoglDeveloperMode &devmode = pDebug->GetDeveloperMode();
	const bool showDebugInfoModule = pDebugInfoModule->GetVisible();
	
	if( pDebugInfoFrameLimiter->GetVisible() ){
		pDebugInfoFLEstimRender->Clear();
		pDebugInfoFLFrameRateRender->Clear();
		
		if( pTimeHistoryRender.HasMetrics() ){
			pDebugInfoFLEstimRender->IncrementElapsedTime( pTimeHistoryRender.GetAverage() );
			pDebugInfoFLFrameRateRender->IncrementCounter( ( int )( 1.0f /
				decMath::max( pTimeHistoryRender.GetAverage(), pFrameTimeLimit ) ) );
		}
	}
	
	if( showDebugInfoModule ){
		pDebugTimeThreadRenderWindowsPrepare = 0.0f;
		pDebugTimeThreadRenderWindowsRender = 0.0f;
		pDebugCountThreadWindows = 0;
		
		pRenderers->GetCanvas().DebugInfoCanvasReset();
		
		pDebugTimerRenderThread1.Reset();
		pDebugTimerRenderThread2.Reset();
	}
#ifdef OS_ANDROID
	DebugMemoryUsage( "deoglRenderThread::pRenderSingleFrame IN" );
	decTimer timer;
#endif
	
	// synchronize with GPU. it is annoying this has to be required since it prevents fullly
	// occupying the GPU. the problem is that swap buller stalls GPU processing and thus
	// GPU->CPU transfer. by synchronizing with the GPU we can ensure all rendering has
	// finished so we can use the GPU for pre-render processing. this trades stalling at
	// an unfortunate time with stalling at a well known time
	OGL_CHECK( *this, glFinish() );
	
	if( showDebugInfoModule ){
		pDebugTimeThreadRenderSyncGpu = pDebugTimerRenderThread2.GetElapsedTime();
	}
	
	// NOTE if there are multiple windows we can not use delay swap buffers as we
	//      can using a single window. we have to go through each window individually
	//      or using a render thread for each of them which would be a problem. this
	//      scenario though happens only on applications like the game editor
	if( pRRenderWindowList.GetCount() > 1 ){
		pBeginFrame();
		if( showDebugInfoModule ){
			pDebugTimeThreadRenderSwap = 0.0f;
			pDebugTimeThreadRenderWindows = 0.0f;
			pDebugTimeThreadRenderBegin = pDebugTimerRenderThread2.GetElapsedTime();
			pDebugTimerRenderThread3.Reset();
		}
		
		pVRRender();
		const int count = pRRenderWindowList.GetCount();
		int i;
		for( i=0; i<count; i++ ){
			deoglRRenderWindow &window = *( ( deoglRRenderWindow* )pRRenderWindowList.GetAt( i ) );
			window.Render();
			if( showDebugInfoModule ){
				pDebugTimeThreadRenderWindows += pDebugTimerRenderThread2.GetElapsedTime();
			}
			
			window.SwapBuffers();
			if( showDebugInfoModule ){
				pDebugTimeThreadRenderSwap += pDebugTimerRenderThread2.GetElapsedTime();
			}
		}
		
		pCaptureCanvas();
		if( showDebugInfoModule ){
			pDebugTimeThreadRenderCapture = pDebugTimerRenderThread2.GetElapsedTime();
		}
		
		pVRSubmit();
		pEndFrame();
		
	}else{
		// pSwapBuffers();
		// if( showDebugInfoModule ){
		// 	pDebugTimeThreadRenderSwap = pDebugTimerRenderThread2.GetElapsedTime();
		// }
		
		#ifdef OS_ANDROID
		if( DoesDebugMemoryUsage() ) pLogger->LogInfo("pRenderSingleFrame ENTER");
		#endif
		pBeginFrame();
		if( showDebugInfoModule ){
			pDebugTimeThreadRenderBegin = pDebugTimerRenderThread2.GetElapsedTime();
			pDebugTimerRenderThread3.Reset();
		}
		#ifdef OS_ANDROID
		if( DoesDebugMemoryUsage() ) pLogger->LogInfo("pRenderSingleFrame BeginFrame");
		#endif
		
		pVRRender();
		pRenderWindows();
		if( showDebugInfoModule ){
			pDebugTimeThreadRenderWindows = pDebugTimerRenderThread2.GetElapsedTime();
		}
		#ifdef OS_ANDROID
		if( DoesDebugMemoryUsage() ) pLogger->LogInfo("pRenderSingleFrame RenderWindows");
		#endif
		
		pCaptureCanvas();
		if( showDebugInfoModule ){
			pDebugTimeThreadRenderCapture = pDebugTimerRenderThread2.GetElapsedTime();
		}
		#ifdef OS_ANDROID
		if( DoesDebugMemoryUsage() ) pLogger->LogInfo("pRenderSingleFrame CaptureCanvas");
		#endif
		
		pVRSubmit();
		pEndFrame();
		
		// the placement of swaping buffers is a problem. if done right before rendering
		// the swaping stalls compute shaders using read back. if done here swaping stalls
		// due to calling glFlush internally. the delay here is though better than the one
		// at the start of rendering since after this swap buffer call the render thread
		// waits on a barrier
		pSwapBuffers();
		if( showDebugInfoModule ){
			pDebugTimeThreadRenderSwap = pDebugTimerRenderThread2.GetElapsedTime();
		}
	}
	
	if( showDebugInfoModule ){
		const deoglDebugTraceGroup debugTraceDI( *this, "DebugInfo" );
		const float time2 = pDebugTimerRenderThread2.GetElapsedTime();
		const float time1 = pDebugTimerRenderThread1.GetElapsedTime();
		
		pDebugInfoThreadRenderSyncGpu->Clear();
		pDebugInfoThreadRenderSyncGpu->IncrementElapsedTime( pDebugTimeThreadRenderSyncGpu );
		
		pDebugInfoThreadRenderBegin->Clear();
		pDebugInfoThreadRenderBegin->IncrementElapsedTime( pDebugTimeThreadRenderBegin );
		
		pDebugInfoThreadRenderWindows->Clear();
		pDebugInfoThreadRenderWindows->IncrementElapsedTime( pDebugTimeThreadRenderWindows );
		
		pDebugInfoThreadRenderWindowsPrepare->Clear();
		pDebugInfoThreadRenderWindowsPrepare->IncrementElapsedTime( pDebugTimeThreadRenderWindowsPrepare );
		pDebugInfoThreadRenderWindowsPrepare->IncrementCounter( pDebugCountThreadWindows );
		
		pDebugInfoThreadRenderWindowsRender->Clear();
		pDebugInfoThreadRenderWindowsRender->IncrementElapsedTime( pDebugTimeThreadRenderWindowsRender );
		pDebugInfoThreadRenderWindowsRender->IncrementCounter( pDebugCountThreadWindows );
		
		pDebugInfoThreadRenderCapture->Clear();
		pDebugInfoThreadRenderCapture->IncrementElapsedTime( pDebugTimeThreadRenderCapture );
		
		pDebugInfoThreadRenderEnd->Clear();
		pDebugInfoThreadRenderEnd->IncrementElapsedTime( time2 );
		
		pDebugInfoThreadRenderSwap->Clear();
		pDebugInfoThreadRenderSwap->IncrementElapsedTime( pDebugTimeThreadRenderSwap );
		
		pDebugInfoThreadRender->Clear();
		pDebugInfoThreadRender->IncrementElapsedTime( time1 );
		
		pRenderers->GetCanvas().DebugInfoCanvasUpdate();
	}
	
#ifdef OS_ANDROID
	pLogger->LogInfoFormat( "pRenderSingleFrame: %dms", (int )(timer.GetElapsedTime() * 1000.0f) );
	DebugMemoryUsage( "deoglRenderThread::pRenderSingleFrame OUT" );
#endif
}

#ifdef OS_ANDROID
#include "../texture/texture2d/deoglRenderableColorTextureManager.h"
#include "../texture/texture2d/deoglRenderableDepthTextureManager.h"
#include "../texture/arraytexture/deoglRenderableArrayTextureManager.h"
#include "../model/deoglModel.h"
#include "../model/deoglRModel.h"
#include "../model/deoglModelLOD.h"
#include "../model/face/deoglModelFace.h"
#include "../component/deoglComponent.h"
#include "../component/deoglRComponent.h"
#include "../component/deoglRComponentLOD.h"
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentManager.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelFace.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/model/deModelTextureCoordinatesSet.h>
#include <dragengine/resources/model/deModelWeight.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/video/deVideo.h>
#include <dragengine/resources/video/deVideoManager.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileReader.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <stdio.h>

bool deoglRenderThread::DoesDebugMemoryUsage() const{
	if( ! pLogger ) return false;
	if( ! pTexture ) return false;
	if( ! pDeferredRendering ) return false;
	
	return pOgl.GetGameEngine()->GetCacheAppID() == "testing";
}

void deoglRenderThread::DebugMemoryUsage( const char *prefix ){
	if( ! DoesDebugMemoryUsage() ) return;
	
	pLogger->LogInfoFormat( "DebugMemoryUsage: %s", prefix );
	
	const char * const fmtTex2D  = "Tex2D (%4i): %4uM %4uM(%2i%%) %3uM | C(%4i) %4uM %4uM(%2i%%) %3uM | D(%3i) %3uM";
	const char * const fmtTexArr = "TexArr(%4i): %4uM %4uM(%2i%%) %3uM | C(%4i) %4uM %4uM(%2i%%) %3uM | D(%3i) %3uM";
	const char * const fmtCube   = "Cube  (%4i): %4uM %4uM(%2i%%) %3uM | C(%4i) %4uM %4uM(%2i%%) %3uM | D(%3i) %3uM";
	const char * const fmtRenBuf = "RenBuf(%4i): %4uM                 | C(%4i) %4uM                 | D(%3i) %3uM";
	const char * const fmtSkin   = "Skins (%4i): %4uM %4uM(%2i%%) %3uM";
	const char * const fmtRender = "Renderables :  2D-C(%2i) %3uM | 2D-D(%2i) %3uM | Arr(%2i) %3uM";
	const char * const fmtVBO    = "VBO   (%4i): %4uM | S(%4i) %4uM | I(%4i) %4uM | T(%4i) %4uM";
	const char * const fmtDefRen = "DefRen      : %3uM | T %3uM | R %3uM";
	
	const deoglMemoryConsumptionTexture &consumptionTexture2D = pMemoryManager.GetConsumption().GetTexture2D();
	const int textureCount = consumptionTexture2D.GetCount();
	const int textureColorCount = consumptionTexture2D.GetColorCount();
	const int textureDepthCount = consumptionTexture2D.GetDepthCount();
	unsigned int textureGPU = consumptionTexture2D.GetGPU();
	unsigned int textureGPUCompressed = consumptionTexture2D.GetGPUCompressed();
	unsigned int textureGPUUncompressed = consumptionTexture2D.GetGPUUncompressed();
	unsigned int textureColorGPU = consumptionTexture2D.GetColorGPU();
	unsigned int textureColorGPUCompressed = consumptionTexture2D.GetColorGPUCompressed();
	unsigned int textureColorGPUUncompressed = consumptionTexture2D.GetColorGPUUncompressed();
	unsigned int textureDepthGPU = consumptionTexture2D.GetDepthGPU();
	double textureRatioCompressed = 0.0f;
	double textureColorRatioCompressed = 0.0f;
	
	if( textureGPU > 0 ){
		textureRatioCompressed = 100.0 * ( ( double )textureGPUCompressed / ( double )textureGPU );
	}
	if( textureColorGPU > 0 ){
		textureColorRatioCompressed = 100.0 * ( ( double )textureColorGPUCompressed / ( double )textureColorGPU );
	}
	
	textureGPU /= 1000000;
	textureGPUCompressed /= 1000000;
	textureGPUUncompressed /= 1000000;
	textureColorGPU /= 1000000;
	textureColorGPUCompressed /= 1000000;
	textureColorGPUUncompressed /= 1000000;
	textureDepthGPU /= 1000000;
	
	pLogger->LogInfoFormat( fmtTex2D, textureCount, textureGPU, textureGPUCompressed, ( int )textureRatioCompressed,
		textureGPUUncompressed, textureColorCount, textureColorGPU, textureColorGPUCompressed,
		( int )textureColorRatioCompressed, textureColorGPUUncompressed, textureDepthCount, textureDepthGPU );
	
	// textures array
	const deoglMemoryConsumptionTexture &consumptionTextureArray = pMemoryManager.GetConsumption().GetTextureArray();
	const int textureArrayCount = consumptionTextureArray.GetCount();
	const int textureArrayColorCount = consumptionTextureArray.GetColorCount();
	const int textureArrayDepthCount = consumptionTextureArray.GetDepthCount();
	unsigned int textureArrayGPU = consumptionTextureArray.GetGPU();
	unsigned int textureArrayGPUCompressed = consumptionTextureArray.GetGPUCompressed();
	unsigned int textureArrayGPUUncompressed = consumptionTextureArray.GetGPUUncompressed();
	unsigned int textureArrayColorGPU = consumptionTextureArray.GetColorGPU();
	unsigned int textureArrayColorGPUCompressed = consumptionTextureArray.GetColorGPUCompressed();
	unsigned int textureArrayColorGPUUncompressed = consumptionTextureArray.GetColorGPUUncompressed();
	unsigned int textureArrayDepthGPU = consumptionTextureArray.GetDepthGPU();
	double textureArrayRatioCompressed = 0.0f;
	double textureArrayColorRatioCompressed = 0.0f;
	
	if( textureArrayGPU > 0 ){
		textureArrayRatioCompressed = 100.0 * ( ( double )textureArrayGPUCompressed / ( double )textureArrayGPU );
	}
	if( textureArrayColorGPU > 0 ){
		textureArrayColorRatioCompressed = 100.0 * ( ( double )textureArrayColorGPUCompressed / ( double )textureArrayColorGPU );
	}
	
	textureArrayGPU /= 1000000;
	textureArrayGPUCompressed /= 1000000;
	textureArrayGPUUncompressed /= 1000000;
	textureArrayColorGPU /= 1000000;
	textureArrayColorGPUCompressed /= 1000000;
	textureArrayColorGPUUncompressed /= 1000000;
	textureArrayDepthGPU /= 1000000;
	
	pLogger->LogInfoFormat( fmtTexArr, textureArrayCount, textureArrayGPU, textureArrayGPUCompressed, ( int )textureArrayRatioCompressed,
		textureArrayGPUUncompressed, textureArrayColorCount, textureArrayColorGPU, textureArrayColorGPUCompressed,
		( int )textureArrayColorRatioCompressed, textureArrayColorGPUUncompressed, textureArrayDepthCount, textureArrayDepthGPU );
	
	// cube maps
	const deoglMemoryConsumptionTexture &consumptionTextureCube = pMemoryManager.GetConsumption().GetTextureCube();
	const int cubemapCount = consumptionTextureCube.GetCount();
	const int cubemapColorCount = consumptionTextureCube.GetColorCount();
	const int cubemapDepthCount = consumptionTextureCube.GetDepthCount();
	unsigned int cubemapGPU = consumptionTextureCube.GetGPU();
	unsigned int cubemapGPUCompressed = consumptionTextureCube.GetGPUCompressed();
	unsigned int cubemapGPUUncompressed = consumptionTextureCube.GetGPUUncompressed();
	unsigned int cubemapColorGPU = consumptionTextureCube.GetColorGPU();
	unsigned int cubemapColorGPUCompressed = consumptionTextureCube.GetColorGPUCompressed();
	unsigned int cubemapColorGPUUncompressed = consumptionTextureCube.GetColorGPUUncompressed();
	unsigned int cubemapDepthGPU = consumptionTextureCube.GetDepthGPU();
	double cubemapRatioCompressed = 0.0f;
	double cubemapColorRatioCompressed = 0.0f;
	
	if( cubemapGPU > 0 ){
		cubemapRatioCompressed = 100.0 * ( ( double )cubemapGPUCompressed / ( double )cubemapGPU );
	}
	if( cubemapColorGPU > 0 ){
		cubemapColorRatioCompressed = 100.0 * ( ( double )cubemapColorGPUCompressed / ( double )cubemapColorGPU );
	}
	
	cubemapGPU /= 1000000;
	cubemapGPUCompressed /= 1000000;
	cubemapGPUUncompressed /= 1000000;
	cubemapColorGPU /= 1000000;
	cubemapColorGPUCompressed /= 1000000;
	cubemapColorGPUUncompressed /= 1000000;
	cubemapDepthGPU /= 1000000;
	
	pLogger->LogInfoFormat( fmtCube, cubemapCount, cubemapGPU, cubemapGPUCompressed, ( int )cubemapRatioCompressed, cubemapGPUUncompressed,
		cubemapColorCount, cubemapColorGPU, cubemapColorGPUCompressed, ( int )cubemapColorRatioCompressed, cubemapColorGPUUncompressed,
		cubemapDepthCount, cubemapDepthGPU );
	
	// skin memory consumption
	const deoglMemoryConsumptionTexture &consumptionSkin = pMemoryManager.GetConsumption().GetSkin();
	const int skinCount = consumptionSkin.GetCount();
	unsigned int skinGPU = consumptionSkin.GetGPU();
	unsigned int skinGPUCompressed = consumptionSkin.GetGPUCompressed();
	unsigned int skinGPUUncompressed = consumptionSkin.GetGPUUncompressed();
	double percentageCompressed = 0.0f;
	
	if( skinGPU > 0 ){
		percentageCompressed = 100.0 * ( ( double )skinGPUCompressed / ( double )skinGPU );
	}
	
	skinGPU /= 1000000;
	skinGPUCompressed /= 1000000;
	skinGPUUncompressed /= 1000000;
	
	pLogger->LogInfoFormat( fmtSkin, skinCount, skinGPU, skinGPUCompressed, ( int )percentageCompressed, skinGPUUncompressed );
	
	// renderable memory consumption
	const int renderable2DColorCount = pTexture->GetRenderableColorTexture().GetTextureCount();
	const int renderable2DDepthCount = pTexture->GetRenderableDepthTexture().GetTextureCount();
	const int renderableArrayCount = pTexture->GetRenderableArrayTexture().GetTextureCount();
	unsigned int renderable2DColorGPU = 0; //memmgr.GetRenderable2DColorGPU();
	unsigned int renderable2DDepthGPU = 0; //memmgr.GetRenderable2DDepthGPU();
	unsigned int renderableArrayGPU = 0; //memmgr.GetRenderableArrayGPU();
	
	renderable2DColorGPU /= 1000000;
	renderable2DDepthGPU /= 1000000;
	renderableArrayGPU /= 1000000;
	
	pLogger->LogInfoFormat( fmtRender, renderable2DColorCount, renderable2DColorGPU, renderable2DDepthCount, renderable2DDepthGPU,
		renderableArrayCount, renderableArrayGPU );
	
	// vbo
	const deoglMemoryConsumptionBufferObject &consumptionVBO = pMemoryManager.GetConsumption().GetVBO();
	const int vboCount = consumptionVBO.GetCount();
	const int vboSharedCount = consumptionVBO.GetSharedCount();
	const int vboIBOCount = consumptionVBO.GetIBOCount();
	const int vboTBOCount = consumptionVBO.GetTBOCount();
	unsigned int vboGPU = consumptionVBO.GetGPU() / 1000000;
	unsigned int vboSharedGPU = consumptionVBO.GetSharedGPU() / 1000000;
	unsigned int vboIBOGPU = consumptionVBO.GetIBOGPU() / 1000000;
	unsigned int vboTBOGPU = consumptionVBO.GetTBOGPU() / 1000000;
	
	pLogger->LogInfoFormat( fmtVBO, vboCount, vboGPU, vboSharedCount, vboSharedGPU, vboIBOCount, vboIBOGPU, vboTBOCount, vboTBOGPU );
	
	// deferred rendering system
	int defrenGPU = pDeferredRendering->GetMemoryUsageGPU() / 1000000;
	int defrenGPUTexture = pDeferredRendering->GetMemoryUsageGPUTexture() / 1000000;
	
	pLogger->LogInfoFormat( fmtDefRen, defrenGPU, defrenGPUTexture, defrenGPURenBuf );
	
	// grand total of all above
	int totalGPU;
	
	totalGPU = consumptionTexture2D.GetGPU();
	totalGPU += consumptionTextureArray.GetGPU();
	totalGPU += consumptionTextureCube.GetGPU();
	totalGPU += consumptionVBO.GetGPU();
	totalGPU /= 1000000;
	
	pLogger->LogInfoFormat( "Total %4iM", totalGPU );
	
	// proc/self/statm info
	char bufstatm[256];
	int filestam = open("/proc/self/statm", O_RDONLY);
	int lenstam = (int)read(filestam, &bufstatm[0], 256);
	bufstatm[lenstam] = '\0';
	close(filestam);
	const decStringList statm(decString(bufstatm).Split(' '));
	pLogger->LogInfoFormat( "TotalProgramSize=%d ResidentSetSize=%d SharedPages%d Text(code)=%d DataStack=%d Library=%d DirtyPages=%d",
		statm.GetAt(0).ToInt(), statm.GetAt(1).ToInt(), statm.GetAt(2).ToInt(), statm.GetAt(3).ToInt(),
		statm.GetAt(4).ToInt(), statm.GetAt(5).ToInt(), statm.GetAt(6).ToInt() );
	
	// malloc info
	const struct mallinfo minfo( mallinfo() );
	//pLogger->LogInfoFormat( "HeapSize=%d HeapAllocSize=%d", minfo.usmblks, minfo.uordblks );
	pLogger->LogInfoFormat( "TotalAllocSpace=%d TotalFreeSpace=%d", minfo.uordblks, minfo.fordblks );
	
	#if 0
	char *minfoString = NULL;
	size_t minfoSize = 0;
	FILE *minfoFile = open_memstream( &minfoString, &minfoSize );
	if( minfoFile ){
		if( malloc_info( 0, minfoFile ) == 0 ){
			decMemoryFile *memFile = new decMemoryFile( "malloc_info" );
			memFile->Resize( minfoSize );
			memcpy( memFile->GetPointer(), minfoString, minfoSize );
			decMemoryFileReader *memFileReader = new decMemoryFileReader( memFile );
			decXmlDocument xmlDoc;
			decXmlParser xmlParser( pLogger );
			xmlParser.ParseXml( memFileReader, &xmlDoc );
			memFileReader->FreeReference();
			memFile->FreeReference();
			long systemCurrent = 0;
			long systemMax = 0;
			int i;
			for( i=0; i<xmlDoc.GetRoot()->GetElementCount(); i++ ){
				const decXmlElement * const element = xmlDoc.GetRoot()->GetElementAt( i );
				if( ! element->CanCastToElementTag() ){
					continue;
				}
				const decXmlElementTag * const tag = element->CastToElementTag():
				if( strcmp( tag->GetName(), "system" ) == 0 ){
					const char *type = NULL;
					long size = 0;
					int j;
					for( j=0; j<tag.GetElementCount(); j++ ){
						const decXmlElement * const element2 = tag->GetElementAt( j );
						if( ! element2->CanCastToAttValue() ){
							continue;
						}
						const decXmlAttValue * const attr = element2->CastToAttValue();
						if( strcmp( attr->GetName(), "type" ) == 0 ){
							type = attr->GetValue();
						}else if( strcmp( attr->GetName(), "size" ) == 0 ){
							size = strtol( attr->GetValue(), NULL, 10 );
						}
					}
					if( strcmp( type, "current" ) == 0 ){
						systemCurrent = size;
					}else if( strcmp( type, "max" ) == 0 ){
						systemMax = size;
					}
				}
			}
		}
		free( minfoString );
	}
	pLogger->LogInfoFormat( "SystemCurrent=%ld SystemMax=%ld", systemCurrent, systemMax );
	#endif
	
	// resource consumption
	deImage *scanImage = pOgl.GetGameEngine()->GetImageManager()->GetRootImage();
	int imageMemUsage = 0;
	int imageCountRetained = 0;
	int imageCountReleased = 0;
	while( scanImage ){
		if( scanImage->GetData() ){
			imageMemUsage += scanImage->GetWidth() * scanImage->GetHeight() * scanImage->GetDepth()
				* scanImage->GetComponentCount() * ( scanImage->GetBitCount() / 8 );
			imageCountRetained++;
		}else{
			imageCountReleased++;
		}
		scanImage = ( deImage* )scanImage->GetLLManagerNext();
	}
	pLogger->LogInfoFormat( "Images: Usage=%dM Retained=%d Released=%d",
		imageMemUsage / 1000000, imageCountRetained, imageCountReleased );
	
	deModel *scanModel = pOgl.GetGameEngine()->GetModelManager()->GetRootModel();
	int modelMemUsage = 0;
	int modelCount = 0;
	while( scanModel ){
		const int lodCount = scanModel->GetLODCount();
		int l;
		for( l=0; l<lodCount; l++ ){
			const deModelLOD &lod = *scanModel->GetLODAt( l );
			modelMemUsage += sizeof( deModelWeight ) * lod.GetWeightCount()
				+ sizeof( int ) * lod.GetWeightGroupCount()
				+ sizeof( deModelVertex ) * lod.GetVertexCount()
				+ sizeof( deModelFace ) * lod.GetFaceCount()
				+ sizeof( decVector2 ) * ( lod.GetTextureCoordinatesSetCount() * lod.GetTextureCoordinatesCount() );
		}
		modelMemUsage += sizeof( decVector ) * scanModel->GetNormalCount() /* deprecated */
			+ sizeof( decVector ) * scanModel->GetTangentCount() /* deprecated */
			+ sizeof( deModel::sHackWeight ) * scanModel->GetHackWeightCount() /* deprecated */ ;
		modelCount++;
		scanModel = ( deModel* )scanModel->GetLLManagerNext();
	}
	pLogger->LogInfoFormat( "Models: Usage=%dM Count=%d", modelMemUsage / 1000000, modelCount );
	
	modelMemUsage = 0;
	modelCount = 0;
	scanModel = pOgl.GetGameEngine()->GetModelManager()->GetRootModel();
	while( scanModel ){
		const deoglRModel &rmodel = *( ( ( deoglModel* )scanModel->GetPeerGraphic() )->GetRModel() );
		const int lodCount = rmodel.GetLODCount();
		int l;
		for( l=0; l<lodCount; l++ ){
			const deoglModelLOD &lod = rmodel.GetLODAt( l );
			modelMemUsage += sizeof( oglModelPosition ) * lod.GetPositionCount()
				+ sizeof( decVector2 ) * lod.GetTextureCoordinatesCount()
				+ sizeof( decVector ) * lod.GetNormalCount()
				+ ( sizeof( decVector ) + sizeof( bool ) ) * lod.GetTangentCount()
				+ sizeof( oglModelWeight * ) * lod.GetWeightsEntryCount()
				+ sizeof( int ) * lod.GetWeightsCount()
				+ sizeof( oglModelVertex ) * lod.GetVertexCount()
				+ sizeof( deoglModelFace ) * lod.GetFaceCount()
				+ ( sizeof( decVector2 ) + sizeof( decVector ) + sizeof( bool ) ) * lod.GetTextureCoordinatesCount();
		}
		modelCount++;
		scanModel = ( deModel* )scanModel->GetLLManagerNext();
	}
	pLogger->LogInfoFormat( "OGL-Models: Usage=%dM Count=%d", modelMemUsage / 1000000, modelCount );
	/*
	deVideo *scanVideo = pOgl.GetGameEngine()->GetVideoManager()->GetRootVideo();
	int videoMemUsage = 0;
	int videoCountRetained = 0;
	int videoCountReleased = 0;
	while( scanVideo ){
		if( scanVideo->GetFrameData() ){
			videoMemUsage += scanVideo->GetWidth() * scanVideo->GetHeight() * 3 * scanVideo->GetFrameCount();
			videoCountRetained++;
		}else{
			videoCountReleased++;
		}
		scanVideo = ( deVideo* )scanVideo->GetLLManagerNext();
	}
	pLogger->LogInfoFormat( "Videos: Usage=%dM Retained=%d Released=%d",
		videoMemUsage / 1000000, videoCountRetained, videoCountReleased );
	*/
	
	deComponent *scanComponent = pOgl.GetGameEngine()->GetComponentManager()->GetRootComponent();
	int componentMemUsage = 0;
	int componentCount = 0;
	while( scanComponent ){
		if( scanComponent->GetModel() ){
			componentMemUsage += sizeof( decVector ) * scanComponent->GetModel()->GetLODAt( 0 )->GetVertexCount()
				+ sizeof( decMatrix ) * scanComponent->GetModel()->GetHackWeightCount();
		}
		componentCount++;
		scanComponent = ( deComponent* )scanComponent->GetLLManagerNext();
	}
	pLogger->LogInfoFormat( "Components: Usage=%dM Bare=%dB Count=%d",
		componentMemUsage / 1000000, sizeof( deComponent ) * componentCount, componentCount );
	
	scanComponent = pOgl.GetGameEngine()->GetComponentManager()->GetRootComponent();
	componentMemUsage = 0;
	int componentBareUsage = 0;
	componentCount = 0;
	while( scanComponent ){
		const deoglRComponent &oglComponent = *( ( ( deoglComponent* )scanComponent->GetPeerGraphic() )->GetRComponent() );
		componentBareUsage += sizeof( deoglRComponent );
		componentMemUsage += sizeof( oglMatrix3x4 ) * oglComponent.GetBoneMatrixCount();
		
		const int lodCount = oglComponent.GetLODCount();
		int l;
		for( l=0; l<lodCount; l++ ){
			const deoglRComponentLOD &lod = oglComponent.GetLODAt( l );
			deoglRModel * const model = oglComponent.GetModel();
			
			componentBareUsage += sizeof( deoglRComponentLOD );
			componentMemUsage += sizeof( deoglVBOpnt ) * lod.GetPointCount();
			if( model ){
				const deoglModelLOD &mlod = model->GetLODAt( lod.GetLODIndex() );
				if( lod.GetWeights() ){
					componentMemUsage += sizeof( oglMatrix3x4 ) + mlod.GetWeightsCount();
				}
				if( lod.GetPositions() ){
					componentMemUsage += sizeof( oglVector ) * mlod.GetPositionCount();
				}
				if( lod.GetNormals() ){
					componentMemUsage += sizeof( oglVector ) * mlod.GetNormalCount();
				}
				if( lod.GetTangents() ){
					componentMemUsage += sizeof( oglVector ) * mlod.GetTangentCount();
				}
				if( lod.GetFaceNormals() ){
					componentMemUsage += sizeof( oglVector ) * mlod.GetFaceCount();
				}
			}
		}
		componentCount++;
		scanComponent = ( deComponent* )scanComponent->GetLLManagerNext();
	}
	pLogger->LogInfoFormat( "OGL-Components: Usage=%dM Bare=%dB Count=%d",
		componentMemUsage / 1000000, componentBareUsage, componentCount );
}

void deoglRenderThread::DebugMemoryUsageSmall( const char *prefix ){
	if( ! DoesDebugMemoryUsage() ) return;
	
	const struct mallinfo minfo( mallinfo() );
	pLogger->LogInfoFormat( "DebugMemoryUsageSmall: %s) TotalAllocSpace=%d TotalFreeSpace=%d",
		prefix, minfo.uordblks, minfo.fordblks );
}
#endif

void deoglRenderThread::pSwapBuffers(){
	const int count = pRRenderWindowList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoglRRenderWindow* )pRRenderWindowList.GetAt( i ) )->SwapBuffers();
	}
}

void deoglRenderThread::pBeginFrame(){
	const deoglDebugTraceGroup debugTrace( *this, "BeginFrame" );
	#ifdef ANDROID
	pContext->CheckConfigurationChanged();
	#endif
	
	// free objects registered for delayed deletion. done before the rendering starts
	// not after since delayed free requests typically happen during synchronization
	// time. during rendering time new objects are created so freeing objects before
	// new ones are created uses GPU memory better
	pDelayedOperations->ProcessFreeOperations();
	
	pDelayedOperations->ProcessInitOperations( 1.0f / 30.0f ); // for VR this can be set lower
	
	pOptimizerManager->Run( 2000 ); // 4000 // DEPRECATED do this using parallel tasks if required
	
	pBufferObject->GetSharedVBOListList().PrepareAllLists();
	pEnvMapSlotManager->IncreaseSlotLastUsedCounters();
	
	#if defined OS_UNIX && ! defined ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
	pContext->ProcessEventLoop();
	#endif
	
	pFrameCounter++; // wraps around when hitting maximum
	pVRBeginFrame();
}

void deoglRenderThread::pSyncConfiguration(){
	deoglConfiguration &config = pOgl.GetConfiguration();
	
	// the main thread configuration is dirty so the user changed some parameters. copy
	// the configuration over to the render thread configuration and set both
	// configurations non-dirty
	if( config.GetDirty() ){
		pConfiguration = config;
		pConfigChanged = true;
		
		pConfiguration.SetDirty( false );
		config.SetDirty( false );
		
		pUpdateConfigFrameLimiter();
		
	// the main thread configuration did not change but the render thread configuration
	// change then a single-frame debug parameter has been reset. in this case update the
	// main thread configuration and mark it non-dirty
	}else if( pConfiguration.GetDirty() ){
		config.SetQuickDebug( pConfiguration.GetQuickDebug() );
		config.SetDebugSnapshot( pConfiguration.GetDebugSnapshot() );
		config.SetDirty( false );
		pConfiguration.SetDirty( false );
	}
}

void deoglRenderThread::pRenderWindows(){
	const int count = pRRenderWindowList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoglRRenderWindow* )pRRenderWindowList.GetAt( i ) )->Render();
	}
}

void deoglRenderThread::pVRBeginFrame(){
	deoglVR * const vr = pVRCamera ? pVRCamera->GetVR() : nullptr;
	if( ! vr ){
		return;
	}
	
	vr->BeginFrame();
	
	if( pSignalSemaphoreSyncVR ){
		pSignalSemaphoreSyncVR = false;
		pSemaphoreSyncVR.Signal();
	}
}

void deoglRenderThread::pVRRender(){
	deoglVR * const vr = pVRCamera ? pVRCamera->GetVR() : nullptr;
	if( vr ){
		vr->Render();
	}
}

void deoglRenderThread::pVRSubmit(){
	deoglVR * const vr = pVRCamera ? pVRCamera->GetVR() : nullptr;
	if( vr ){
		vr->Submit();
	}
}

void deoglRenderThread::pVREndFrame(){
	deoglVR * const vr = pVRCamera ? pVRCamera->GetVR() : nullptr;
	if( vr ){
		vr->EndFrame();
	}
}

void deoglRenderThread::pCaptureCanvas(){
	const int count = pRCaptureCanvasList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoglRCaptureCanvas* )pRCaptureCanvasList.GetAt( i ) )->CapturePending();
	}
}

void deoglRenderThread::pEndFrame(){
	pVREndFrame();
	if( pDebug->GetDeveloperMode().GetEnabled() ){
		if( pDebug->GetDeveloperMode().GetLogMemoryConsumption() ){
			pDebug->GetDebugMemoryConsumption().Update();
		}
	}
}

void deoglRenderThread::pLimitFrameRate( float elapsed ){
	float limit = pFrameTimeLimit;
	
	// if VR is used frame limiter has to be overriden. VR does frame limiting itself
	// to 90Hz. if the rendering can not keep up with steady 90Hz this can result in
	// problems. one solution is to limit the rendering to 45Hz or 30Hz. this will
	// cause the VR environment to adjust for a lesser performing application. For this
	// to work though we have to artifically limit frame rate to not go beyond the
	// threshold or the VR environment can start oscillate
	// 
	// to avoid oscillating a hystersis value is used before switching to a higher
	// target FPS rate. by default the hysteresis is 20%. this ensures the rendering
	// can really keep up with a higher frame rate before switching up
	if( pVRCamera && pVRCamera->GetVR() ){
		// the way SteamVR handles frame time guessing calculation nowadays conflicts
		// largely with this code below. we now simply try to churn out the frames at
		// the time it takes to render them hoping for the runtime to make good guesses
		return;
		/*
		elapsed = pTimerVRFrameUpdate.GetElapsedTime();
		
		deoglVR &vr = *pVRCamera->GetVR();
		vr.UpdateTargetFPS( elapsed );
		
		if( vr.GetTargetFPS() == 90 ){
			return;
		}
		
		limit = 1.0f / ( float )vr.GetTargetFPS();
		
		// VR runtimes use a head-start time of roughly 3ms. if we end up inside the
		// head-start time the VR runtime can guess wrong the required time causing
		// slow-down and frame rate jumping. to avoid this problem the head-start time
		// is subtracted from the frame time limit to be on the save side
		limit -= 0.003f;
		*/
	}
	
	#ifdef OS_W32
	decTimer timer;
	timer.Reset();
	#endif
	
	while( elapsed < limit ){
		// we have some spare time due to the frame limiter. we could do here some
		// optimization work to use the time for intelligent stuff. we can touch
		// the GPU during this time if necessary but if possible it should be
		// avoided. Gives the GPU time to also cool down a bit and not consume
		// energy.
		
		// right now we just sleep
		#ifdef OS_W32
		elapsed += timer.GetElapsedTime();
		
		#else
		timespec timeout, remaining;
		timeout.tv_sec = 0;
		timeout.tv_nsec = ( long )( ( limit - elapsed ) * 1e9f );
		while( nanosleep( &timeout, &remaining ) == -1 && errno == EINTR ){
			timeout = remaining;
		}
		break;
		#endif
	}
	
	pTimerVRFrameUpdate.Reset();
}



void deoglRenderThread::pUpdateConfigFrameLimiter(){
	int refreshRate = pConfiguration.GetFrameRateLimit();
	
	if( refreshRate == 0 ){
		refreshRate = pOgl.GetGameEngine()->GetOS()->GetDisplayCurrentRefreshRate( 0 );
		pLogger->LogInfoFormat( "Using Current Display Refresh Rate: %d", refreshRate );
	}
	
	pFrameTimeLimit = 1.0f / ( ( float )refreshRate + 0.5f );
}

//#define TIME_CLEANUP 1

void deoglRenderThread::pCleanUpThread(){
	// NOTE this call is done in the render thread but has also main thread access
	//      in certain places. this is allowed since the thread cleanup is only called
	//      during clean up time where the main thread is blocked waiting for the
	//      render thread to finish. thus synchronizing is safe to be used here
	
	// guard a reference to the active render window. this is allowed in this call.
	// the context does steal the reference during cleanup so keep this in mind
// 	deoglRRenderWindow *cleanUpWindow = NULL;
	
// 	if( pContext ){
// 		cleanUpWindow = pContext->GetActiveRRenderWindow();
// 		if( cleanUpWindow ){
// 			cleanUpWindow->AddReference();
// 		}
// 	}
	
	#ifdef TIME_CLEANUP
	decTimer cleanUpTimer;
	cleanUpTimer.Reset();
	#endif
	
	try{
		// remove all capture canvas and render windows
		pRCaptureCanvasList.RemoveAll();
		pRRenderWindowList.RemoveAll();
		
// 		if( pContext && cleanUpWindow ){
// 			// required for the leak checks to not report wrong leaks
// 			cleanUpWindow->DropRCanvasView();
// 		}
		#ifdef TIME_CLEANUP
		pLogger->LogInfoFormat( "RT-CleanUp: render windows (%iys)", (int)(cleanUpTimer.GetElapsedTime() * 1e6f) );
		#endif
		
		// remove canvas if present
		if( pCanvasDebugOverlay ){
			pCanvasDebugOverlay->FreeReference();
			pCanvasDebugOverlay = NULL;
		}
		if( pCanvasInputOverlay ){
			pCanvasInputOverlay->FreeReference();
			pCanvasInputOverlay = NULL;
		}
		#ifdef TIME_CLEANUP
		pLogger->LogInfoFormat( "RT-CleanUp: canvas overlay (%iys)", (int)(cleanUpTimer.GetElapsedTime() * 1e6f) );
		#endif
		
		// synchronize to break reference loops in leak checked objects
		pLogger->Synchronize();
		if( pDelayedOperations ){
			pDelayedOperations->ProcessSynchronizeOperations();
		}
		#ifdef TIME_CLEANUP
		pLogger->LogInfoFormat( "RT-CleanUp: delayed operations synchronize (%iys)", (int)(cleanUpTimer.GetElapsedTime() * 1e6f) );
		#endif
		
		// process free operations to remove remaining objects included in the leak report.
		// force deleting all objects to get a clean slate for the leak report.
		if( pDelayedOperations ){
			pDelayedOperations->ProcessFreeOperations( true );
		}
		#ifdef TIME_CLEANUP
		pLogger->LogInfoFormat( "RT-CleanUp: delayed operations free (%iys)", (int)(cleanUpTimer.GetElapsedTime() * 1e6f) );
		#endif
		
		// report leaks
		pReportLeaks(); // only if enabled
		
		// clean up objects
		if( pRenderers ){
			delete pRenderers;
			pRenderers = NULL;
		}
		#ifdef TIME_CLEANUP
		pLogger->LogInfoFormat( "RT-CleanUp: destroy renderers (%iys)", (int)(cleanUpTimer.GetElapsedTime() * 1e6f) );
		#endif
		
		if( pLightBoundarybox ){
			delete pLightBoundarybox;
			pLightBoundarybox = NULL;
		}
		if( pOcclusionTestPool ){
			delete pOcclusionTestPool;
			pOcclusionTestPool = NULL;
		}
		if( pPersistentRenderTaskPool ){
			delete pPersistentRenderTaskPool;
			pPersistentRenderTaskPool = NULL;
		}
		if( pTriangleSorter ){
			delete pTriangleSorter;
			pTriangleSorter = NULL;
		}
		
		if( pOccQueryMgr ){
			delete pOccQueryMgr;
			pOccQueryMgr = NULL;
		}
		if( pGI ){
			delete pGI;
			pGI = NULL;
		}
		#ifdef TIME_CLEANUP
		pLogger->LogInfoFormat( "RT-CleanUp: destroy occlusion managers (%iys)", (int)(cleanUpTimer.GetElapsedTime() * 1e6f) );
		#endif
		
		if( pDelayedOperations ){
			pDelayedOperations->Clear(); // first stage clear
		}
		
		if( pDeferredRendering ){
			delete pDeferredRendering;
			pDeferredRendering = NULL;
		}
		#ifdef TIME_CLEANUP
		pLogger->LogInfoFormat( "RT-CleanUp: destroy deferred rendering (%iys)", (int)(cleanUpTimer.GetElapsedTime() * 1e6f) );
		#endif
		if( pShadowMapper ){
			delete pShadowMapper;
			pShadowMapper = NULL;
		}
		#ifdef TIME_CLEANUP
		pLogger->LogInfoFormat( "RT-CleanUp: destroy shadow mapper (%iys)", (int)(cleanUpTimer.GetElapsedTime() * 1e6f) );
		#endif
		if( pEnvMapSlotManager ){
			delete pEnvMapSlotManager;
			pEnvMapSlotManager = NULL;
		}
		#ifdef TIME_CLEANUP
		pLogger->LogInfoFormat( "RT-CleanUp: destroy env-map slot manager (%iys)", (int)(cleanUpTimer.GetElapsedTime() * 1e6f) );
		#endif
		if( pBufferObject ){
			delete pBufferObject;
			pBufferObject = NULL;
		}
		#ifdef TIME_CLEANUP
		pLogger->LogInfoFormat( "RT-CleanUp: destroy buffer objects (%iys)", (int)(cleanUpTimer.GetElapsedTime() * 1e6f) );
		#endif
		
		if( pFramebuffer ){
			delete pFramebuffer;
			pFramebuffer = NULL;
		}
		#ifdef TIME_CLEANUP
		pLogger->LogInfoFormat( "RT-CleanUp: destroy framebuffers (%iys)", (int)(cleanUpTimer.GetElapsedTime() * 1e6f) );
		#endif
		pPipelineManager = nullptr;
		if( pTexture ){
			delete pTexture;
			pTexture = NULL;
		}
		#ifdef TIME_CLEANUP
		pLogger->LogInfoFormat( "RT-CleanUp: destroy textures (%iys)", (int)(cleanUpTimer.GetElapsedTime() * 1e6f) );
		#endif
		
		pVulkanDevice = nullptr;
		pVulkan = nullptr;
		
		deoglDebugInformationList &dilist = pDebug->GetDebugInformationList();
		dilist.RemoveIfPresent( pDebugInfoModule );
		dilist.RemoveIfPresent( pDebugInfoFrameLimiter );
		
		pDebugInfoModule = nullptr;
		pDebugInfoThreadMain = nullptr;
		pDebugInfoThreadMainWaitFinish = nullptr;
		pDebugInfoThreadMainSynchronize = nullptr;
		pDebugInfoThreadRender = nullptr;
		pDebugInfoThreadRenderSyncGpu = nullptr;
		pDebugInfoThreadRenderBegin = nullptr;
		pDebugInfoThreadRenderWindows = nullptr;
		pDebugInfoThreadRenderWindowsPrepare = nullptr;
		pDebugInfoThreadRenderWindowsRender = nullptr;
		pDebugInfoThreadRenderCapture = nullptr;
		pDebugInfoThreadRenderEnd = nullptr;
		pDebugInfoThreadRenderSwap = nullptr;
		pDebugInfoFrameLimiter = nullptr;
		pDebugInfoFLEstimMain = nullptr;
		pDebugInfoFLEstimRender = nullptr;
		pDebugInfoFLFrameRateMain = nullptr;
		pDebugInfoFLFrameRateRender = nullptr;
		
		#ifdef TIME_CLEANUP
		pLogger->LogInfoFormat( "RT-CleanUp: destroy debug-info (%iys)", (int)(cleanUpTimer.GetElapsedTime() * 1e6f) );
		#endif
		
		if( pDebug ){
			delete pDebug;
			pDebug = NULL;
		}
		#ifdef TIME_CLEANUP
		pLogger->LogInfoFormat( "RT-CleanUp: destroy debug (%iys)", (int)(cleanUpTimer.GetElapsedTime() * 1e6f) );
		#endif
		
		if( pCapabilities ){
			delete pCapabilities;
			pCapabilities = NULL;
		}
		#ifdef TIME_CLEANUP
		pLogger->LogInfoFormat( "RT-CleanUp: destroy capabilities (%iys)", (int)(cleanUpTimer.GetElapsedTime() * 1e6f) );
		#endif
		
		if( pExtensions ){
			delete pExtensions;
			pExtensions = NULL;
		}
		#ifdef TIME_CLEANUP
		pLogger->LogInfoFormat( "RT-CleanUp: destroy extensions (%iys)", (int)(cleanUpTimer.GetElapsedTime() * 1e6f) );
		#endif
		
		if( pDefaultTextures ){
			delete pDefaultTextures;
			pDefaultTextures = NULL;
		}
		#ifdef TIME_CLEANUP
		pLogger->LogInfoFormat( "RT-CleanUp: destroy default textures (%iys)", (int)(cleanUpTimer.GetElapsedTime() * 1e6f) );
		#endif
		
		if( pShader ){
			delete pShader;
			pShader = NULL;
		}
		#ifdef TIME_CLEANUP
		pLogger->LogInfoFormat( "RT-CleanUp: destroy shaders (%iys)", (int)(cleanUpTimer.GetElapsedTime() * 1e6f) );
		#endif
		
		if( pChoices ){
			delete pChoices;
			pChoices = NULL;
		}
		
		// deprecated
		if( pOptimizerManager ){
			delete pOptimizerManager;
			pOptimizerManager = NULL;
		}
		if( pQuickSorter ){
			delete pQuickSorter;
			pQuickSorter = NULL;
		}
		// deprecated
		
		// has to come last
		if( pRenderTaskSharedPool ){
			delete pRenderTaskSharedPool;
			pRenderTaskSharedPool = NULL;
		}
		if( pUniqueKey ){
			delete pUniqueKey;
			pUniqueKey = NULL;
		}
		
		if( pDelayedOperations ){
			delete pDelayedOperations;
			pDelayedOperations = NULL;
		}
		#ifdef TIME_CLEANUP
		pLogger->LogInfoFormat( "RT-CleanUp: destroy delayed operations (%iys)", (int)(cleanUpTimer.GetElapsedTime() * 1e6f) );
		#endif
		
		// free context
// 		cleanUpWindow->FreeReference();
// 		cleanUpWindow = NULL;
		
		if( pContext ){
			pContext->CleanUp();
			
			delete pContext;
			pContext = NULL;
		}
		#ifdef TIME_CLEANUP
		pLogger->LogInfoFormat( "RT-CleanUp: destroy context (%iys)", (int)(cleanUpTimer.GetElapsedTime() * 1e6f) );
		#endif
		
		// synchronize again to process log messages build up during cleaning up
		pLogger->Synchronize();
		
	}catch( const deException & ){
// 		if( cleanUpWindow ){
// 			cleanUpWindow->FreeReference();
// 		}
		throw;
	}
}



void deoglRenderThread::pReportLeaks(){
	// if disabled this all boils down to empty lines
	IF_LEAK_CHECK( pLogger->LogInfo( "*** Leak check reports ***" ) );
	IF_LEAK_CHECK( pLogger->LogInfo( "(NOTE the main render window will be reported as leak although it does not)" ) );
	
	LEAK_CHECK_REPORT_LEAKS( *this, Billboard );
	LEAK_CHECK_REPORT_LEAKS( *this, Camera );
	LEAK_CHECK_REPORT_LEAKS( *this, CanvasCanvasView );
	LEAK_CHECK_REPORT_LEAKS( *this, CanvasImage );
	LEAK_CHECK_REPORT_LEAKS( *this, CanvasPaint );
	LEAK_CHECK_REPORT_LEAKS( *this, CanvasRenderWorld );
	LEAK_CHECK_REPORT_LEAKS( *this, CanvasText );
	LEAK_CHECK_REPORT_LEAKS( *this, CanvasVideoPlayer );
	LEAK_CHECK_REPORT_LEAKS( *this, CanvasView );
	LEAK_CHECK_REPORT_LEAKS( *this, CaptureCanvas );
	LEAK_CHECK_REPORT_LEAKS( *this, Component );
	LEAK_CHECK_REPORT_LEAKS( *this, ComponentLOD );
	LEAK_CHECK_REPORT_LEAKS( *this, ComponentTexture );
	LEAK_CHECK_REPORT_LEAKS( *this, Decal );
	LEAK_CHECK_REPORT_LEAKS( *this, DebugDrawer );
	LEAK_CHECK_REPORT_LEAKS( *this, DSRenderableCamera );
	LEAK_CHECK_REPORT_LEAKS( *this, DSRenderableCanvas );
	LEAK_CHECK_REPORT_LEAKS( *this, DSRenderableColor );
	LEAK_CHECK_REPORT_LEAKS( *this, DSRenderableImage );
	LEAK_CHECK_REPORT_LEAKS( *this, DSRenderableValue );
	LEAK_CHECK_REPORT_LEAKS( *this, DSRenderableVideoFrame );
	LEAK_CHECK_REPORT_LEAKS( *this, DynamicSkin );
	LEAK_CHECK_REPORT_LEAKS( *this, EffectColorMatrix );
	LEAK_CHECK_REPORT_LEAKS( *this, EffectDistortImage );
	LEAK_CHECK_REPORT_LEAKS( *this, EffectFilterKernel );
	LEAK_CHECK_REPORT_LEAKS( *this, EffectOverlayImage );
	LEAK_CHECK_REPORT_LEAKS( *this, EnvironmentMapProbe );
	LEAK_CHECK_REPORT_LEAKS( *this, Font );
	LEAK_CHECK_REPORT_LEAKS( *this, HeightTerrain );
	LEAK_CHECK_REPORT_LEAKS( *this, HTSector );
	LEAK_CHECK_REPORT_LEAKS( *this, Image );
	LEAK_CHECK_REPORT_LEAKS( *this, Light );
	LEAK_CHECK_REPORT_LEAKS( *this, Lumimeter );
	LEAK_CHECK_REPORT_LEAKS( *this, Model );
	LEAK_CHECK_REPORT_LEAKS( *this, OcclusionMesh );
	LEAK_CHECK_REPORT_LEAKS( *this, ParticleEmitter );
	LEAK_CHECK_REPORT_LEAKS( *this, ParticleEmitterType );
	LEAK_CHECK_REPORT_LEAKS( *this, ParticleEmitterInstance );
	LEAK_CHECK_REPORT_LEAKS( *this, ParticleEmitterInstanceType );
	LEAK_CHECK_REPORT_LEAKS( *this, PropField );
	LEAK_CHECK_REPORT_LEAKS( *this, PropFieldType );
	LEAK_CHECK_REPORT_LEAKS( *this, RenderWindow );
	LEAK_CHECK_REPORT_LEAKS( *this, Skin );
	LEAK_CHECK_REPORT_LEAKS( *this, Sky );
	LEAK_CHECK_REPORT_LEAKS( *this, SkyLayer );
	LEAK_CHECK_REPORT_LEAKS( *this, SkyInstance );
	LEAK_CHECK_REPORT_LEAKS( *this, World );
	
	IF_LEAK_CHECK( pLogger->LogInfo( "*** End of leak check reports ***" ) );
}
