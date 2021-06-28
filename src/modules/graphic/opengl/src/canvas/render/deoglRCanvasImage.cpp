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

#include "deoglRCanvasImage.h"
#include "../../texture/deoglRImage.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../rendering/deoglRenderCanvas.h"
#include "../../rendering/deoglRenderCanvasContext.h"

#include <dragengine/common/exceptions.h>



// Class deoglRCanvasImage
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRCanvasImage::deoglRCanvasImage( deoglRenderThread &renderThread ) :
deoglRCanvas( renderThread ),
pImage( NULL ),
pTCClampMin( 0.0f, 0.0f ),
pTCClampMax( 1.0f, 1.0f ){
	LEAK_CHECK_CREATE( renderThread, CanvasImage );
}

deoglRCanvasImage::~deoglRCanvasImage(){
	LEAK_CHECK_FREE( GetRenderThread(), CanvasImage );
	SetImage( NULL );
}



// Management
///////////////

void deoglRCanvasImage::SetImage( deoglRImage *image ){
	if( image == pImage ){
		return;
	}
	
	if( pImage ){
		pImage->FreeReference();
	}
	pImage = image;
	if( image ){
		image->AddReference();
	}
}

void deoglRCanvasImage::SetTCTransform( const decTexMatrix2 &transform ){
	pTCTransform = transform;
}

void deoglRCanvasImage::SetTCClampMinimum( const decVector2 &clamp ){
	pTCClampMin = clamp;
}

void deoglRCanvasImage::SetTCClampMaximum( const decVector2 &clamp ){
	pTCClampMax = clamp;
}



void deoglRCanvasImage::PrepareForRender( const deoglRenderPlanMasked *renderPlanMask ){
	if( ! pImage ){
		return;
	}
	
	deoglRCanvas::PrepareForRender( renderPlanMask );
	pImage->PrepareForRender();
}

void deoglRCanvasImage::Render( const deoglRenderCanvasContext &context ){
	if( ! pImage ){
		return;
	}
	
	deoglRenderCanvasContext imageContext( context, *this );
	imageContext.SetTCClampMinimum( pTCClampMin );
	imageContext.SetTCClampMaximum( pTCClampMax );
	GetRenderThread().GetRenderers().GetCanvas().DrawCanvasImage( imageContext, *this );
}
