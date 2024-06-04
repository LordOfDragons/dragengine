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
