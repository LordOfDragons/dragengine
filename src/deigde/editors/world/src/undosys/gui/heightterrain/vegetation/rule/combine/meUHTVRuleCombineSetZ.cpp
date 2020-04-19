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

#include "meUHTVRuleCombineSetZ.h"
#include "../../../../../../world/heightterrain/meHTVegetationLayer.h"
#include "../../../../../../world/heightterrain/rules/meHTVRuleCombine.h"

#include <dragengine/common/exceptions.h>



// Class meUHTVRuleCombineSetZ
////////////////////////////////

// Constructor, destructor
////////////////////////////

meUHTVRuleCombineSetZ::meUHTVRuleCombineSetZ( meHTVegetationLayer *vlayer, meHTVRuleCombine *rule, float nz ){
	if( ! vlayer || ! rule ) DETHROW( deeInvalidParam );
	
	pVLayer = NULL;
	pRule = NULL;
	
	SetShortInfo( "Vegetation Layer Rule Combine Set Z" );
	SetMemoryConsumption( sizeof( meUHTVRuleCombineSetZ ) );
	
	pOldZ = rule->GetZ();
	pNewZ = nz;
	
	pVLayer = vlayer;
	vlayer->AddReference();
	pRule = rule;
	rule->AddReference();
}

meUHTVRuleCombineSetZ::~meUHTVRuleCombineSetZ(){
	if( pRule ) pRule->FreeReference();
	if( pVLayer ) pVLayer->FreeReference();
}



// Management
///////////////

void meUHTVRuleCombineSetZ::Undo(){
	pRule->SetZ( pOldZ );
	pVLayer->NotifyRuleChanged( pRule );
}

void meUHTVRuleCombineSetZ::Redo(){
	pRule->SetZ( pNewZ );
	pVLayer->NotifyRuleChanged( pRule );
}
