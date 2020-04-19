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

#include "deoglRDSRenderableImage.h"
#include "../../deoglRDynamicSkin.h"
#include "../../../../renderthread/deoglRenderThread.h"
#include "../../../../texture/deoglRImage.h"

#include <dragengine/common/exceptions.h>



// Class deoglRDSRenderableImage
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRDSRenderableImage::deoglRDSRenderableImage( deoglRDynamicSkin &dynamicSkin ) :
deoglRDSRenderable( dynamicSkin ),
pImage( NULL )
{
	LEAK_CHECK_CREATE( dynamicSkin.GetRenderThread(), DSRenderableImage );
}

deoglRDSRenderableImage::~deoglRDSRenderableImage(){
	LEAK_CHECK_FREE( GetDynamicSkin().GetRenderThread(), DSRenderableImage );
	pCleanUp();
}



// Management
///////////////

void deoglRDSRenderableImage::SetImage( deoglRImage *image ){
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
	
	GetDynamicSkin().TextureConfigurationChanged();
}

void deoglRDSRenderableImage::PrepareForRender(){
	if( pImage ){
		pImage->PrepareForRender();
	}
}

deoglTexture *deoglRDSRenderableImage::GetRenderTexture(){
	if( ! pImage ){
		return NULL;
	}
	return pImage->GetTexture();
}



// Private Functions
//////////////////////

void deoglRDSRenderableImage::pCleanUp(){
	SetImage( NULL );
}
