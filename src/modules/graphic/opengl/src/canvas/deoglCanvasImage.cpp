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
#include <string.h>

#include "deoglCanvasImage.h"
#include "render/deoglRCanvasImage.h"
#include "../deGraphicOpenGl.h"
#include "../texture/deoglImage.h"
#include "../texture/deoglRImage.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvas.h>
#include <dragengine/resources/canvas/deCanvasImage.h>
#include <dragengine/resources/image/deImage.h>



// Class deoglCanvasImage
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglCanvasImage::deoglCanvasImage( deGraphicOpenGl &ogl, deCanvasImage &canvas ) :
deoglCanvas( ogl, canvas ),
pCanvasImage( canvas ),
pRCanvasImage( NULL ),
pImage( NULL ),
pDirty( true ){
}

deoglCanvasImage::~deoglCanvasImage(){
	if( pImage ){
		pImage->GetNotifyCanvas().Remove( this );
	}
}



// Management
///////////////

void deoglCanvasImage::DropRCanvas(){
	pRCanvasImage = NULL;
	deoglCanvas::DropRCanvas();
}

void deoglCanvasImage::SyncContentToRender(){
	if( pImage ){
		pImage->SyncToRender();
	}
	
	if( pDirty ){
		pDirty = false;
		
		decVector2 halfPixel( 0.0f, 0.0f );
		
		if( pImage ){
			deoglRImage * const rimage = pImage->GetRImage();
			pRCanvasImage->SetImage( rimage );
			halfPixel.x = 0.5f / ( float )rimage->GetWidth();
			halfPixel.y = 0.5f / ( float )rimage->GetHeight();
			
		}else{
			pRCanvasImage->SetImage( NULL );
		}
		
		const float repeatScaleU = ( float )pCanvasImage.GetRepeatX();
		const float repeatScaleV = ( float )pCanvasImage.GetRepeatY();
		pRCanvasImage->SetTCTransform( decTexMatrix2::CreateScale( repeatScaleU, repeatScaleV ) );
		pRCanvasImage->SetTCClampMinimum( halfPixel );
		pRCanvasImage->SetTCClampMaximum( decVector2( repeatScaleU, repeatScaleV ) - halfPixel );
	}
}

void deoglCanvasImage::ImageRequiresSync(){
	deoglCanvas::ContentChanged();
}

void deoglCanvasImage::DropImage(){
	pImage = NULL;
}



void deoglCanvasImage::ContentChanged(){
	deoglCanvas::ContentChanged();
	
	if( pImage ){
		pImage->GetNotifyCanvas().Remove( this );
	}
	
	if( pCanvasImage.GetImage() ){
		pImage = ( deoglImage* )pCanvasImage.GetImage()->GetPeerGraphic();
		if( pImage ){
			pImage->GetNotifyCanvas().Add( this );
		}
		
	}else{
		pImage = NULL;
	}
	
	pDirty = true;
}



// Protected Functions
////////////////////////

deoglRCanvas *deoglCanvasImage::CreateRCanvas(){
	pRCanvasImage = new deoglRCanvasImage( GetOgl().GetRenderThread() );
	return pRCanvasImage;
}
