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
