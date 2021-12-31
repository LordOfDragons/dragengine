/* 
 * Drag[en]gine Shared Vulkan
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include <stdlib.h>
#include <string.h>

#include "devkFramebufferConfiguration.h"

#include <dragengine/common/exceptions.h>


// class devkFramebufferConfiguration
//////////////////////////////////////

devkFramebufferConfiguration::devkFramebufferConfiguration() :
pAttachmentCount( 0 ),
pAttachments( nullptr ),
pSize( 1, 1 ),
pLayerCount( 1 ){
}

devkFramebufferConfiguration::devkFramebufferConfiguration( const devkFramebufferConfiguration &configuration ) :
pAttachmentCount( 0 ),
pAttachments( nullptr ),
pSize( 1, 1 ),
pLayerCount( 1 )
{
	*this = configuration;
}

devkFramebufferConfiguration::~devkFramebufferConfiguration(){
	if( pAttachments ){
		delete [] pAttachments;
	}
}



// Management
///////////////

void devkFramebufferConfiguration::SetAttachmentCount( int count ){
	if( count < 0 ){
		DETHROW_INFO( deeInvalidParam, "count < 0" );
	}
	
	if( pAttachments ){
		delete [] pAttachments;
		pAttachments = nullptr;
		pAttachmentCount = 0;
	}
	
	if( count == 0 ){
		return;
	}
	
	pAttachments = new devkImageView::Ref[ count ];
	pAttachmentCount = count;
}

devkImageView *devkFramebufferConfiguration::GetAttachmentAt( int index ) const{
	if( index < 0 ){
		DETHROW_INFO( deeInvalidParam, "index < 0" );
	}
	if( index >= pAttachmentCount ){
		DETHROW_INFO( deeInvalidParam, "index >= attachmentCount" );
	}
	
	return pAttachments[ index ];
}

void devkFramebufferConfiguration::SetAttachmentAt( int index, devkImageView *attachment ){
	if( index < 0 ){
		DETHROW_INFO( deeInvalidParam, "index < 0" );
	}
	if( index >= pAttachmentCount ){
		DETHROW_INFO( deeInvalidParam, "index >= attachmentCount" );
	}
	
	pAttachments[ index ] = attachment;
}

void devkFramebufferConfiguration::SetSize( const decPoint &size ){
	if( ! ( size >= decPoint( 1, 1 ) ) ){
		DETHROW_INFO( deeInvalidParam, "!(size >= (1,1))" );
	}
	
	pSize = size;
}

void devkFramebufferConfiguration::SetLayerCount( int count ){
	if( count < 1 ){
		DETHROW_INFO( deeInvalidParam, "count < " );
	}
	
	pLayerCount = count;
}



// Operators
//////////////

bool devkFramebufferConfiguration::operator==( const devkFramebufferConfiguration &configuration ) const{
	if( pAttachmentCount != configuration.pAttachmentCount
	|| pSize != configuration.pSize
	|| pLayerCount != configuration.pLayerCount ){
		return false;
	}
	
	int i;
	for( i=0; i<pAttachmentCount; i++ ){
		if( pAttachments[ i ] != configuration.pAttachments[ i ] ){
			return false;
		}
	}
	
	return true;
}

devkFramebufferConfiguration &devkFramebufferConfiguration::operator=( const devkFramebufferConfiguration &configuration ){
	SetAttachmentCount( configuration.pAttachmentCount );
	int i;
	for( i=0; i<pAttachmentCount; i++ ){
		pAttachments[ i ] = configuration.pAttachments[ i ];
	}
	
	pSize = configuration.pSize;
	pLayerCount = configuration.pLayerCount;
	return *this;
}
