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
