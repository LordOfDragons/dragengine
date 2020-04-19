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
