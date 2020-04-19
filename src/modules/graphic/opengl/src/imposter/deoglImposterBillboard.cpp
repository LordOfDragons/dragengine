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

#include "deoglImposterBillboard.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../texture/arraytexture/deoglArrayTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglImposterBillboard
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglImposterBillboard::deoglImposterBillboard( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pChannelTextures( NULL )
{
	int i;
	pChannelTextures = new deoglArrayTexture*[ deoglSkinChannel::CHANNEL_COUNT ];
	for( i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++ ){
		pChannelTextures[ i ] = NULL;
	}
}

deoglImposterBillboard::~deoglImposterBillboard(){
	if( pChannelTextures ){
		int i;
		
		for( i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++ ){
			if( pChannelTextures[ i ] ){
				delete pChannelTextures[ i ];
			}
		}
		
		delete [] pChannelTextures;
	}
}



// Management
///////////////

deoglArrayTexture *deoglImposterBillboard::GetChannelTextureAt( int channel ) const{
	if( channel < 0 || channel >= deoglSkinChannel::CHANNEL_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	return pChannelTextures[ channel ];
}

void deoglImposterBillboard::EnableChannelTexture( int channel, bool enable ){
	if( channel < 0 || channel >= deoglSkinChannel::CHANNEL_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	if( enable ){
		if( pChannelTextures[ channel ] ){
			return;
		}
		
		pChannelTextures[ channel ] = new deoglArrayTexture( pRenderThread );
		
	}else{
		if( ! pChannelTextures[ channel ] ){
			return;
		}
		
		delete pChannelTextures[ channel ];
		pChannelTextures[ channel ] = NULL;
	}
}

bool deoglImposterBillboard::IsChannelTextureEnabled( int channel ) const{
	if( channel < 0 || channel >= deoglSkinChannel::CHANNEL_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	return pChannelTextures[ channel ] != NULL;
}



void deoglImposterBillboard::SetExtends( const decVector2 &minExtend, const decVector2 &maxExtend ){
	pMinExtend = minExtend;
	pMaxExtend = maxExtend;
}
