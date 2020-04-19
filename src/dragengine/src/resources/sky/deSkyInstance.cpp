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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deSky.h"
#include "deSkyInstance.h"
#include "deSkyInstanceManager.h"
#include "deSkyController.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/graphic/deBaseGraphicSkyInstance.h"



// Class deSkyInstance
////////////////////////

// Constructor, destructor
////////////////////////////

deSkyInstance::deSkyInstance( deSkyInstanceManager *manager ) :
deResource( manager ),
pOrder( 0 ),

pControllers( NULL ),
pControllerCount( 0 ),

pPeerGraphic( NULL ),

pParentWorld( NULL ),
pLLWorldPrev( NULL ),
pLLWorldNext( NULL ){
}

deSkyInstance::~deSkyInstance(){
	if( pPeerGraphic ){
		delete pPeerGraphic;
		pPeerGraphic = NULL;
	}
	
	if( pControllers ){
		delete [] pControllers;
	}
}



// Management
///////////////

void deSkyInstance::SetSky( deSky *sky ){
	if( pSky == sky ){
		return;
	}
	
	if( pControllers ){
		delete [] pControllers;
		pControllers = NULL;
		pControllerCount = 0;
	}
	
	pSky = sky;
	
	if( sky ){
		const int controllerCount = sky->GetControllerCount();
		if( controllerCount > 0 ){
			pControllers = new deSkyController[ controllerCount ];
			
			int i;
			for( i=0; i<controllerCount; i++ ){
				pControllers[ i ] = sky->GetControllerAt( i );
			}
			pControllerCount = controllerCount;
		}
	}
	
	if( pPeerGraphic ){
		pPeerGraphic->SkyChanged();
	}
}

void deSkyInstance::SetOrder( int order ){
	if( pOrder == order ){
		return;
	}
	
	pOrder = order;
	
	if( pPeerGraphic ){
		pPeerGraphic->OrderChanged();
	}
}



deSkyController &deSkyInstance::GetControllerAt( int index ) const{
	if( index < 0 || index >= pControllerCount ){
		DETHROW( deeInvalidParam );
	}
	return pControllers[ index ];
}

int deSkyInstance::IndexOfControllerNamed( const char *name ) const{
	int i;
	for( i=0; i<pControllerCount; i++ ){
		if( pControllers[ i ].GetName() == name ){
			return i;
		}
	}
	return -1;
}

void deSkyInstance::NotifyControllerChangedAt( int index ){
	if( index < 0 || index >= pControllerCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPeerGraphic ){
		pPeerGraphic->ControllerChanged( index );
	}
}



void deSkyInstance::SetLayerMask( const decLayerMask &layerMask ){
	if( layerMask == pLayerMask ){
		return;
	}
	
	pLayerMask = layerMask;
	
	if( pPeerGraphic ){
		pPeerGraphic->LayerMaskChanged();
	}
}



// System Peers
/////////////////

void deSkyInstance::SetPeerGraphic( deBaseGraphicSkyInstance *peer ){
	if( peer == pPeerGraphic ){
		return;
	}
	
	if( pPeerGraphic ){
		delete pPeerGraphic;
	}
	
	pPeerGraphic = peer;
}



// Linked List
////////////////

void deSkyInstance::SetParentWorld( deWorld *world ){
	pParentWorld = world;
}

void deSkyInstance::SetLLWorldPrev( deSkyInstance *sky ){
	pLLWorldPrev = sky;
}

void deSkyInstance::SetLLWorldNext( deSkyInstance *sky ){
	pLLWorldNext = sky;
}
