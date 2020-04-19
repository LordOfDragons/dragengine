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

#include "meUHTVRulePCSetClass.h"
#include "../../../../../../world/heightterrain/meHTVegetationLayer.h"
#include "../../../../../../world/heightterrain/rules/meHTVRulePropCount.h"

#include <dragengine/common/exceptions.h>



// Class meUHTVRulePCSetClass
///////////////////////////////

// Constructor, destructor
////////////////////////////

meUHTVRulePCSetClass::meUHTVRulePCSetClass( meHTVegetationLayer *vlayer, meHTVRulePropCount *rule, const char *nclass ){
	if( ! vlayer || ! rule || ! nclass ) DETHROW( deeInvalidParam );
	
	pVLayer = NULL;
	pRule = NULL;
	
	SetShortInfo( "Vegetation Layer Rule Prop Count Set Class" );
	SetMemoryConsumption( sizeof( meUHTVRulePCSetClass ) );
	
	pOldPropClass = rule->GetPropClass();
	pNewPropClass = nclass;
	
	pVLayer = vlayer;
	vlayer->AddReference();
	pRule = rule;
	rule->AddReference();
}

meUHTVRulePCSetClass::~meUHTVRulePCSetClass(){
	if( pRule ) pRule->FreeReference();
	if( pVLayer ) pVLayer->FreeReference();
}



// Management
///////////////

void meUHTVRulePCSetClass::Undo(){
	pRule->SetPropClass( pOldPropClass.GetString() );
	pVLayer->NotifyRuleChanged( pRule );
}

void meUHTVRulePCSetClass::Redo(){
	pRule->SetPropClass( pNewPropClass.GetString() );
	pVLayer->NotifyRuleChanged( pRule );
}
