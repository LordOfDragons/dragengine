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
