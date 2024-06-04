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

#include "deoglEffectDistortImage.h"
#include "render/deoglREffectDistortImage.h"

#include "../deGraphicOpenGl.h"
#include "../texture/deoglImage.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/effect/deEffectDistortImage.h>
#include <dragengine/resources/image/deImage.h>



// Class deoglEffectDistortImage
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglEffectDistortImage::deoglEffectDistortImage( deGraphicOpenGl &ogl,
const deEffectDistortImage &effect ) :
deoglEffect( ogl, effect ),

pEffectDistortImage( effect ),

pREffectDistortImage( NULL ),

pDirtyStrength( true ),
pDirtyImage( true )
{
	pREffectDistortImage = new deoglREffectDistortImage( ogl.GetRenderThread() );
	InitREffect( pREffectDistortImage );
}

deoglEffectDistortImage::~deoglEffectDistortImage(){
}



// Notifications
//////////////////

void deoglEffectDistortImage::SyncToRender(){
	deoglEffect::SyncToRender();
	
	if( pDirtyImage ){
		if( pEffectDistortImage.GetImage() ){
			deoglImage * const oglImage = ( deoglImage* )pEffectDistortImage.GetImage()->GetPeerGraphic();
			oglImage->SyncToRender();
			pREffectDistortImage->SetImage( oglImage->GetRImage() );
			
		}else{
			pREffectDistortImage->SetImage( NULL );
		}
		
		pDirtyImage = false;
	}
	
	if( pDirtyStrength ){
		pREffectDistortImage->SetStrength( decVector2( pEffectDistortImage.GetStrengthU(), pEffectDistortImage.GetStrengthV() ) );
		pDirtyStrength = false;
	}
}



// Notifications
//////////////////

void deoglEffectDistortImage::ImageChanged(){
	pDirtyImage = true;
}

void deoglEffectDistortImage::StrengthChanged(){
	pDirtyStrength = true;
}
