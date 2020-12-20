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

#include "deoglRCanvasVideoPlayer.h"
#include "../../video/deoglRVideoPlayer.h"
#include "../../rendering/deoglRenderCanvas.h"
#include "../../rendering/deoglRenderCanvasContext.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTRenderers.h"

#include <dragengine/common/exceptions.h>



// Class deoglRCanvasVideoPlayer
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRCanvasVideoPlayer::deoglRCanvasVideoPlayer( deoglRenderThread &renderThread ) :
deoglRCanvas( renderThread ),
pVideoPlayer( NULL ),
pTCClampMin( 0.0f, 0.0f ),
pTCClampMax( 1.0f, 1.0f ){
	LEAK_CHECK_CREATE( renderThread, CanvasVideoPlayer );
}

deoglRCanvasVideoPlayer::~deoglRCanvasVideoPlayer(){
	LEAK_CHECK_FREE( GetRenderThread(), CanvasVideoPlayer );
	SetVideoPlayer( NULL );
}



// Management
///////////////

void deoglRCanvasVideoPlayer::SetVideoPlayer( deoglRVideoPlayer *videoPlayer ){
	if( videoPlayer == pVideoPlayer ){
		return;
	}
	
	if( pVideoPlayer ){
		pVideoPlayer->FreeReference();
	}
	pVideoPlayer = videoPlayer;
	if( videoPlayer ){
		videoPlayer->AddReference();
	}
}

void deoglRCanvasVideoPlayer::SetTCTransform( const decTexMatrix2 &transform ){
	pTCTransform = transform;
}

void deoglRCanvasVideoPlayer::SetTCClampMinimum( const decVector2 &clamp ){
	pTCClampMin = clamp;
}

void deoglRCanvasVideoPlayer::SetTCClampMaximum( const decVector2 &clamp ){
	pTCClampMax = clamp;
}



void deoglRCanvasVideoPlayer::PrepareForRender(){
	if( ! pVideoPlayer ){
		return;
	}
	
	deoglRCanvas::PrepareForRender();
	pVideoPlayer->UpdateTexture();
}

void deoglRCanvasVideoPlayer::Render( const deoglRenderCanvasContext &context ){
	if( ! pVideoPlayer ){
		return;
	}
	
	deoglRenderCanvasContext videoPlayerContext( context, *this );
	videoPlayerContext.SetTCClampMinimum( pTCClampMin );
	videoPlayerContext.SetTCClampMaximum( pTCClampMax );
	GetRenderThread().GetRenderers().GetCanvas().DrawCanvasVideoPlayer( videoPlayerContext, *this );
}
