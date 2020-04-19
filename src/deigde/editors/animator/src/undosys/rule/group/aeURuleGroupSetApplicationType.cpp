/* 
 * Drag[en]gine IGDE Animator Editor
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

#include "aeURuleGroupSetApplicationType.h"
#include "../../../animator/rule/aeRuleGroup.h"

#include <dragengine/common/exceptions.h>



// Class aeURuleGroupSetApplicationType
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeURuleGroupSetApplicationType::aeURuleGroupSetApplicationType( aeRuleGroup *rule,
deAnimatorRuleGroup::eApplicationTypes newType ){
	if( ! rule ){
		DETHROW( deeInvalidParam );
	}
	
	pRule = NULL;
	pOldType = rule->GetApplicationType();
	pNewType = newType;
	
	SetShortInfo( "Group rule set application type" );
	
	pRule = rule;
	pRule->AddReference();
}

aeURuleGroupSetApplicationType::~aeURuleGroupSetApplicationType(){
	if( pRule ){
		pRule->FreeReference();
	}
}



// Management
///////////////

void aeURuleGroupSetApplicationType::Undo(){
	pRule->SetApplicationType( pOldType );
}

void aeURuleGroupSetApplicationType::Redo(){
	pRule->SetApplicationType( pNewType );
}
