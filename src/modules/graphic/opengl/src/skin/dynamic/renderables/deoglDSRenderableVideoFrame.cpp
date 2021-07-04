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

#include "deoglDSRenderableVideoFrame.h"
#include "render/deoglRDSRenderableVideoFrame.h"
#include "../deoglDynamicSkin.h"
#include "../../../video/deoglVideoPlayer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableVideoFrame.h>
#include <dragengine/resources/video/deVideoPlayer.h>



// Class deoglDSRenderableVideoFrame
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDSRenderableVideoFrame::deoglDSRenderableVideoFrame( deoglDynamicSkin &dynamicSkin,
	const deDSRenderableVideoFrame &renderable ) :
deoglDSRenderable( dynamicSkin, renderable ),
pRenderableVideoFrame( renderable ),
pRRenderableVideoFrame( NULL ),
pVideoPlayer( NULL ),
pDirty( true )
{
	try{
		pRRenderableVideoFrame = new deoglRDSRenderableVideoFrame( *dynamicSkin.GetRDynamicSkin() );
		
		if( renderable.GetVideoPlayer() ){
			pVideoPlayer = ( deoglVideoPlayer* )renderable.GetVideoPlayer()->GetPeerGraphic();
			pVideoPlayer->GetNotifyRenderables().Add( this );
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglDSRenderableVideoFrame::~deoglDSRenderableVideoFrame(){
	pCleanUp();
}



// Management
///////////////

deoglRDSRenderable *deoglDSRenderableVideoFrame::GetRRenderable() const{
	return pRRenderableVideoFrame;
}

void deoglDSRenderableVideoFrame::RenderableChanged(){
	deoglVideoPlayer * const videoPlayer = pRenderableVideoFrame.GetVideoPlayer()
		? ( deoglVideoPlayer* )pRenderableVideoFrame.GetVideoPlayer()->GetPeerGraphic() : NULL;
	
	if( videoPlayer != pVideoPlayer ){
		if( pVideoPlayer ){
			pVideoPlayer->GetNotifyRenderables().Remove( this );
		}
		
		pVideoPlayer = videoPlayer;
		
		if( videoPlayer ){
			videoPlayer->GetNotifyRenderables().Add( this );
		}
		
		pDirty = true;
		
		pDynamicSkin.NotifyRenderableChanged( *this );
	}
	
	if( pRenderableVideoFrame.GetName() != pRRenderableVideoFrame->GetName() ){
		pDynamicSkin.NotifyRenderablesChanged();
	}
}

void deoglDSRenderableVideoFrame::SyncToRender(){
	if( pVideoPlayer ){
		pVideoPlayer->SyncToRender();
	}
	
	if( pDirty ){
		pRRenderableVideoFrame->SetName( pRenderableVideoFrame.GetName() );
		
		if( pVideoPlayer ){
			pRRenderableVideoFrame->SetVideoPlayer( pVideoPlayer->GetRVideoPlayer() );
			
		}else{
			pRRenderableVideoFrame->SetVideoPlayer( NULL );
		}
		
		pDirty = false;
	}
}

void deoglDSRenderableVideoFrame::VideoPlayerRequiresSync(){
	pDynamicSkin.NotifyRenderableRequiresSync( *this );
}

void deoglDSRenderableVideoFrame::DropVideoPlayer(){
	pVideoPlayer = NULL;
}



// Private Functions
//////////////////////

void deoglDSRenderableVideoFrame::pCleanUp(){
	if( pRRenderableVideoFrame ){
		pRRenderableVideoFrame->FreeReference();
	}
	
	if( pVideoPlayer ){
		pVideoPlayer->GetNotifyRenderables().Remove( this );
	}
}
