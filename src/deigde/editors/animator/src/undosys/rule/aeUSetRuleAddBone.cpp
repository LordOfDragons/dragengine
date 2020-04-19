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

#include "aeUSetRuleAddBone.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/aeAnimator.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/component/deComponent.h>



// Class aeUSetRuleAddBone
////////////////////////////

// Constructor, destructor
////////////////////////////

aeUSetRuleAddBone::aeUSetRuleAddBone( aeRule *rule, const char *pattern ){
	if( ! rule || ! pattern ){
		DETHROW( deeInvalidParam );
	}
	
	pRule = NULL;
	
	const decStringSet &ruleBoneList = rule->GetListBones();
	const deRig *engRig = rule->GetAnimator()->GetEngineRig();
	
	if( engRig ){
		const int boneCount = engRig->GetBoneCount();
		int i;
		
		for( i=0; i<boneCount; i++ ){
			const decString bone( engRig->GetBoneAt( i ).GetName() );
			
			if( bone.MatchesPattern( pattern ) && ! ruleBoneList.Has( bone ) ){
				pBones.Add( bone );
			}
		}
		
	}else{
		if( ! ruleBoneList.Has( pattern ) ){
			pBones.Add( pattern );
		}
	}
	
	if( pBones.GetCount() == 1 ){
		SetShortInfo( "Add bone to rule" );
		
	}else{
		SetShortInfo( "Add bones to rule" );
	}
	
	pRule = rule;
	pRule->AddReference();
}

aeUSetRuleAddBone::~aeUSetRuleAddBone(){
	if( pRule ){
		pRule->FreeReference();
	}
}



// Management
///////////////

void aeUSetRuleAddBone::Undo(){
	const int count = pBones.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pRule->RemoveBone( pBones.GetAt( i ) );
	}
}

void aeUSetRuleAddBone::Redo(){
	const int count = pBones.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pRule->AddBone( pBones.GetAt( i ) );
	}
}
