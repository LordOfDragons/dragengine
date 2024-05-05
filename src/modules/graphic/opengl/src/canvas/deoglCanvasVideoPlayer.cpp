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

#include "deoglCanvasVideoPlayer.h"
#include "render/deoglRCanvasVideoPlayer.h"
#include "../deGraphicOpenGl.h"
#include "../video/deoglRVideoPlayer.h"
#include "../video/deoglVideoPlayer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvas.h>
#include <dragengine/resources/canvas/deCanvasVideoPlayer.h>
#include <dragengine/resources/video/deVideoPlayer.h>
#include <dragengine/resources/video/deVideo.h>



// Class deoglCanvasVideoPlayer
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCanvasVideoPlayer::deoglCanvasVideoPlayer( deGraphicOpenGl &ogl, deCanvasVideoPlayer &canvas ) :
deoglCanvas( ogl, canvas ),
pCanvasVideoPlayer( canvas ),
pRCanvasVideoPlayer( NULL ),
pVideoPlayer( NULL ),
pDirty( true ){
}

deoglCanvasVideoPlayer::~deoglCanvasVideoPlayer(){
	if( pVideoPlayer ){
		pVideoPlayer->GetNotifyCanvas().Remove( this );
	}
}



// Management
///////////////

void deoglCanvasVideoPlayer::DropRCanvas(){
	pRCanvasVideoPlayer = NULL;
	deoglCanvas::DropRCanvas();
}

void deoglCanvasVideoPlayer::SyncContentToRender(){
	if( pVideoPlayer ){
		pVideoPlayer->SyncToRender();
	}
	
	if( pDirty ){
		pDirty = false;
		
		if( pVideoPlayer ){
			pRCanvasVideoPlayer->SetVideoPlayer( pVideoPlayer->GetRVideoPlayer() );
			
		}else{
			pRCanvasVideoPlayer->SetVideoPlayer( NULL );
		}
		
		const float repeatScaleU = ( float )pCanvasVideoPlayer.GetRepeatX();
		const float repeatScaleV = ( float )pCanvasVideoPlayer.GetRepeatY();
		pRCanvasVideoPlayer->SetTCTransform( decTexMatrix2::CreateScale( repeatScaleU, repeatScaleV ) );
		pRCanvasVideoPlayer->SetTCClampMaximum( decVector2( repeatScaleU, repeatScaleV ) );
	}
}

void deoglCanvasVideoPlayer::VideoPlayerRequiresSync(){
	deoglCanvas::ContentChanged();
}

void deoglCanvasVideoPlayer::DropVideoPlayer(){
	pVideoPlayer = NULL;
}



void deoglCanvasVideoPlayer::ContentChanged(){
	deoglCanvas::ContentChanged();
	
	if( pVideoPlayer ){
		pVideoPlayer->GetNotifyCanvas().Remove( this );
	}
	
	if( pCanvasVideoPlayer.GetVideoPlayer() ){
		pVideoPlayer = ( deoglVideoPlayer* )pCanvasVideoPlayer.GetVideoPlayer()->GetPeerGraphic();
		pVideoPlayer->GetNotifyCanvas().Add( this );
		
	}else{
		pVideoPlayer = NULL;
	}
	
	pDirty = true;
}



// Protected Functions
////////////////////////

deoglRCanvas *deoglCanvasVideoPlayer::CreateRCanvas(){
	pRCanvasVideoPlayer = new deoglRCanvasVideoPlayer( GetOgl().GetRenderThread() );
	return pRCanvasVideoPlayer;
}
