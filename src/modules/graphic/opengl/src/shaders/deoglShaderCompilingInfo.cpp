/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "deoglShaderCompilingInfo.h"
#include "deoglShaderLanguage.h"
#include "deoglShaderManager.h"
#include "../deGraphicOpenGl.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTShader.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/font/deFontManager.h>
#include <dragengine/resources/loader/deResourceLoader.h>
#include <dragengine/resources/loader/tasks/deResourceLoaderTask.h>
#include <dragengine/resources/video/deVideoManager.h>
#include <dragengine/resources/video/deVideoPlayerManager.h>
#include <dragengine/threading/deMutexGuard.h>


// Class deoglShaderCompilingInfo
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderCompilingInfo::deoglShaderCompilingInfo( deGraphicOpenGl &ogl ) :
pOgl( ogl ),
pState( esInvisible ),
pDelayFadeIn( 0.25f ),
pTimeFadeIn( 0.25f ),
pDelayFadeOut( 1.0f ),
pTimeFadeOut( 0.5f ),
pElapsed( 0.0f ),
pTransparency( 0.0f ),
pHasLoadingShader( false ),
pHasCompilingShader( false )
{
	pCreateCanvas();
}

deoglShaderCompilingInfo::~deoglShaderCompilingInfo(){
	if( pCanvasView ){
		pOgl.GetOverlay()->RemoveCanvas( pCanvasView );
	}
}



// Management
///////////////

bool deoglShaderCompilingInfo::IsVisible() const{
	return pState != esInvisible && pState != esDelayFadeIn;
}

void deoglShaderCompilingInfo::Update( float elapsed ){
	pUpdateChecks();
	pUpdateState( elapsed );
	pUpdateCanvas( elapsed );
	
	// pOgl.LogInfoFormat("ShaderCompilingInfo: hls=%d hcs=%d s=%d e=%f",
		// pHasLoadingShader, pHasCompilingShader, pState, pElapsed);
}



// Private Functions
//////////////////////

void deoglShaderCompilingInfo::pCreateCanvas(){
	deEngine &engine = *pOgl.GetGameEngine();
	deCanvasManager &canvasManager = *engine.GetCanvasManager();
	deVirtualFileSystem &vfs = pOgl.GetVFS();
	
	// the correct and optimal solution would be to use asynchronous loading. for this the
	// resource loader has to be used place a loading request. the returned task is not held
	// and has to be put aside to be checked in the upcoming frames for state to be no more
	// pending. if the state is then esSucceeded then GetResource() can be used with casting
	// to store the resource into the canvas. something for later for deoglResources
	/*
	deResourceLoader &loader = *engine.GetResourceLoader();
	deTObjectReference<deResourceLoaderTask> task( loader.AddLoadRequest(
		&vfs, "/share/images/compileShaders.apng", deResourceLoader::ertVideo ) );
	if( task->GetState() != deResourceLoaderTask::esPending ) ...
	*/
	
	pVideoCompile.TakeOver( engine.GetVideoManager()->LoadVideo(
		&vfs, "/share/images/compileShaders.apng", "/", false ) );
	
	pVideoPlayerCompile.TakeOver( engine.GetVideoPlayerManager()->CreateVideoPlayer() );
	pVideoPlayerCompile->SetVideo( pVideoCompile );
	pVideoPlayerCompile->SetLooping( true );
	
	pCanvasView.TakeOver( canvasManager.CreateCanvasView() );
	pCanvasView->SetOrder( 1.0f );
	pOgl.GetOverlay()->AddCanvas( pCanvasView );
	
	pCanvasVideo.TakeOver( canvasManager.CreateCanvasVideoPlayer() );
	pCanvasVideo->SetOrder( 1.0f );
	pCanvasVideo->SetVideoPlayer( pVideoPlayerCompile );
	pCanvasVideo->SetSize( decPoint( pVideoCompile->GetWidth(), pVideoCompile->GetHeight() ) );
	pCanvasView->AddCanvas( pCanvasVideo );
	
	pCanvasText.TakeOver( canvasManager.CreateCanvasText() );
	pCanvasText->SetColor( decColor( 1.0f, 1.0f, 1.0f ) );
	pCanvasText->SetOrder( 2.0f );
	pCanvasView->AddCanvas( pCanvasText );
}

