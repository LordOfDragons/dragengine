/* 
 * Drag[en]gine Game Engine
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

#include "deCanvasVideoPlayer.h"
#include "deCanvasVisitor.h"
#include "../video/deVideoPlayer.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/graphic/deBaseGraphicCanvas.h"



// Class deCanvasVideoPlayer
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deCanvasVideoPlayer::deCanvasVideoPlayer( deCanvasManager *manager ) :
deCanvas( manager ),
pRepeatX( 1 ),
pRepeatY( 1 ){
}

deCanvasVideoPlayer::~deCanvasVideoPlayer(){
}



// Management
///////////////

void deCanvasVideoPlayer::SetVideoPlayer( deVideoPlayer *videoPlayer ){
	if( videoPlayer == pVideoPlayer ){
		return;
	}
	
	pVideoPlayer = videoPlayer;
	NotifyContentChanged();
}

void deCanvasVideoPlayer::SetRepeatX( int count ){
	if( count < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count == pRepeatX ){
		return;
	}
	
	pRepeatX = count;
	NotifyContentChanged();
}

void deCanvasVideoPlayer::SetRepeatY( int count ){
	if( count < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count == pRepeatY ){
		return;
	}
	
	pRepeatY = count;
	NotifyContentChanged();
}



// Visiting
/////////////

void deCanvasVideoPlayer::Visit( deCanvasVisitor &visitor ){
	visitor.VisitVideoPlayer( *this );
}
