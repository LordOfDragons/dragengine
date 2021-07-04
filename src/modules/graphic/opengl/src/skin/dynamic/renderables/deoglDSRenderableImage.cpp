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

#include "deoglDSRenderableImage.h"
#include "render/deoglRDSRenderableImage.h"
#include "../deoglDynamicSkin.h"
#include "../../../texture/deoglImage.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableImage.h>



// Class deoglDSRenderableImage
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDSRenderableImage::deoglDSRenderableImage( deoglDynamicSkin &dynamicSkin, const deDSRenderableImage &renderable ) :
deoglDSRenderable( dynamicSkin, renderable ),
pRenderableImage( renderable ),
pRRenderableImage( NULL ),
pImage( NULL ),
pDirty( true )
{
	try{
		pRRenderableImage = new deoglRDSRenderableImage( *dynamicSkin.GetRDynamicSkin() );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglDSRenderableImage::~deoglDSRenderableImage(){
	pCleanUp();
}



// Management
///////////////

deoglRDSRenderable *deoglDSRenderableImage::GetRRenderable() const{
	return pRRenderableImage;
}

void deoglDSRenderableImage::RenderableChanged(){
	deoglImage * const image = pRenderableImage.GetImage()
		? ( deoglImage* )pRenderableImage.GetImage()->GetPeerGraphic() : NULL;
	
	if( image != pImage ){
		pImage = image;
		pDirty = true;
		
		pDynamicSkin.NotifyRenderableChanged( *this );
	}
	
	if( pRenderableImage.GetName() != pRRenderableImage->GetName() ){
		pDynamicSkin.NotifyRenderablesChanged();
	}
}

void deoglDSRenderableImage::SyncToRender(){
	if( pDirty ){
		pRRenderableImage->SetName( pRenderableImage.GetName() );
		
		if( pRenderableImage.GetImage() ){
			deoglImage &image = *( ( deoglImage* )pRenderableImage.GetImage()->GetPeerGraphic() );
			image.SyncToRender();
			pRRenderableImage->SetImage( image.GetRImage() );
			
		}else{
			pRRenderableImage->SetImage( NULL );
		}
		
		pDirty = false;
	}
}



// Private Functions
//////////////////////

void deoglDSRenderableImage::pCleanUp(){
	if( pRRenderableImage ){
		pRRenderableImage->FreeReference();
	}
}
