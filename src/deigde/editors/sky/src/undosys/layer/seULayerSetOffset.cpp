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

#include "seULayerSetOffset.h"
#include "../../sky/layer/seLayer.h"

#include <dragengine/common/exceptions.h>



// Class seULayerSetOffset
////////////////////////////

// Constructor, destructor
////////////////////////////

seULayerSetOffset::seULayerSetOffset( seLayer *layer, const decVector &newOffset ) :
pLayer( NULL ),
pNewOffset( newOffset )
{
	if( ! layer ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Set Layer Offset" );
	
	pOldOffset = layer->GetOffset();
	
	pLayer = layer;
	layer->AddReference();
}

seULayerSetOffset::~seULayerSetOffset(){
	if( pLayer ){
		pLayer->FreeReference();
	}
}



// Management
///////////////

void seULayerSetOffset::Undo(){
	pLayer->SetOffset( pOldOffset );
}

void seULayerSetOffset::Redo(){
	pLayer->SetOffset( pNewOffset );
}
