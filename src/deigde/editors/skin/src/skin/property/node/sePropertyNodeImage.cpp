/* 
 * Drag[en]gine IGDE Skin Editor
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

#include "sePropertyNodeImage.h"
#include "../seProperty.h"
#include "../../seSkin.h"
#include "../../texture/seTexture.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>



// Class sePropertyNodeImage
//////////////////////////////

// Constructor, destructor
////////////////////////////

sePropertyNodeImage::sePropertyNodeImage( deEngine &engine ) :
sePropertyNode( entImage, engine ),
pImage( NULL ),
pRepeat( 1, 1 ){
}

sePropertyNodeImage::sePropertyNodeImage( const sePropertyNodeImage &node ) :
sePropertyNode( node ),
pPath( node.pPath ),
pImage( NULL ),
pRepeat( node.pRepeat )
{
	pImage = node.pImage;
	if( pImage ){
		pImage->AddReference();
	}
}

sePropertyNodeImage::~sePropertyNodeImage(){
	if( pImage ){
		pImage->FreeReference();
	}
}



// Management
///////////////

void sePropertyNodeImage::SetPath( const char *path ){
	if( pPath.Equals( path ) ){
		return;
	}
	
	pPath = path;
	UpdateImage();
	NotifyChanged();
}

void sePropertyNodeImage::UpdateImage(){
	deImage *image = NULL;
	
	if( ! pPath.IsEmpty() && GetProperty() && GetProperty()->GetTexture() && GetProperty()->GetTexture()->GetSkin() ){
		const decString &basePath = GetProperty()->GetTexture()->GetSkin()->GetDirectoryPath();
		
		try{
			image = GetEngine().GetImageManager()->LoadImage( pPath, basePath );
			
		}catch( const deException &e ){
			GetProperty()->GetTexture()->GetSkin()->GetLogger()->LogException( "Skin Editor", e );
		}
	}
	
	if( image == pImage ){
		if( image ){
			image->FreeReference();
		}
		return;
	}
	
	if( pImage ){
		pImage->FreeReference();
	}
	pImage = image;
}

void sePropertyNodeImage::SetRepeat( const decPoint &count ){
	if( ! ( count >= decPoint( 1, 1 ) ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( count == pRepeat ){
		return;
	}
	
	pRepeat = count;
	
	NotifyChanged();
}



sePropertyNode *sePropertyNodeImage::Copy() const{
	return new sePropertyNodeImage( *this );
}

void sePropertyNodeImage::UpdateResources(){
	sePropertyNode::UpdateResources();
	UpdateImage();
}
