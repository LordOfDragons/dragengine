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

#include "deCanvas.h"
#include "deCanvasImage.h"
#include "deCanvasPaint.h"
#include "deCanvasCanvasView.h"
#include "deCanvasRenderWorld.h"
#include "deCanvasText.h"
#include "deCanvasVideoPlayer.h"
#include "deCanvasView.h"
#include "deCanvasVisitorIdentify.h"
#include "../../common/exceptions.h"



// Class deCanvasVisitorIdentify
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deCanvasVisitorIdentify::deCanvasVisitorIdentify() :
pCanvas( NULL ),
pType( deCanvasVisitorIdentify::ectUnknown ){
}

deCanvasVisitorIdentify::~deCanvasVisitorIdentify(){
}



// Management
///////////////

deCanvasImage &deCanvasVisitorIdentify::CastToImage() const{
	if( pType != ectImage ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deCanvasImage* )pCanvas );
}

deCanvasPaint &deCanvasVisitorIdentify::CastToPaint() const{
	if( pType != ectPaint ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deCanvasPaint* )pCanvas );
}

deCanvasCanvasView &deCanvasVisitorIdentify::CastToCanvasView() const{
	if( pType != ectCanvasView ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deCanvasCanvasView* )pCanvas );
}

deCanvasRenderWorld &deCanvasVisitorIdentify::CastToRenderWorld() const{
	if( pType != ectRenderWorld ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deCanvasRenderWorld* )pCanvas );
}

deCanvasText &deCanvasVisitorIdentify::CastToText() const{
	if( pType != ectText ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deCanvasText* )pCanvas );
}

deCanvasVideoPlayer &deCanvasVisitorIdentify::CastToVideoPlayer() const{
	if( pType != ectVideoPlayer ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deCanvasVideoPlayer* )pCanvas );
}

deCanvasView &deCanvasVisitorIdentify::CastToView() const{
	if( pType != ectView ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deCanvasView* )pCanvas );
}

void deCanvasVisitorIdentify::Reset(){
	pCanvas = NULL;
	pType = ectUnknown;
}



// Visiting
/////////////

void deCanvasVisitorIdentify::VisitCanvas( deCanvas &canvas ){
	pCanvas = &canvas;
	pType = ectUnknown;
}

void deCanvasVisitorIdentify::VisitImage( deCanvasImage &canvas ){
	pCanvas = &canvas;
	pType = ectImage;
}

void deCanvasVisitorIdentify::VisitPaint( deCanvasPaint &canvas ){
	pCanvas = &canvas;
	pType = ectPaint;
}

void deCanvasVisitorIdentify::VisitCanvasView( deCanvasCanvasView &canvas ){
	pCanvas = &canvas;
	pType = ectCanvasView;
}

void deCanvasVisitorIdentify::VisitRenderWorld( deCanvasRenderWorld &canvas ){
	pCanvas = &canvas;
	pType = ectRenderWorld;
}

void deCanvasVisitorIdentify::VisitText( deCanvasText &canvas ){
	pCanvas = &canvas;
	pType = ectText;
}

void deCanvasVisitorIdentify::VisitVideoPlayer( deCanvasVideoPlayer &canvas ){
	pCanvas = &canvas;
	pType = ectVideoPlayer;
}

void deCanvasVisitorIdentify::VisitView( deCanvasView &canvas ){
	pCanvas = &canvas;
	pType = ectView;
}
