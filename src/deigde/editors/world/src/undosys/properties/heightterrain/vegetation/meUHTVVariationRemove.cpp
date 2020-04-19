/* 
 * Drag[en]gine IGDE World Editor
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

#include "meUHTVVariationRemove.h"
#include "../../../../world/heightterrain/meHTVegetationLayer.h"
#include "../../../../world/heightterrain/meHTVVariation.h"

#include <dragengine/common/exceptions.h>



// Class meUHTVVariationRemove
////////////////////////////////

// Constructor, destructor
////////////////////////////

meUHTVVariationRemove::meUHTVVariationRemove( meHTVegetationLayer *vlayer, meHTVVariation *variation ){
	if( ! vlayer || ! variation ) DETHROW( deeInvalidParam );
	
	pVLayer = vlayer;
	pVariation = variation;
	
	pIndex = vlayer->IndexOfVariation( variation );
	if( pIndex == -1 ) DETHROW( deeInvalidParam );
	
	SetShortInfo( "Remove Vegetation Layer Variation" );
	
	vlayer->AddReference();
	variation->AddReference();
}

meUHTVVariationRemove::~meUHTVVariationRemove(){
	if( pVariation ) pVariation->FreeReference();
	if( pVLayer ) pVLayer->FreeReference();
}



// Management
///////////////

void meUHTVVariationRemove::Undo(){
	pVLayer->InsertVariation( pIndex, pVariation );
}

void meUHTVVariationRemove::Redo(){
	pVLayer->RemoveVariation( pVariation );
}
