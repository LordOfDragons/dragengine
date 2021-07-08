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
#include "../deoglPreloader.h"
#include "../canvas/deoglCanvasView.h"
#include "../canvas/capture/deoglRCaptureCanvas.h"
#include "../canvas/render/deoglRCanvas.h"
#include "../capabilities/deoglCapabilities.h"
#include "../configuration/deoglConfiguration.h"
#include "../debug/deoglDebugInformation.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../devmode/deoglDeveloperMode.h"
#include "../edgefinder/deoglEdgeFinder.h"
#include "../envmap/deoglEnvMapSlotManager.h"
#include "../extensions/deoglExtensions.h"
#include "../gi/deoglGI.h"
#include "../light/deoglLightBoundaryMap.h"
#include "../occlusiontest/deoglOcclusionTestPool.h"
#include "../occquery/deoglOcclusionQueryManager.h"
#include "../optimizer/deoglOptimizerManager.h"
#include "../rendering/deoglRenderCanvas.h"
#include "../rendering/defren/deoglDeferredRendering.h"
#include "../rendering/task/persistent/deoglPersistentRenderTaskPool.h"
#include "../rendering/task/shared/deoglRenderTaskSharedPool.h"
#include "../shadow/deoglShadowMapper.h"
#include "../texture/deoglTextureStageManager.h"
#include "../triangles/deoglTriangleSorter.h"
#include "../utils/deoglQuickSorter.h"
#include "../vbo/deoglSharedVBOListList.h"
#include "../window/deoglRenderWindow.h"
#include "../window/deoglRRenderWindow.h"

#include <deSharedVulkan.h>

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

pLeakTracker( *this ),

pCanvasInputOverlay( NULL ),
pCanvasDebugOverlay( NULL ),

pBufferObject( NULL ),
pContext( NULL ),
pDebug( NULL ),
pDefaultTextures( NULL ),
pFramebuffer( NULL ),
pLogger( NULL ),
pRenderers( NULL ),
pShader( NULL ),
pTexture( NULL ),

pCapabilities( NULL ),
pDeferredRendering( NULL ),
pDelayedOperations( NULL ),
pEnvMapSlotManager( NULL ),
pExtensions( NULL ),
pLightBoundarybox( NULL ),
pOccQueryMgr( NULL ),
pGI( NULL ),
pShadowMapper( NULL ),
pTriangleSorter( NULL ),
pPersistentRenderTaskPool( NULL ),
pRenderTaskSharedPool( NULL ),
pUniqueKey( NULL ),
pOcclusionTestPool( NULL ),
pVulkan( NULL ),

pTimeHistoryMain( 29, 2 ),
pTimeHistoryRender( 29, 2 ),
pEstimatedRenderTime( 0.0f ),
pAccumulatedMainTime( 0.0f ),
pFrameTimeLimit( 1.0f / 30.0f ),

pDebugInfoModule( NULL ),

pDebugInfoThreadMain( NULL ),
pDebugInfoThreadMainWaitFinish( NULL ),
pDebugInfoThreadMainSynchronize( NULL ),

pDebugInfoThreadRender( NULL ),
pDebugInfoThreadRenderSwap( NULL ),
pDebugInfoThreadRenderBegin( NULL ),
pDebugInfoThreadRenderWindows( NULL ),
pDebugInfoThreadRenderWindowsPrepare( NULL ),
pDebugInfoThreadRenderWindowsRender( NULL ),
pDebugInfoThreadRenderCapture( NULL ),
pDebugInfoThreadRenderEnd( NULL ),

pDebugInfoFrameLimiter( NULL ),
pDebugInfoFLEstimMain( NULL ),
pDebugInfoFLEstimRender( NULL ),
pDebugInfoFLFrameRateMain( NULL ),
pDebugInfoFLFrameRateRender( NULL ),

pMainThreadShowDebugInfoModule( false ),

pDebugTimeThreadMainWaitFinish( 0.0f ),

pDebugTimeThreadRenderSwap( 0.0f ),
pDebugTimeThreadRenderBegin( 0.0f ),
pDebugTimeThreadRenderWindows( 0.0f ),
pDebugTimeThreadRenderWindowsPrepare( 0.0f ),
pDebugTimeThreadRenderWindowsRender( 0.0f ),
pDebugTimeThreadRenderCapture( 0.0f ),
pDebugCountThreadWindows( 0 ),

// deprecated
pQuickSorter( NULL ),
pPreloader( NULL ),
pEdgeFinder( NULL ),
pOptimizerManager( NULL ),

