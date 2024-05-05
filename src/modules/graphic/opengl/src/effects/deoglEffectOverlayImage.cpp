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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglEffectOverlayImage.h"
#include "render/deoglREffectOverlayImage.h"

#include "../deGraphicOpenGl.h"
#include "../texture/deoglImage.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/effect/deEffectOverlayImage.h>
#include <dragengine/resources/image/deImage.h>



// Class deoglEffectOverlayImage
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglEffectOverlayImage::deoglEffectOverlayImage( deGraphicOpenGl &ogl,
const deEffectOverlayImage &effect ) :
deoglEffect( ogl, effect ),

pEffectOverlayImage( effect ),

pREffectOverlayImage( NULL ),

pDirtyTransparency( true ),
pDirtyImage( true )
{
	pREffectOverlayImage = new deoglREffectOverlayImage( ogl.GetRenderThread() );
	InitREffect( pREffectOverlayImage );
}

deoglEffectOverlayImage::~deoglEffectOverlayImage(){
}



// Notifications
//////////////////

void deoglEffectOverlayImage::SyncToRender(){
	deoglEffect::SyncToRender();
	
	if( pDirtyImage ){
		if( pEffectOverlayImage.GetImage() ){
			deoglImage * const oglImage = ( deoglImage* )pEffectOverlayImage.GetImage()->GetPeerGraphic();
			oglImage->SyncToRender();
			pREffectOverlayImage->SetImage( oglImage->GetRImage() );
			
		}else{
			pREffectOverlayImage->SetImage( NULL );
		}
		
		pDirtyImage = false;
	}
	
	if( pDirtyTransparency ){
		pREffectOverlayImage->SetTransparency( pEffectOverlayImage.GetTransparency() );
		pDirtyTransparency = false;
	}
}



// Notifications
//////////////////

void deoglEffectOverlayImage::ImageChanged(){
	pDirtyImage = true;
}

void deoglEffectOverlayImage::TransparencyChanged(){
	pDirtyTransparency = true;
}
