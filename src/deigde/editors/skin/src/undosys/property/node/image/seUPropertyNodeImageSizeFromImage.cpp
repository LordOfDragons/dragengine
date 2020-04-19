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

#include "seUPropertyNodeImageSizeFromImage.h"
#include "../../../../skin/property/node/sePropertyNodeImage.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>



// Class seUPropertyNodeImageSizeFromImage
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

seUPropertyNodeImageSizeFromImage::seUPropertyNodeImageSizeFromImage( sePropertyNodeImage *node ) :
seUPropertyNodeSetSize( node, GetSizeFromImage( node ) )
{
	SetShortInfo( "Node size from image size" );
}



// Management
///////////////

decPoint3 seUPropertyNodeImageSizeFromImage::GetSizeFromImage( const sePropertyNodeImage *node ){
	if( ! node ){
		DETHROW( deeInvalidParam );
	}
	
	const deImage * const image = node->GetImage();
	if( ! image ){
		DETHROW( deeInvalidParam );
	}
	
	return decPoint3( image->GetWidth(), image->GetHeight(), image->GetDepth() );
}
