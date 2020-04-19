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

#include "aeUSetRuleRemoveBone.h"
#include "../../animator/rule/aeRule.h"

#include <dragengine/common/exceptions.h>



// Class aeUSetRuleRemoveBone
///////////////////////////////

// Constructor, destructor
////////////////////////////

aeUSetRuleRemoveBone::aeUSetRuleRemoveBone( aeRule* rule, const char* pattern ){
	if( ! rule || ! pattern ){
		DETHROW( deeInvalidParam );
	}
	
	pRule = NULL;
	
	const decStringSet &ruleBoneList = rule->GetListBones();
	const int boneCount = ruleBoneList.GetCount();
	int i;
	
	for( i=0; i<boneCount; i++ ){
		const decString &bone = ruleBoneList.GetAt( i );
		
		if( bone.MatchesPattern( pattern ) ){
			pBones.Add( bone );
		}
	}
	
	if( pBones.GetCount() == 1 ){
		SetShortInfo( "Remove bone from rule" );
		
	}else{
		SetShortInfo( "Remove bones from rule" );
	}
	
	pRule = rule;
	pRule->AddReference();
}

aeUSetRuleRemoveBone::~aeUSetRuleRemoveBone(){
	if( pRule ){
		pRule->FreeReference();
	}
}



// Management
///////////////

void aeUSetRuleRemoveBone::Undo(){
	const int count = pBones.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pRule->AddBone( pBones.GetAt( i ) );
	}
}

void aeUSetRuleRemoveBone::Redo(){
	const int count = pBones.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pRule->RemoveBone( pBones.GetAt( i ) );
	}
}
