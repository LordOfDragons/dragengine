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

#include "meUHTVLinkAdd.h"
#include "../../../../../world/heightterrain/meHTVegetationLayer.h"
#include "../../../../../world/heightterrain/rules/meHTVRSlot.h"
#include "../../../../../world/heightterrain/rules/meHTVRLink.h"
#include "../../../../../world/heightterrain/rules/meHTVRule.h"

#include <dragengine/common/exceptions.h>



// Class meUHTVLinkAdd
////////////////////////

// Constructor, destructor
////////////////////////////

meUHTVLinkAdd::meUHTVLinkAdd( meHTVegetationLayer *vlayer, meHTVRLink *link ){
	if( ! vlayer || ! link ) DETHROW( deeInvalidParam );
	
	pVLayer = NULL;
	pLink = NULL;
	
	SetShortInfo( "Add Vegetation Layer Link" );
	SetMemoryConsumption( sizeof( meUHTVLinkAdd ) );
	
	pVLayer = vlayer;
	vlayer->AddReference();
	pLink = link;
	link->AddReference();
}

meUHTVLinkAdd::~meUHTVLinkAdd(){
	if( pLink ) pLink->FreeReference();
	if( pVLayer ) pVLayer->FreeReference();
}



// Management
///////////////

void meUHTVLinkAdd::Undo(){
	meHTVRule *ruleSource = pLink->GetSourceRule();
	meHTVRule *ruleDestination = pLink->GetDestinationRule();
	int slotSource = pLink->GetSourceSlot();
	int slotDestination = pLink->GetDestinationSlot();
	
	ruleSource->GetSlotAt( slotSource ).RemoveLink( pLink );
	ruleDestination->GetSlotAt( slotDestination ).RemoveLink( pLink );
	
	pVLayer->RemoveLink( pLink );
}

void meUHTVLinkAdd::Redo(){
	meHTVRule *ruleSource = pLink->GetSourceRule();
	meHTVRule *ruleDestination = pLink->GetDestinationRule();
	int slotSource = pLink->GetSourceSlot();
	int slotDestination = pLink->GetDestinationSlot();
	
	ruleSource->GetSlotAt( slotSource ).AddLink( pLink );
	ruleDestination->GetSlotAt( slotDestination ).AddLink( pLink );
	
	pVLayer->AddLink( pLink );
}
