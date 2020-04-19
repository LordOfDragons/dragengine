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

#include "meUHTVRuleCombineSetX.h"
#include "../../../../../../world/heightterrain/meHTVegetationLayer.h"
#include "../../../../../../world/heightterrain/rules/meHTVRuleCombine.h"

#include <dragengine/common/exceptions.h>



// Class meUHTVRuleCombineSetX
////////////////////////////////

// Constructor, destructor
////////////////////////////

meUHTVRuleCombineSetX::meUHTVRuleCombineSetX( meHTVegetationLayer *vlayer, meHTVRuleCombine *rule, float nx ){
	if( ! vlayer || ! rule ) DETHROW( deeInvalidParam );
	
	pVLayer = NULL;
	pRule = NULL;
	
	SetShortInfo( "Vegetation Layer Rule Combine Set X" );
	SetMemoryConsumption( sizeof( meUHTVRuleCombineSetX ) );
	
	pOldX = rule->GetX();
	pNewX = nx;
	
	pVLayer = vlayer;
	vlayer->AddReference();
	pRule = rule;
	rule->AddReference();
}

meUHTVRuleCombineSetX::~meUHTVRuleCombineSetX(){
	if( pRule ) pRule->FreeReference();
	if( pVLayer ) pVLayer->FreeReference();
}



// Management
///////////////

void meUHTVRuleCombineSetX::Undo(){
	pRule->SetX( pOldX );
	pVLayer->NotifyRuleChanged( pRule );
}

void meUHTVRuleCombineSetX::Redo(){
	pRule->SetX( pNewX );
	pVLayer->NotifyRuleChanged( pRule );
}
