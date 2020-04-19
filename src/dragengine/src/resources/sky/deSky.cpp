/* 
 * Drag[en]gine Game Engine
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

#include "deSky.h"
#include "deSkyManager.h"
#include "deSkyLayer.h"
#include "deSkyController.h"
#include "deSkyLink.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/graphic/deBaseGraphicSky.h"



// Class deSky
////////////////

// Constructor, destructor
////////////////////////////

deSky::deSky( deSkyManager *manager ) :
deResource( manager ),

pControllers( NULL ),
pControllerCount( 0 ),

pLinks( NULL ),
pLinkCount( 0 ),

pLayers( NULL ),
pLayerCount( 0 ),

pPeerGraphic( NULL ){
}

deSky::~deSky(){
	if( pPeerGraphic ){
		delete pPeerGraphic;
		pPeerGraphic = NULL;
	}
	
	if( pLayers ){
		delete [] pLayers;
	}
	if( pLinks ){
		delete [] pLinks;
	}
	if( pControllers ){
		delete [] pControllers;
	}
}



// Management
///////////////

void deSky::SetBgColor( const decColor &color ){
	pBgColor = color;
}



void deSky::SetControllerCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pControllers ){
		delete [] pControllers;
		pControllers = NULL;
		pControllerCount = 0;
	}
	
	if( count > 0 ){
		pControllers = new deSkyController[ count ];
		pControllerCount = count;
	}
}

deSkyController &deSky::GetControllerAt( int index ) const{
	if( index < 0 || index >= pControllerCount ){
		DETHROW( deeInvalidParam );
	}
	return pControllers[ index ];
}

int deSky::IndexOfControllerNamed( const char *name ) const{
	int i;
	for( i=0; i<pControllerCount; i++ ){
		if( pControllers[ i ].GetName() == name ){
			return i;
		}
	}
	
	return -1;
}



void deSky::SetLinkCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pLinks ){
		delete [] pLinks;
		pLinks = NULL;
		pLinkCount = 0;
	}
	
	if( count > 0 ){
		pLinks = new deSkyLink[ count ];
		pLinkCount = count;
	}
}

deSkyLink &deSky::GetLinkAt( int index ) const{
	if( index < 0 || index >= pLinkCount ){
		DETHROW( deeInvalidParam );
	}
	return pLinks[ index ];
}



void deSky::SetLayerCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pLayers ){
		delete [] pLayers;
		pLayers = NULL;
		pLayerCount = 0;
	}
	
	if( count > 0 ){
		pLayers = new deSkyLayer[ count ];
		pLayerCount = count;
	}
}

deSkyLayer &deSky::GetLayerAt( int index ) const{
	if( index < 0 || index >= pLayerCount ){
		DETHROW( deeInvalidParam );
	}
	return pLayers[ index ];
}



void deSky::NotifyParametersChanged(){
	if( pPeerGraphic ){
		pPeerGraphic->ParametersChanged();
	}
}



// System Peers
/////////////////

void deSky::SetPeerGraphic( deBaseGraphicSky *peer ){
	if( peer == pPeerGraphic ){
		return;
	}
	
	if( pPeerGraphic ){
		delete pPeerGraphic;
	}
	pPeerGraphic = peer;
}
