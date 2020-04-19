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

#include "seULayerSetTransparency.h"
#include "../../sky/layer/seLayer.h"

#include <dragengine/common/exceptions.h>



// Class seULayerSetTransparency
//////////////////////////////////

// Constructor, destructor
////////////////////////////

seULayerSetTransparency::seULayerSetTransparency( seLayer *layer, float newTransparency ) :
pLayer( NULL ),
pNewTransparency( newTransparency )
{
	if( ! layer ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Set Layer Transparency" );
	
	pOldTransparency = layer->GetTransparency();
	
	pLayer = layer;
	layer->AddReference();
}

seULayerSetTransparency::~seULayerSetTransparency(){
	if( pLayer ){
		pLayer->FreeReference();
	}
}



// Management
///////////////

void seULayerSetTransparency::SetNewTransparency( float transparency ){
	pNewTransparency = transparency;
}



void seULayerSetTransparency::Undo(){
	pLayer->SetTransparency( pOldTransparency );
}

void seULayerSetTransparency::Redo(){
	pLayer->SetTransparency( pNewTransparency );
}
