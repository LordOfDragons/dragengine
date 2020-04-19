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

#include "meUHTVRuleMove.h"
#include "../../../../../world/meWorld.h"
#include "../../../../../world/terrain/meHeightTerrain.h"
#include "../../../../../world/terrain/meHeightTerrainSector.h"
#include "../../../../../world/heightterrain/meHTVegetationLayer.h"
#include "../../../../../world/heightterrain/rules/meHTVRule.h"

#include <dragengine/common/exceptions.h>



// Class meUHTVRuleMove
/////////////////////////

// Constructor, destructor
////////////////////////////

meUHTVRuleMove::meUHTVRuleMove( meHTVegetationLayer *vlayer, meHTVRule *rule ){
	if( ! vlayer || ! rule ) DETHROW( deeInvalidParam );
	
	pVLayer = NULL;
	pRule = NULL;
	
	pOldPosition = rule->GetPosition();
	pNewPosition = pOldPosition;
	
	SetShortInfo( "Move Vegetation Layer Rule" );
	SetMemoryConsumption( sizeof( meUHTVRuleMove ) );
	
	pVLayer = vlayer;
	vlayer->AddReference();
	pRule = rule;
	rule->AddReference();
}

meUHTVRuleMove::~meUHTVRuleMove(){
	if( pRule ) pRule->FreeReference();
	if( pVLayer ) pVLayer->FreeReference();
}



// Management
///////////////

void meUHTVRuleMove::SetNewPosition( const decVector2 &position ){
	pNewPosition = position;
}



void meUHTVRuleMove::Undo(){
	pRule->SetPosition( pOldPosition );
	pVLayer->NotifyRuleMoved( pRule );
}

void meUHTVRuleMove::Redo(){
	pRule->SetPosition( pNewPosition );
	pVLayer->NotifyRuleMoved( pRule );
}
