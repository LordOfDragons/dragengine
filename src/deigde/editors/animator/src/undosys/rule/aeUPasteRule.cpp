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

#include "aeUPasteRule.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/controller/aeController.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/link/aeLink.h"

#include <dragengine/common/exceptions.h>



// Class aeUPasteRule
///////////////////////

// Constructor, destructor
////////////////////////////

aeUPasteRule::aeUPasteRule( aeAnimator *animator, const aeRuleList &ruleList, int index ) :
pAnimator( NULL ),
pIndex( index ){
	const int ruleCount = ruleList.GetCount();
	
	if( ! animator || ruleCount == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	aeRule *rule = NULL;
	int i;
	
	try{
		for( i=0; i<ruleCount; i++ ){
			rule = ruleList.GetAt( i )->CreateCopy();
			pRuleList.Add( rule );
			rule->FreeReference();
			rule = NULL;
		}
		
	}catch( const deException & ){
		if( rule ){
			rule->FreeReference();
		}
		throw;
	}
	
	pAnimator = animator;
	animator->AddReference();
}

aeUPasteRule::~aeUPasteRule(){
	pCleanUp();
}



// Management
///////////////

void aeUPasteRule::Undo(){
	const int ruleCount = pRuleList.GetCount();
	int i;
	
	// remove the rules
	for( i=0; i<ruleCount; i++ ){
		pAnimator->RemoveRule( pRuleList.GetAt( i ) );
	}
	
	// remove links added in a prior redo
	const int linkCount = pRemoveLinkList.GetCount();
	for( i=0; i<linkCount; i++ ){
		pAnimator->RemoveLink( pRemoveLinkList.GetAt( i ) );
	}
	pRemoveLinkList.RemoveAll();
	
	// remove controller added in a prior redo
	const int controllerCount = pRemoveControllerList.GetCount();
	for( i=0; i<controllerCount; i++ ){
		pAnimator->RemoveController( pRemoveControllerList.GetAt( i ) );
	}
	pRemoveControllerList.RemoveAll();
}

void aeUPasteRule::Redo(){
	const aeControllerList &controllers = pAnimator->GetControllers();
	const aeLinkList &links = pAnimator->GetLinks();
	const int ruleCount = pRuleList.GetCount();
	int i, j;
	
	pRemoveLinkList.RemoveAll();
	pRemoveControllerList.RemoveAll();
	
	for( i=0; i<ruleCount; i++ ){
		aeRule * const rule = pRuleList.GetAt( i );
		
		// check if links exist in the animator. if not add them and mark them to remove during undo
		aeLinkList linkList;
		rule->ListLinks( linkList );
		
		const int linkCount = linkList.GetCount();
		for( j=0; j<linkCount; j++ ){
			aeLink * const link = linkList.GetAt( j );
			if( links.Has( link ) ){
				continue;
			}
			
			// check if controllers exist in the animator. if not add them and mark them to
			// remove during undo. controllers are matched by name. if controller has been
			// added modify all links. this should be safe since only pasted links not present
			// in the animator can be potentially modified
			aeController * const controller = link->GetController();
			if( controller && ! controllers.Has( controller ) ){
				aeController * const sameNameController = controllers.GetNamed( controller->GetName() );
				
				if( sameNameController ){
					link->SetController( sameNameController, false );
					
				}else{
					link->SetController( controller, false );
					pRemoveControllerList.Add( controller );
					pAnimator->AddController( controller );
				}
			}
			
			pRemoveLinkList.Add( link );
			pAnimator->AddLink( link );
		}
		
		// insert the rule
		pAnimator->InsertRuleAt( rule, pIndex + i );
	}
}



// Private Functions
//////////////////////

void aeUPasteRule::pCleanUp(){
	if( pAnimator ){
		pAnimator->FreeReference();
	}
}
