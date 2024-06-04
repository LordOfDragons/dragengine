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
pTCClampMin( 0.0f, 0.0f ),
pTCClampMax( 1.0f, 1.0f ){
	LEAK_CHECK_CREATE( renderThread, CanvasImage );
}

deoglRCanvasImage::~deoglRCanvasImage(){
	LEAK_CHECK_FREE( GetRenderThread(), CanvasImage );
}



// Management
///////////////

void deoglRCanvasImage::SetImage( deoglRImage *image ){
	pImage = image;
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
