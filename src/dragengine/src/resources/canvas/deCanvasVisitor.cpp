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
#include "deCanvasVisitor.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"



// Class deCanvasVisitor
//////////////////////////

// Constructor, destructor
////////////////////////////

deCanvasVisitor::deCanvasVisitor(){
}

deCanvasVisitor::~deCanvasVisitor(){
}



// Visiting
/////////////

void deCanvasVisitor::VisitCanvas( deCanvas &canvas ){
}

void deCanvasVisitor::VisitImage( deCanvasImage &canvas ){
	VisitCanvas( canvas );
}

void deCanvasVisitor::VisitPaint( deCanvasPaint &canvas ){
	VisitCanvas( canvas );
}

void deCanvasVisitor::VisitCanvasView( deCanvasCanvasView &canvas ){
	VisitCanvas( canvas );
}

void deCanvasVisitor::VisitRenderWorld( deCanvasRenderWorld &canvas ){
	VisitCanvas( canvas );
}

void deCanvasVisitor::VisitText( deCanvasText &canvas ){
	VisitCanvas( canvas );
}

void deCanvasVisitor::VisitVideoPlayer( deCanvasVideoPlayer &canvas ){
	VisitCanvas( canvas );
}

void deCanvasVisitor::VisitView( deCanvasView &canvas ){
	VisitCanvas( canvas );
}
