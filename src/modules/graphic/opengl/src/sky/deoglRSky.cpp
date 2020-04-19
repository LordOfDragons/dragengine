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

#include "deoglRSky.h"
#include "deoglRSkyLink.h"
#include "deoglRSkyLayer.h"
#include "../configuration/deoglConfiguration.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../deGraphicOpenGl.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyLayer.h>



// Class deoglRSky
////////////////////

// Constructor, destructor
////////////////////////////

deoglRSky::deoglRSky( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pLinks( NULL ),
pLinkCount( 0 ),
pLayers( NULL ),
pLayerCount( 0 )
{
	LEAK_CHECK_CREATE( renderThread, Sky );
}

deoglRSky::~deoglRSky(){
	if( pLayers ){
		while( pLayerCount > 0 ){
			delete pLayers[ --pLayerCount ];
		}
		delete [] pLayers;
	}
	
	if( pLinks ){
		while( pLinkCount > 0 ){
			delete pLinks[ --pLinkCount ];
		}
		delete [] pLinks;
	}
	
	LEAK_CHECK_FREE( pRenderThread, Sky );
}



// Management
///////////////

void deoglRSky::SetBgColor( const decColor &color ){
	pBgColor = color;
}



const deoglRSkyLink &deoglRSky::GetLinkAt( int index ) const{
	if( index < 0 || index >= pLinkCount ){
		DETHROW( deeInvalidParam );
	}
	return *pLinks[ index ];
}

void deoglRSky::RebuildLinks( const deSky &sky ){
	if( pLinks ){
		while( pLinkCount > 0 ){
			delete pLinks[ --pLinkCount ];
		}
		delete [] pLinks;
		pLinks = NULL;
	}
	
	const int linkCount = sky.GetLinkCount();
	if( linkCount == 0 ){
		return;
	}
	
	pLinks = new deoglRSkyLink*[ linkCount ];
	for( pLinkCount=0; pLinkCount<linkCount; pLinkCount++ ){
		pLinks[ pLinkCount ] = new deoglRSkyLink( sky.GetLinkAt( pLinkCount ) );
	}
}



deoglRSkyLayer &deoglRSky::GetLayerAt( int index ) const{
	if( index < 0 || index >= pLayerCount ){
		DETHROW( deeInvalidParam );
	}
	return *pLayers[ index ];
}

void deoglRSky::RebuildLayers( const deSky &sky ){
	if( pLayers ){
		while( pLayerCount > 0 ){
			delete pLayers[ --pLayerCount ];
		}
		delete [] pLayers;
		pLayers = NULL;
	}
	
	const int layerCount = sky.GetLayerCount();
	if( layerCount == 0 ){
		return;
	}
	
	pLayers = new deoglRSkyLayer*[ layerCount ];
	for( pLayerCount=0; pLayerCount<layerCount; pLayerCount++ ){
		pLayers[ pLayerCount ] = new deoglRSkyLayer( sky.GetLayerAt( pLayerCount ) );
	}
}
