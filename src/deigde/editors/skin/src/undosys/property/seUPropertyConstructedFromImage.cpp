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
#include <string.h>
#include <stdlib.h>

#include "seUPropertyConstructedFromImage.h"
#include "../../skin/property/node/sePropertyNodeImage.h"
#include "../../skin/property/node/sePropertyNodeGroup.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>



// Class seUPropertyConstructedFromImage
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

seUPropertyConstructedFromImage::seUPropertyConstructedFromImage( seProperty *property ) :
pProperty( NULL ),
pOldContent( NULL ),
pNewContent( NULL )
{
	if( ! property ){
		DETHROW( deeInvalidParam );
	}
	
	const deImage * const image = property->GetEngineImage();
	if( ! image ){
		DETHROW( deeInvalidParam );
	}
	
	const decPoint3 size( image->GetWidth(), image->GetHeight(), image->GetDepth() );
	
	SetShortInfo( "Property constructed from image" );
	
	sePropertyNodeImage *nodeImage = NULL;
	
	try{
		nodeImage = new sePropertyNodeImage( *property->GetEngine() );
		nodeImage->SetSize( size );
		nodeImage->SetPath( property->GetImagePath() );
		
		pNewContent = new sePropertyNodeGroup( *property->GetEngine() );
		pNewContent->SetSize( size );
		pNewContent->AddNode( nodeImage );
		nodeImage->FreeReference();
		nodeImage = NULL;
		
	}catch( const deException & ){
		if( nodeImage ){
			nodeImage->FreeReference();
		}
		pCleanUp();
		throw;
	}
	
	pOldContent = property->GetNodeGroup();
	pOldContent->AddReference();
	
	pProperty = property;
	property->AddReference();
}

seUPropertyConstructedFromImage::~seUPropertyConstructedFromImage(){
	pCleanUp();
}



// Management
///////////////

void seUPropertyConstructedFromImage::Undo(){
	pProperty->SetNodeGroup( pOldContent );
}

void seUPropertyConstructedFromImage::Redo(){
	pProperty->SetNodeGroup( pNewContent );
}



// Private Functions
//////////////////////

void seUPropertyConstructedFromImage::pCleanUp(){
	if( pNewContent ){
		pNewContent->FreeReference();
	}
	if( pOldContent ){
		pOldContent->FreeReference();
	}
	if( pProperty ){
		pProperty->FreeReference();
	}
}
