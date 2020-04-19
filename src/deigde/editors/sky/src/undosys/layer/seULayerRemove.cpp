/* 
 * Drag[en]gine IGDE Sky Editor
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

#include "seULayerRemove.h"
#include "../../sky/seSky.h"
#include "../../sky/layer/seLayer.h"

#include <dragengine/common/exceptions.h>



// Class seULayerRemove
/////////////////////////

// Constructor, destructor
////////////////////////////

seULayerRemove::seULayerRemove( seLayer *layer ) :
pSky( NULL ),
pLayer( NULL ),
pIndex( 0 )
{
	if( ! layer ){
		DETHROW( deeInvalidParam );
	}
	
	seSky * const sky = layer->GetSky();
	if( ! sky ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Remove Layer" );
	
	pIndex = sky->GetLayers().IndexOf( layer );
	if( pIndex == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pSky = sky;
	sky->AddReference();
	
	pLayer = layer;
	layer->AddReference();
}

seULayerRemove::~seULayerRemove(){
	if( pLayer ){
		pLayer->FreeReference();
	}
	if( pSky ){
		pSky->FreeReference();
	}
}



// Management
///////////////

void seULayerRemove::Undo(){
	pSky->InsertLayerAt( pLayer, pIndex );
}

void seULayerRemove::Redo(){
	pSky->RemoveLayer( pLayer );
}
