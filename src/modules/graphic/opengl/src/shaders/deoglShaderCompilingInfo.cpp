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

#include "deoglShaderCompilingInfo.h"
#include "deoglShaderLanguage.h"
#include "deoglShaderManager.h"
#include "../deGraphicOpenGl.h"
#include "../canvas/deoglCanvasView.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTShader.h"
#include "../video/deoglVideoPlayer.h"
#include "../video/deoglRVideoPlayer.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
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
pMode( emLoading ),
pDelayFadeIn( 0.25f ),
pTimeFadeIn( 0.25f ),
pDelayFadeOut( 0.25f ),
pTimeFadeOut( 0.25f ),
pElapsed( 0.0f ),
pFrames( 0 ),
pTransparency( 0.0f ),
pMaxTransparency( 1.0f ), // 0.95f
pHasLoadingShader( false ),
pHasCompilingShader( false ),
pVideoCompilePlayPosition( 0.0f )
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
	elapsed = decMath::min( elapsed, 1.0f / 30.0f );
	
	pUpdateChecks();
	pUpdateState( elapsed );
	pUpdateCanvas( elapsed );
	
	// pOgl.LogInfoFormat("ShaderCompilingInfo: hls=%d hcs=%d s=%d e=%f",
		// pHasLoadingShader, pHasCompilingShader, pState, pElapsed);
}

void deoglShaderCompilingInfo::PrepareForRender( float elapsed ){
	// this is a little trick. we update the video player elapsed time directly from the
	// render thread instead of the main thread. this ensures the playback is smooth even
	// if the main thread does some expensive operations which prevent sync in time
	if( pVideoPlayerCompile && pVideoPlayerCompile->GetPeerGraphic() && pVideoPlayerCompile->GetPlaying() ){
		deoglVideoPlayer &vplayer = *( ( deoglVideoPlayer* )pVideoPlayerCompile->GetPeerGraphic() );
		
		pVideoCompilePlayPosition = decMath::normalize( pVideoCompilePlayPosition + elapsed,
			0.0f, pVideoCompile->GetPlayTime() );
		vplayer.SetCurrentFrame( ( int )( pVideoCompilePlayPosition
			* ( float )pVideoCompile->GetFrameRate() + 0.5f ) );
		// vplayer.SyncToRender();
	}
	
	if( pCanvasView && pCanvasView->GetPeerGraphic() ){
		deoglCanvasView &canvasView = *( ( deoglCanvasView* )pCanvasView->GetPeerGraphic() );
		canvasView.SyncToRender();
	}
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
		&vfs, "/share/videos/compileShaders.webm", deResourceLoader::ertVideo ) );
	if( task->GetState() != deResourceLoaderTask::esPending ) ...
	*/
	
	const decColor textColor( 0.85f, 0.85f, 0.85f );
	
	pFontText.TakeOver( engine.GetFontManager()->LoadFont(
		&vfs, "loading.defont", "/share/fonts" ) );
	
	pVideoCompile.TakeOver( engine.GetVideoManager()->LoadVideo(
		&vfs, "compileShaders.webm", "/share/videos", false ) );
	
	pVideoPlayerCompile.TakeOver( engine.GetVideoPlayerManager()->CreateVideoPlayer() );
	pVideoPlayerCompile->SetVideo( pVideoCompile );
	pVideoPlayerCompile->SetLooping( true );
	
	pCanvasView.TakeOver( canvasManager.CreateCanvasView() );
	pCanvasView->SetOrder( ( float )pOgl.GetOverlay()->GetCanvasCount() );
	pOgl.GetOverlay()->AddCanvas( pCanvasView );
	
	pCanvasVideo.TakeOver( canvasManager.CreateCanvasVideoPlayer() );
	pCanvasVideo->SetVideoPlayer( pVideoPlayerCompile );
	pCanvasVideo->SetSize( decPoint( pVideoCompile->GetWidth(), pVideoCompile->GetHeight() ) );
	pCanvasVideo->SetOrder( ( float )pCanvasView->GetCanvasCount() );
	pCanvasView->AddCanvas( pCanvasVideo );
	
	pCanvasText1.TakeOver( canvasManager.CreateCanvasText() );
	pCanvasText1->SetFont( pFontText );
	pCanvasText1->SetFontSize( ( float )pFontText->GetLineHeight() );
	pCanvasText1->SetColor( textColor );
	pCanvasText1->SetOrder( ( float )pCanvasView->GetCanvasCount() );
	pCanvasView->AddCanvas( pCanvasText1 );
	
	pCanvasText2.TakeOver( canvasManager.CreateCanvasText() );
	pCanvasText2->SetFont( pFontText );
	pCanvasText2->SetFontSize( ( float )pFontText->GetLineHeight() );
	pCanvasText2->SetColor( textColor );
	pCanvasText2->SetOrder( ( float )pCanvasView->GetCanvasCount() );
	pCanvasView->AddCanvas( pCanvasText2 );
	
	pCanvasView->SetSize( decPoint( pVideoCompile->GetWidth(), pVideoCompile->GetHeight() ) );
	
	pUpdateText();
}

