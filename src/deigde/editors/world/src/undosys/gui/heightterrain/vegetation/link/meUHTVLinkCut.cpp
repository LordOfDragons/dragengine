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

#include "meUHTVLinkCut.h"
#include "../../../../../world/heightterrain/meHTVegetationLayer.h"
#include "../../../../../world/heightterrain/rules/meHTVRSlot.h"
#include "../../../../../world/heightterrain/rules/meHTVRLink.h"
#include "../../../../../world/heightterrain/rules/meHTVRule.h"

#include <dragengine/common/exceptions.h>



// Class meUHTVLinkCut
////////////////////////

// Constructor, destructor
////////////////////////////

meUHTVLinkCut::meUHTVLinkCut( meHTVegetationLayer *vlayer ){
	if( ! vlayer ) DETHROW( deeInvalidParam );
	
	pVLayer = NULL;
	pLinks = NULL;
	pLinkCount = 0;
	
	SetShortInfo( "Cut Vegetation Layer Links" );
	SetMemoryConsumption( sizeof( meUHTVLinkCut ) );
	
	pVLayer = vlayer;
	vlayer->AddReference();
}

meUHTVLinkCut::~meUHTVLinkCut(){
	if( pLinks ){
		while( pLinkCount > 0 ){
			pLinkCount--;
			pLinks[ pLinkCount ]->FreeReference();
		}
		delete [] pLinks;
	}
	
	if( pVLayer ) pVLayer->FreeReference();
}



// Management
///////////////

void meUHTVLinkCut::AddLinkToCut( meHTVRLink *link ){
	if( ! link ) DETHROW( deeInvalidParam );
	
	meHTVRLink **newArray = new meHTVRLink*[ pLinkCount + 1 ];
	if( ! newArray ) DETHROW( deeOutOfMemory );
	if( pLinks ){
		memcpy( newArray, pLinks, sizeof( meHTVRLink* ) * pLinkCount );
		delete [] pLinks;
	}
	pLinks = newArray;
	pLinks[ pLinkCount++ ] = link;
	
	link->AddReference();
}



void meUHTVLinkCut::Undo(){
	meHTVRule *ruleSource, *ruleDestination;
	int l, slotSource, slotDestination;
	
	for( l=0; l<pLinkCount; l++ ){
		ruleSource = pLinks[ l ]->GetSourceRule();
		ruleDestination = pLinks[ l ]->GetDestinationRule();
		slotSource = pLinks[ l ]->GetSourceSlot();
		slotDestination = pLinks[ l ]->GetDestinationSlot();
		
		ruleSource->GetSlotAt( slotSource ).AddLink( pLinks[ l ] );
		ruleDestination->GetSlotAt( slotDestination ).AddLink( pLinks[ l ] );
		
		pVLayer->AddLink( pLinks[ l ] );
	}
}

void meUHTVLinkCut::Redo(){
	meHTVRule *ruleSource, *ruleDestination;
	int l, slotSource, slotDestination;
	
	for( l=0; l<pLinkCount; l++ ){
		ruleSource = pLinks[ l ]->GetSourceRule();
		ruleDestination = pLinks[ l ]->GetDestinationRule();
		slotSource = pLinks[ l ]->GetSourceSlot();
		slotDestination = pLinks[ l ]->GetDestinationSlot();
		
		ruleSource->GetSlotAt( slotSource ).RemoveLink( pLinks[ l ] );
		ruleDestination->GetSlotAt( slotDestination ).RemoveLink( pLinks[ l ] );
		
		pVLayer->RemoveLink( pLinks[ l ] );
	}
}