// thread control parameters
pInitialRenderWindow( NULL ), // temporary variable for thread initialization
pThreadState( etsStopped ),
pThreadFailure( false ),
pBarrierSyncIn( 2 ),
pBarrierSyncOut( 2 )
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



void deoglRenderThread::SetCanvasInputOverlay( deoglRCanvas *canvas ){
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

void deoglRenderThread::SetCanvasDebugOverlay( deoglRCanvas *canvas ){
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
			
		}catch( const deException &e ){
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
			
		}catch( const deException &exception ){
			pLogger->LogException( exception );
		}
		
		return;
	}
	
	DEBUG_SYNC_RT_WAIT("out")
	pBarrierSyncOut.Wait();
	DEBUG_SYNC_RT_PASS("out")
	
	// render loop
	pTimerFrameUpdate.Reset();
	while( true ){
		// wait for entering synchronize
		DEBUG_SYNC_RT_WAIT("in")
		pBarrierSyncIn.Wait();
		DEBUG_SYNC_RT_PASS("in")
		
		// main thread is messing with our state here. proceed to next barrier doing nothing
		// except alter the estimated render time. this value is used by the main thread
		// only outside the synchronization part so we can update it here
		pEstimatedRenderTime = decMath::max( pTimeHistoryRender.GetAverage(), pFrameTimeLimit );
		
		// wait for leaving synchronize
		DEBUG_SYNC_RT_WAIT("out")
		pBarrierSyncOut.Wait();
		DEBUG_SYNC_RT_PASS("out")
		
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
				pThreadFailure = true;
				DEBUG_SYNC_RT_FAILURE
			}
			
			pTimerFrameUpdate.GetElapsedTime();
			/*
			const float elapsedFrameUpdate = pTimerFrameUpdate.GetElapsedTime();
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
	
	if( pAsyncRendering ){
		// begin rendering next frame unless thread is not active
		if( pThreadState == etsStopped ){
			return;
		}
		
		pThreadState = etsRendering;
		DEBUG_SYNC_MT_STATE
		
		DEBUG_SYNC_MT_WAIT("out");
		pBarrierSyncOut.Wait();
		DEBUG_SYNC_MT_PASS("out");
		
		pTimerMain.Reset();
		
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
			
			if( remainingTime / estimatedGameTime >= ratioTimes ){
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

void deoglRenderThread::pInitThreadPhase4(){
	pContext->InitPhase4( pInitialRenderWindow );
	pInitialRenderWindow = NULL;
	
	// deprecated
	pQuickSorter = new deoglQuickSorter;
	pPreloader = new deoglPreloader( *this );
	pEdgeFinder = new deoglEdgeFinder;
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
	
	pInitCapabilities();
	
	// debug information
	const decColor colorText( 1.0f, 1.0f, 1.0f, 1.0f );
	const decColor colorBg( 0.0f, 0.0f, 0.25f, 0.75f );
	const decColor colorBgSub( 0.05f, 0.05f, 0.05f, 0.75f );
	const decColor colorBgSub2( 0.1f, 0.1f, 0.1f, 0.75f );
	const decColor colorBgSub3( 0.15f, 0.15f, 0.15f, 0.75f );
	
	pDebugInfoModule = new deoglDebugInformation( "Module", colorText, colorBg );
	pDebugInfoModule->SetVisible( false );
	pDebug->GetDebugInformationList().Add( pDebugInfoModule );
 	
	pDebugInfoThreadMain = new deoglDebugInformation( "Main Thread", colorText, colorBgSub );
	pDebugInfoModule->GetChildren().Add( pDebugInfoThreadMain );
		
		pDebugInfoThreadMainWaitFinish = new deoglDebugInformation( "Wait Finish", colorText, colorBgSub2 );
		pDebugInfoThreadMain->GetChildren().Add( pDebugInfoThreadMainWaitFinish );
		
		pDebugInfoThreadMainSynchronize = new deoglDebugInformation( "Synchronize", colorText, colorBgSub2 );
		pDebugInfoThreadMain->GetChildren().Add( pDebugInfoThreadMainSynchronize );
	
	pDebugInfoThreadRender = new deoglDebugInformation( "Render Thread", colorText, colorBgSub );
	pDebugInfoModule->GetChildren().Add( pDebugInfoThreadRender );
		
		pDebugInfoThreadRenderSwap = new deoglDebugInformation( "Swap Buffers", colorText, colorBgSub2 );
		pDebugInfoThreadRender->GetChildren().Add( pDebugInfoThreadRenderSwap );
		
		pDebugInfoThreadRenderBegin = new deoglDebugInformation( "Begin", colorText, colorBgSub2 );
		pDebugInfoThreadRender->GetChildren().Add( pDebugInfoThreadRenderBegin );
		
		pDebugInfoThreadRenderWindows = new deoglDebugInformation( "Windows", colorText, colorBgSub2 );
		pDebugInfoThreadRender->GetChildren().Add( pDebugInfoThreadRenderWindows );
			
			pDebugInfoThreadRenderWindowsPrepare = new deoglDebugInformation( "Prepare", colorText, colorBgSub3 );
			pDebugInfoThreadRenderWindows->GetChildren().Add( pDebugInfoThreadRenderWindowsPrepare );
			
			pDebugInfoThreadRenderWindowsRender = new deoglDebugInformation( "Render", colorText, colorBgSub3 );
			pDebugInfoThreadRenderWindows->GetChildren().Add( pDebugInfoThreadRenderWindowsRender );
		
		pDebugInfoThreadRenderCapture = new deoglDebugInformation( "Capture", colorText, colorBgSub2 );
		pDebugInfoThreadRender->GetChildren().Add( pDebugInfoThreadRenderCapture );
		
		pDebugInfoThreadRenderEnd = new deoglDebugInformation( "End", colorText, colorBgSub2 );
		pDebugInfoThreadRender->GetChildren().Add( pDebugInfoThreadRenderEnd );
	
	pDebugInfoFrameLimiter = new deoglDebugInformation( "Frame Limiter", colorText, colorBg );
	pDebugInfoFrameLimiter->SetVisible( false );
	pDebug->GetDebugInformationList().Add( pDebugInfoFrameLimiter );
		
		pDebugInfoFLEstimMain = new deoglDebugInformation( "Estimate main", colorText, colorBgSub );
		pDebugInfoFrameLimiter->GetChildren().Add( pDebugInfoFLEstimMain );
		
		pDebugInfoFLEstimRender = new deoglDebugInformation( "Estimate Render", colorText, colorBgSub );
		pDebugInfoFrameLimiter->GetChildren().Add( pDebugInfoFLEstimRender );
		
		pDebugInfoFLFrameRateMain = new deoglDebugInformation( "FPS Main", colorText, colorBgSub );
		pDebugInfoFrameLimiter->GetChildren().Add( pDebugInfoFLFrameRateMain );
		
		pDebugInfoFLFrameRateRender = new deoglDebugInformation( "FPS Render", colorText, colorBgSub );
		pDebugInfoFrameLimiter->GetChildren().Add( pDebugInfoFLFrameRateRender );
	
	// below depends on capabilities being known
	pChoices = new deoglRTChoices( *this );
	
	pBufferObject = new deoglRTBufferObject( *this );
	pBufferObject->Init();
	
	pTriangleSorter = new deoglTriangleSorter;
	pOcclusionTestPool = new deoglOcclusionTestPool( *this );
	pPersistentRenderTaskPool = new deoglPersistentRenderTaskPool;
	pDelayedOperations = new deoglDelayedOperations( *this );
	pShadowMapper = new deoglShadowMapper( *this );
	pDeferredRendering = new deoglDeferredRendering( *this );
	pEnvMapSlotManager = new deoglEnvMapSlotManager( *this );
	
	pOccQueryMgr = new deoglOcclusionQueryManager( *this );
	pGI = new deoglGI( *this );
	pLightBoundarybox = new deoglLightBoundaryMap( *this,
		deoglShadowMapper::ShadowMapSize( pConfiguration ) >> 1 );
	
	pRenderers = new deoglRTRenderers( *this );
	pDefaultTextures = new deoglRTDefaultTextures( *this );
	
	// load vulkan if supported
	try{
		pVulkan = new deSharedVulkan( pOgl );
		
	}catch( const deException &e ){
		pLogger->LogException( e );
	}
	// initial notification. required for testing if devode config is hardcoded in constructor
	DevModeDebugInfoChanged();
	
	// some final preparations. is this really required?
	pTexture->GetStages().DisableAllStages();
	
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
		DETHROW( deeInvalidAction );
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
		
		pEndFrame();
		
	}else{
		pSwapBuffers();
		if( showDebugInfoModule ){
			pDebugTimeThreadRenderSwap = pDebugTimerRenderThread2.GetElapsedTime();
		}
		
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
		
		pEndFrame();
	}
	
	if( showDebugInfoModule ){
		const float time2 = pDebugTimerRenderThread2.GetElapsedTime();
		const float time1 = pDebugTimerRenderThread1.GetElapsedTime();
		
		pDebugInfoThreadRenderSwap->Clear();
		pDebugInfoThreadRenderSwap->IncrementElapsedTime( pDebugTimeThreadRenderSwap );
		
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
	
	return pOgl.GetGameEngine()->GetCacheAppID() == "zoids";
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
	
	// renderbuffer
	const deoglMemoryConsumptionTexture &consumptionRenderbuffer = pMemoryManager.GetConsumption().GetRenderbuffer();
	const int renderbufferCount = consumptionRenderbuffer.GetCount();
	const int renderbufferColorCount = consumptionRenderbuffer.GetColorCount();
	const int renderbufferDepthCount = consumptionRenderbuffer.GetDepthCount();
	unsigned int renderbufferGPU = consumptionRenderbuffer.GetGPU();
	unsigned int renderbufferColorGPU = consumptionRenderbuffer.GetColorGPU();
	unsigned int renderbufferDepthGPU = consumptionRenderbuffer.GetDepthGPU();
	
	renderbufferGPU /= 1000000;
	renderbufferColorGPU /= 1000000;
	renderbufferDepthGPU /= 1000000;
	
	pLogger->LogInfoFormat( fmtRenBuf, renderbufferCount, renderbufferGPU, renderbufferColorCount, renderbufferColorGPU,
		renderbufferDepthCount, renderbufferDepthGPU );
	
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
	const deoglMemoryConsumptionVBO &consumptionVBO = pMemoryManager.GetConsumption().GetVBO();
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
	int defrenGPURenBuf = pDeferredRendering->GetMemoryUsageGPURenderbuffer() / 1000000;
	
	pLogger->LogInfoFormat( fmtDefRen, defrenGPU, defrenGPUTexture, defrenGPURenBuf );
	
	// grand total of all above
	int totalGPU;
	
	totalGPU = consumptionTexture2D.GetGPU();
	totalGPU += consumptionTextureArray.GetGPU();
	totalGPU += consumptionTextureCube.GetGPU();
	totalGPU += consumptionRenderbuffer.GetGPU();
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
	#ifdef ANDROID
	pContext->CheckConfigurationChanged();
	#endif
	
	// free objects registered for delayed deletion. done before the rendering starts not after
	// since delayed free requests are registered during the synchronization time. during
	// rendering new objects are created so freeing objects before new ones are created is
	// uses GPU memory better. the number of objects deleted per call is limited to prevent
	// hickups if lots of objects are deleted.
	pDelayedOperations->ProcessFreeOperations( false );
	
	pDelayedOperations->ProcessInitOperations();
	
	pPreloader->PreloadAll(); // DEPRECATED
	pOptimizerManager->Run( 2000 ); // 4000 // DEPRECATED do this using parallel tasks if required
	
	pBufferObject->GetSharedVBOListList().PrepareAllLists();
	pEnvMapSlotManager->IncreaseSlotLastUsedCounters();
	
	#if defined OS_UNIX && ! defined ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
	pContext->ProcessEventLoop();
	#endif
}

void deoglRenderThread::pSyncConfiguration(){
	deoglConfiguration &config = pOgl.GetConfiguration();
	
	// the main thread configuration is dirty so the user changed some parameters. copy
	// the configuration over to the render thread configuration and set both
	// configurations non-dirty
	if( config.GetDirty() ){
		const bool needResize = ( config.GetDefRenUsePOTs() != pConfiguration.GetDefRenUsePOTs() );
		
		pConfiguration = config;
		pConfigChanged = true;
		
		pConfiguration.SetDirty( false );
		config.SetDirty( false );
		
		pDeferredRendering->SetUseEncodedDepth( pConfiguration.GetUseEncodeDepth() );
		if( needResize ){
			pDeferredRendering->ForceResize();
		}
		
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

void deoglRenderThread::pCaptureCanvas(){
	const int count = pRCaptureCanvasList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoglRCaptureCanvas* )pRCaptureCanvasList.GetAt( i ) )->CapturePending();
	}
}

void deoglRenderThread::pEndFrame(){
}

void deoglRenderThread::pLimitFrameRate( float elapsed ){
	#ifdef OS_W32
	decTimer timer;
	timer.Reset();
	#endif
	
	while( elapsed < pFrameTimeLimit ){
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
		timeout.tv_nsec = ( long )( ( pFrameTimeLimit - elapsed ) * 1e9f );
		while( nanosleep( &timeout, &remaining ) == -1 && errno == EINTR ){
			timeout = remaining;
		}
		break;
		#endif
	}
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
			delete pDelayedOperations;
			pDelayedOperations = NULL;
		}
		#ifdef TIME_CLEANUP
		pLogger->LogInfoFormat( "RT-CleanUp: destroy delayed operations (%iys)", (int)(cleanUpTimer.GetElapsedTime() * 1e6f) );
		#endif
		
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
		if( pTexture ){
			delete pTexture;
			pTexture = NULL;
		}
		#ifdef TIME_CLEANUP
		pLogger->LogInfoFormat( "RT-CleanUp: destroy textures (%iys)", (int)(cleanUpTimer.GetElapsedTime() * 1e6f) );
		#endif
		
		if( pVulkan ){
			delete pVulkan;
			pVulkan = NULL;
		}
		
		if( pDebugInfoModule ){
			pDebug->GetDebugInformationList().Remove( pDebugInfoModule );
			pDebugInfoModule->FreeReference();
			pDebugInfoModule = NULL;
		}
		
		if( pDebugInfoThreadMain ){
			pDebugInfoThreadMain->FreeReference();
			pDebugInfoThreadMain = NULL;
		}
		if( pDebugInfoThreadMainWaitFinish ){
			pDebugInfoThreadMainWaitFinish->FreeReference();
			pDebugInfoThreadMainWaitFinish = NULL;
		}
		if( pDebugInfoThreadMainSynchronize ){
			pDebugInfoThreadMainSynchronize->FreeReference();
			pDebugInfoThreadMainSynchronize = NULL;
		}
		
		if( pDebugInfoThreadRender ){
			pDebugInfoThreadRender->FreeReference();
			pDebugInfoThreadRender = NULL;
		}
		if( pDebugInfoThreadRenderSwap ){
			pDebugInfoThreadRenderSwap->FreeReference();
			pDebugInfoThreadRenderSwap = NULL;
		}
		if( pDebugInfoThreadRenderBegin ){
			pDebugInfoThreadRenderBegin->FreeReference();
			pDebugInfoThreadRenderBegin = NULL;
		}
		if( pDebugInfoThreadRenderWindows ){
			pDebugInfoThreadRenderWindows->FreeReference();
			pDebugInfoThreadRenderWindows = NULL;
		}
		if( pDebugInfoThreadRenderWindowsPrepare ){
			pDebugInfoThreadRenderWindowsPrepare->FreeReference();
			pDebugInfoThreadRenderWindowsPrepare = NULL;
		}
		if( pDebugInfoThreadRenderWindowsRender ){
			pDebugInfoThreadRenderWindowsRender->FreeReference();
			pDebugInfoThreadRenderWindowsRender = NULL;
		}
		if( pDebugInfoThreadRenderCapture ){
			pDebugInfoThreadRenderCapture->FreeReference();
			pDebugInfoThreadRenderCapture = NULL;
		}
		if( pDebugInfoThreadRenderEnd ){
			pDebugInfoThreadRenderEnd->FreeReference();
			pDebugInfoThreadRenderEnd = NULL;
		}
		
		if( pDebugInfoFrameLimiter ){
			pDebug->GetDebugInformationList().Remove( pDebugInfoFrameLimiter );
			pDebugInfoFrameLimiter->FreeReference();
			pDebugInfoFrameLimiter = NULL;
		}
		if( pDebugInfoFLEstimMain ){
			pDebugInfoFLEstimMain->FreeReference();
			pDebugInfoFLEstimMain = NULL;
		}
		if( pDebugInfoFLEstimRender ){
			pDebugInfoFLEstimRender->FreeReference();
			pDebugInfoFLEstimRender = NULL;
		}
		if( pDebugInfoFLFrameRateMain ){
			pDebugInfoFLFrameRateMain->FreeReference();
			pDebugInfoFLFrameRateMain = NULL;
		}
		if( pDebugInfoFLFrameRateRender ){
			pDebugInfoFLFrameRateRender->FreeReference();
			pDebugInfoFLFrameRateRender = NULL;
		}
		
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
		if( pEdgeFinder ){
			delete pEdgeFinder;
			pEdgeFinder = NULL;
		}
		if( pPreloader ){
			delete pPreloader;
			pPreloader = NULL;
		}
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
