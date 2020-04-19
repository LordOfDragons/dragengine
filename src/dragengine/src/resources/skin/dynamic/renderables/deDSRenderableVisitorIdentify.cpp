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
#include "deDSRenderableValue.h"
#include "deDSRenderableColor.h"
#include "deDSRenderableImage.h"
#include "deDSRenderableCanvas.h"
#include "deDSRenderableVideoFrame.h"
#include "deDSRenderableCamera.h"
#include "deDSRenderableVisitorIdentify.h"

#include "../../../../common/exceptions.h"



// Class deDSRenderableVisitorIdentify
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deDSRenderableVisitorIdentify::deDSRenderableVisitorIdentify(){
	Reset();
}

deDSRenderableVisitorIdentify::~deDSRenderableVisitorIdentify(){
}



// Management
///////////////

deDSRenderableValue &deDSRenderableVisitorIdentify::CastToValue() const{
	if( pType != eptValue ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deDSRenderableValue* )pRenderable );
}

deDSRenderableColor &deDSRenderableVisitorIdentify::CastToColor() const{
	if( pType != eptColor ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deDSRenderableColor* )pRenderable );
}

deDSRenderableImage &deDSRenderableVisitorIdentify::CastToImage() const{
	if( pType != eptImage ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deDSRenderableImage* )pRenderable );
}

deDSRenderableCanvas &deDSRenderableVisitorIdentify::CastToCanvas() const{
	if( pType != eptCanvas ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deDSRenderableCanvas* )pRenderable );
}

deDSRenderableVideoFrame &deDSRenderableVisitorIdentify::CastToVideoFrame() const{
	if( pType != eptVideoFrame ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deDSRenderableVideoFrame* )pRenderable );
}

deDSRenderableCamera &deDSRenderableVisitorIdentify::CastToCamera() const{
	if( pType != eptCamera ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deDSRenderableCamera* )pRenderable );
}

void deDSRenderableVisitorIdentify::Reset(){
	pRenderable = NULL;
	pType = eptUnknown;
}



// Visiting
/////////////

void deDSRenderableVisitorIdentify::VisitRenderable( deDSRenderable &renderable ){
	pRenderable = &renderable;
	pType = eptUnknown;
}

void deDSRenderableVisitorIdentify::VisitValue( deDSRenderableValue &renderable ){
	pRenderable = &renderable;
	pType = eptValue;
}

void deDSRenderableVisitorIdentify::VisitColor( deDSRenderableColor &renderable ){
	pRenderable = &renderable;
	pType = eptColor;
}

void deDSRenderableVisitorIdentify::VisitImage( deDSRenderableImage &renderable ){
	pRenderable = &renderable;
	pType = eptImage;
}

void deDSRenderableVisitorIdentify::VisitCanvas( deDSRenderableCanvas &renderable ){
	pRenderable = &renderable;
	pType = eptCanvas;
}

void deDSRenderableVisitorIdentify::VisitVideoFrame( deDSRenderableVideoFrame &renderable ){
	pRenderable = &renderable;
	pType = eptVideoFrame;
}

void deDSRenderableVisitorIdentify::VisitCamera( deDSRenderableCamera &renderable ){
	pRenderable = &renderable;
	pType = eptCamera;
}