void deoglShaderCompilingInfo::pUpdateChecks(){
	deoglShaderLanguage &shaderLanguage = *pOgl.GetRenderThread().GetShader().GetShaderManager().GetLanguage();
	pHasLoadingShader = shaderLanguage.GetHasLoadingShader();
	pHasCompilingShader = shaderLanguage.GetHasCompilingShader();
}

bool deoglShaderCompilingInfo::pShouldBeVisible() const{
	return pHasLoadingShader || pHasCompilingShader;
}

void deoglShaderCompilingInfo::pUpdateState( float elapsed ){
	if( pState == esInvisible ){
		if( ! pShouldBeVisible() ){
			return;
		}
		
		pState = esDelayFadeIn;
		pElapsed = 0.0f;
		
	}else if( pState == esVisible ){
		if( pShouldBeVisible() ){
			return;
		}
		
		pState = esDelayFadeOut;
		pElapsed = 0.0f;
	}
	
	if( pState == esDelayFadeIn ){
		if( ! pShouldBeVisible() ){
			pState = esInvisible;
			pElapsed = 0.0f;
			return;
		}
		
		pElapsed += elapsed;
		if( pElapsed < pDelayFadeIn ){
			return;
		}
		
		pState = esFadeIn;
		pElapsed -= pDelayFadeIn;
		
	}else if( pState == esDelayFadeOut ){
		if( pShouldBeVisible() ){
			pState = esVisible;
			pElapsed = 0.0f;
			return;
		}
		
		pElapsed += elapsed;
		if( pElapsed < pDelayFadeOut ){
			return;
		}
		
		pState = esFadeOut;
		pElapsed -= pDelayFadeOut;
	}
	
	if( pState == esFadeIn ){
		pElapsed += elapsed;
		if( pElapsed >= pTimeFadeIn ){
			pState = esVisible;
			pElapsed = 0.0f;
		}
		pUpdateTransparency();
		
	}else if( pState == esFadeOut ){
		if( pShouldBeVisible() ){
			pState = esFadeIn;
			pElapsed = decMath::linearStep( pElapsed, 0.0f, pTimeFadeOut, pTimeFadeIn, 0.0f );
			
		}else{
			pElapsed += elapsed;
			if( pElapsed >= pTimeFadeOut ){
				pState = esInvisible;
				pElapsed = 0.0f;
			}
		}
		pUpdateTransparency();
	}
}

void deoglShaderCompilingInfo::pUpdateTransparency(){
	switch( pState ){
	case esInvisible:
	case esDelayFadeIn:
		pTransparency = 0.0f;
		break;
		
	case esFadeIn:
		pTransparency = decMath::linearStep( pElapsed, 0.0f, pTimeFadeIn, 0.0f, 1.0f );
		break;
		
	case esVisible:
	case esDelayFadeOut:
		pTransparency = 1.0f;
		break;
		
	case esFadeOut:
		pTransparency = decMath::linearStep( pElapsed, 0.0f, pTimeFadeOut, 1.0f, 0.0f );
		break;
	}
}

void deoglShaderCompilingInfo::pUpdateCanvas( float elapsed ){
	if( ! IsVisible() ){
		pCanvasView->SetVisible( false );
		pVideoPlayerCompile->Stop();
		pVideoPlayerCompile->SetPlayPosition( 0.0f );
		return;
	}
	
	const decPoint overlaySize( pOgl.GetOverlay()->GetSize() );
	
	const decPoint padding( 40, 20 );
	const decPoint panelSize( 150, 80 );
	const decPoint position( overlaySize - padding - panelSize );
	
	pCanvasView->SetSize( panelSize );
	pCanvasView->SetPosition( position );
	pCanvasView->SetTransparency( pTransparency );
	pCanvasView->SetVisible( true );
	
	if( pVideoPlayerCompile->GetPlaying() ){
		pVideoPlayerCompile->Update( elapsed );
		
	}else{
		pVideoPlayerCompile->Play();
	}
}