void deoglShaderCompilingInfo::pUpdateChecks(){
	deoglShaderLanguage &shaderLanguage = *pOgl.GetRenderThread().GetShader().GetShaderManager().GetLanguage();
	
	pHasLoadingShader = shaderLanguage.GetHasLoadingShader();
	pHasCompilingShader = shaderLanguage.GetHasCompilingShader();
	
	if( pMode == emLoading && pHasCompilingShader ){
		pMode = emCompiling;
		pUpdateText();
	}
}

bool deoglShaderCompilingInfo::pShouldBeVisible() const{
	return pHasLoadingShader || pHasCompilingShader;
}

void deoglShaderCompilingInfo::pUpdateState( float elapsed ){
	if( pState == esInvisible ){
		if( ! pShouldBeVisible() ){
			return;
		}
		
		if( pHasCompilingShader ){
			pState = esFadeIn;
			
		}else{
			pState = esDelayFadeIn;
		}
		
		pElapsed = 0.0f;
		pFrames = 0;
		
	}else if( pState == esVisible ){
		if( pShouldBeVisible() ){
			return;
		}
		
		pState = esDelayFadeOut;
		pElapsed = 0.0f;
	}
	
	if( pState == esDelayFadeIn ){
		pFrames++;
		
		if( ! pShouldBeVisible() && pFrames > 1 ){
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
	
	const decPoint &overlaySize = pOgl.GetOverlay()->GetSize();
	const decPoint &panelSize = pCanvasView->GetSize();
	const decPoint paddingPanel( 40, 20 );
	
	pCanvasView->SetPosition( overlaySize - paddingPanel - panelSize );
	pCanvasView->SetTransparency( pTransparency * pMaxTransparency );
	pCanvasView->SetVisible( true );
	
	if( pVideoPlayerCompile->GetPlaying() ){
		// pVideoPlayerCompile->Update( elapsed );
		
	}else{
		pVideoPlayerCompile->Play();
		pVideoCompilePlayPosition = 0.0f;
	}
}

void deoglShaderCompilingInfo::pUpdateText(){
	decString text1, text2;
	
	switch( pMode ){
	case emLoading:
		text1 = "Loading";
		text2 = "Shaders";
		break;
		
	case emCompiling:
		text1 = "Compiling";
		text2 = "Shaders";
		break;
	}
	
	const int lineHeight = pFontText->GetLineHeight() * 9 / 10;
	const decPoint text1Size( pFontText->TextSize( text1 ) );
	const decPoint text2Size( pFontText->TextSize( text2 ) );
	
	const decVector2 ps( pCanvasView->GetSize() );
	const decPoint areaTL( ( int )( ps.x * 460.0f / 800.0f ), ( int )( ps.y * 168.0f / 400.0f ) );
	const decPoint areaBR( ( int )( ps.x * 770.0f / 800.0f ), ( int )( ps.y * 378.0f / 400.0f ) );
	const decPoint areaSize( areaBR - areaTL + decPoint( 1, 1 ) );
	
	const int top = areaTL.y + ( areaSize.y - lineHeight * 2 ) / 2;
	
	pCanvasText1->SetText( text1 );
	pCanvasText1->SetSize( text1Size );
	pCanvasText1->SetPosition( decPoint( areaTL.x + ( areaSize.x - text1Size.x ) / 2, top ) );
	
	pCanvasText2->SetText( text2 );
	pCanvasText2->SetSize( text2Size );
	pCanvasText2->SetPosition( decPoint( areaTL.x + ( areaSize.x - text2Size.x ) / 2, top + lineHeight ) );
}
