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

#include "aeURuleGroupPasteRule.h"
#include "../../../animator/rule/aeRuleGroup.h"
#include "../../../animator/aeAnimator.h"
#include "../../../animator/link/aeLink.h"

#include <dragengine/common/exceptions.h>



// Class aeURuleGroupPasteRule
////////////////////////////////

// Constructor, destructor
////////////////////////////

aeURuleGroupPasteRule::aeURuleGroupPasteRule( aeRuleGroup *group, const aeRuleList &ruleList, int index ) :
pGroup( NULL ),
pIndex( index ){
	const int ruleCount = ruleList.GetCount();
	
	if( ! group || ruleCount == 0 ){
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
	
	pGroup = group;
	group->AddReference();
}

aeURuleGroupPasteRule::~aeURuleGroupPasteRule(){
	pCleanUp();
}



// Management
///////////////

void aeURuleGroupPasteRule::Undo(){
	const int ruleCount = pRuleList.GetCount();
	int i;
	
	for( i=0; i<ruleCount; i++ ){
		pGroup->RemoveRule( pRuleList.GetAt( i ) );
	}
	
	// remove links added in a prior redo
	aeAnimator * const animator = pGroup->GetAnimator();
	
	if( animator ){
		const int linkCount = pRemoveLinkList.GetCount();
		for( i=0; i<linkCount; i++ ){
			animator->RemoveLink( pRemoveLinkList.GetAt( i ) );
		}
		pRemoveLinkList.RemoveAll();
	}
	
	// remove controller added in a prior redo
	if( animator ){
		const int controllerCount = pRemoveControllerList.GetCount();
		for( i=0; i<controllerCount; i++ ){
			animator->RemoveController( pRemoveControllerList.GetAt( i ) );
		}
		pRemoveControllerList.RemoveAll();
	}
}

void aeURuleGroupPasteRule::Redo(){
	aeAnimator * const animator = pGroup->GetAnimator();
	const int ruleCount = pRuleList.GetCount();
	int i, j;
	
	pRemoveLinkList.RemoveAll();
	
	for( i=0; i<ruleCount; i++ ){
		aeRule * const rule = pRuleList.GetAt( i );
		
		// check if links exist in the animator. if not add them and mark them to remove during undo
		if( animator ){
			aeLinkList linkList;
			rule->ListLinks( linkList );
			
			const int linkCount = linkList.GetCount();
			for( j=0; j<linkCount; j++ ){
				aeLink * const link = linkList.GetAt( j );
				if( ! animator->GetLinks().Has( link ) ){
					aeController * const controller = link->GetController();
					if( controller && ! animator->GetControllers().Has( controller ) ){
						pRemoveControllerList.Add( controller );
						animator->AddController( controller );
					}
					
					pRemoveLinkList.Add( link );
					animator->AddLink( link );
				}
			}
		}
		
		// insert the rule
		pGroup->InsertRuleAt( rule, pIndex + i );
	}
}



// Private Functions
//////////////////////

void aeURuleGroupPasteRule::pCleanUp(){
	if( pGroup ){
		pGroup->FreeReference();
	}
}
