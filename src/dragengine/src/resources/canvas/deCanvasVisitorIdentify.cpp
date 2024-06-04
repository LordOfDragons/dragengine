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
