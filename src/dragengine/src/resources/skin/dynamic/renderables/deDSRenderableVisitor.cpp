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

#include "deDSRenderable.h"
#include "deDSRenderableVisitor.h"
#include "deDSRenderableValue.h"
#include "deDSRenderableColor.h"
#include "deDSRenderableImage.h"
#include "deDSRenderableCanvas.h"
#include "deDSRenderableVideoFrame.h"
#include "deDSRenderableCamera.h"

#include "../../../../common/exceptions.h"



// Class deDSRenderableVisitor
////////////////////////////////

// Constructor, destructor
////////////////////////////

deDSRenderableVisitor::deDSRenderableVisitor(){
}

deDSRenderableVisitor::~deDSRenderableVisitor(){
}



// Visiting
/////////////

void deDSRenderableVisitor::VisitRenderable( deDSRenderable &renderable ){
}

void deDSRenderableVisitor::VisitValue( deDSRenderableValue &renderable ){
	VisitRenderable( renderable );
}

void deDSRenderableVisitor::VisitColor( deDSRenderableColor &renderable ){
	VisitRenderable( renderable );
}

void deDSRenderableVisitor::VisitImage( deDSRenderableImage &renderable ){
	VisitRenderable( renderable );
}

void deDSRenderableVisitor::VisitCanvas( deDSRenderableCanvas &renderable ){
	VisitRenderable( renderable );
}

void deDSRenderableVisitor::VisitVideoFrame( deDSRenderableVideoFrame &renderable ){
	VisitRenderable( renderable );
}

void deDSRenderableVisitor::VisitCamera( deDSRenderableCamera &renderable ){
	VisitRenderable( renderable );
}
