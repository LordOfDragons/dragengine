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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "aeRuleGroup.h"
#include "../aeAnimator.h"

#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleGroup.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>



// Class aeRuleGroup
//////////////////////

// Constructor, destructor
////////////////////////////

aeRuleGroup::aeRuleGroup() :
aeRule( deAnimatorRuleVisitorIdentify::ertGroup ),
pEnablePosition( true ),
pEnableOrientation( true ),
pEnableSize( false ),
pUseCurrentState( false ),
pApplicationType( deAnimatorRuleGroup::eatAll ),
pTreeListExpanded( false )
{
	SetName( "Group" );
}

aeRuleGroup::aeRuleGroup( const aeRuleGroup &copy ) :
aeRule( copy ),
pEnablePosition( copy.pEnablePosition ),
pEnableOrientation( copy.pEnableOrientation ),
pEnableSize( copy.pEnableSize ),
pUseCurrentState( copy.pUseCurrentState ),
pApplicationType( copy.pApplicationType ),
pTargetSelect( copy.pTargetSelect ),
pTreeListExpanded( copy.pTreeListExpanded )
{
	const int ruleCount = copy.pRules.GetCount();
	aeRule *rule = NULL;
	int i;
	
	try{
		for( i=0; i<ruleCount; i++ ){
			rule = copy.pRules.GetAt( i )->CreateCopy();
			pRules.Add( rule );
			rule->SetParentGroup( this );
			rule->FreeReference();
			rule = NULL;
		}
		
	}catch( const deException & ){
		if( rule ){
			rule->FreeReference();
		}
		throw;
	}
}

aeRuleGroup::~aeRuleGroup(){
	pCleanUp();
}



// Management
///////////////

void aeRuleGroup::AddRule( aeRule *rule ){
	pRules.Add( rule );
	
	aeAnimator * const animator = GetAnimator();
	
	rule->SetParentGroup( this );
	rule->SetAnimator( animator );
	
	if( animator ){
		animator->RebuildRules();
		animator->NotifyRuleStructureChanged();
	}
}

void aeRuleGroup::InsertRuleAt( aeRule *rule, int index ){
	pRules.Insert( rule, index );
	
	aeAnimator * const animator = GetAnimator();
	
	rule->SetParentGroup( this );
	rule->SetAnimator( animator );
	
	if( animator ){
		animator->RebuildRules();
		animator->NotifyRuleStructureChanged();
	}
}

void aeRuleGroup::MoveRuleTo( aeRule *rule, int index ){
	pRules.Move( rule, index );
	
	aeAnimator * const animator = GetAnimator();
	if( animator ){
		animator->RebuildRules();
		animator->NotifyRuleStructureChanged();
	}
}

void aeRuleGroup::RemoveRule( aeRule *rule ){
	if( ! pRules.Has( rule ) ){
		DETHROW( deeInvalidParam );
	}
	
	aeAnimator * const animator = GetAnimator();
	
	if( animator && rule == animator->GetActiveRule() ){
		const int index = pRules.IndexOf( rule );
		const int ruleCount = pRules.GetCount();
		
		if( ruleCount == 1 ){
			animator->SetActiveRule( this );
			
		}else if( index < ruleCount - 1 ){
			animator->SetActiveRule( pRules.GetAt( index + 1 ) );
			
		}else{
			animator->SetActiveRule( pRules.GetAt( index - 1 ) );
		}
	}
	
	rule->SetParentGroup( NULL );
	rule->SetAnimator( NULL );
	
	pRules.Remove( rule );
	
	if( animator ){
		animator->RebuildRules();
		animator->NotifyRuleStructureChanged();
	}
}

void aeRuleGroup::RemoveAllRules(){
	const int count = pRules.GetCount();
	int i;
	
	aeAnimator * const animator = GetAnimator();
	
	if( animator && pRules.Has( animator->GetActiveRule() ) ){
		animator->SetActiveRule( this );
	}
	
	for( i=0; i<count; i++ ){
		aeRule * const rule = pRules.GetAt( i );
		rule->SetParentGroup( NULL );
		rule->SetAnimator( NULL );
	}
	
	pRules.RemoveAll();
	
	if( animator ){
		animator->RebuildRules();
		animator->NotifyRuleStructureChanged();
	}
}



void aeRuleGroup::SetEnablePosition( bool enabled ){
	if( enabled != pEnablePosition ){
		pEnablePosition = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleGroup* )GetEngineRule() )->SetEnablePosition( enabled );
			NotifyRuleChanged();
		}
	}
}

void aeRuleGroup::SetEnableOrientation( bool enabled ){
	if( enabled != pEnableOrientation ){
		pEnableOrientation = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleGroup* )GetEngineRule() )->SetEnableOrientation( enabled );
			NotifyRuleChanged();
		}
	}
}

void aeRuleGroup::SetEnableSize( bool enabled ){
	if( enabled != pEnableSize ){
		pEnableSize = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleGroup* )GetEngineRule() )->SetEnableSize( enabled );
			NotifyRuleChanged();
		}
	}
}


void aeRuleGroup::SetUseCurrentState( bool useCurrentState ){
	if( useCurrentState == pUseCurrentState ){
		return;
	}
	
	pUseCurrentState = useCurrentState;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleGroup* )GetEngineRule() )->SetUseCurrentState( useCurrentState );
		NotifyRuleChanged();
	}
}

void aeRuleGroup::SetApplicationType( deAnimatorRuleGroup::eApplicationTypes type ){
	if( type != pApplicationType ){
		pApplicationType = type;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleGroup* )GetEngineRule() )->SetApplicationType( type );
			NotifyRuleChanged();
		}
	}
}



void aeRuleGroup::UpdateTargets(){
	aeRule::UpdateTargets();
	
	deAnimatorRuleGroup * const rule = ( deAnimatorRuleGroup* )GetEngineRule();
	if( rule ){
		pTargetSelect.UpdateEngineTarget( GetAnimator(), rule->GetTargetSelect() );
	}
	
	const int count = pRules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( aeRule* )pRules.GetAt( i ) )->UpdateTargets();
	}
}

int aeRuleGroup::CountLinkUsage( aeLink *link ) const{
	int i, usageCount = aeRule::CountLinkUsage( link );
	const int count = pRules.GetCount();
	
	if( pTargetSelect.HasLink( link ) ){
		usageCount++;
	}
	
	for( i=0; i<count; i++ ){
		usageCount += pRules.GetAt( i )->CountLinkUsage( link );
	}
	
	return usageCount;
}

void aeRuleGroup::RemoveLinkFromTargets( aeLink *link ){
	aeRule::RemoveLinkFromTargets( link );
	
	if( pTargetSelect.HasLink( link ) ){
		pTargetSelect.RemoveLink( link );
	}
	
	const int count = pRules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pRules.GetAt( i )->RemoveLinkFromTargets( link );
	}
	
	aeRule::UpdateTargets();
}

void aeRuleGroup::RemoveLinksFromAllTargets(){
	aeRule::RemoveLinksFromAllTargets();
	
	pTargetSelect.RemoveAllLinks();
	
	const int count = pRules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pRules.GetAt( i )->RemoveLinksFromAllTargets();
	}
	
	aeRule::UpdateTargets();
}



deAnimatorRule *aeRuleGroup::CreateEngineRule(){
	deAnimatorRuleGroup *engRule = NULL;
	deAnimatorRule *subEngRule = NULL;
	const int count = pRules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pRules.GetAt( i )->SetEngineRule( NULL );
	}
	
	try{
		engRule = new deAnimatorRuleGroup;
		
		InitEngineRule( engRule );
		
		for( i=0; i<count; i++ ){
			aeRule * const rule = pRules.GetAt( i );
			
			subEngRule = rule->CreateEngineRule();
			engRule->AddRule( subEngRule );
			rule->SetEngineRule( subEngRule );
			subEngRule->FreeReference();
			subEngRule = NULL;
		}
		
		engRule->SetEnablePosition( pEnablePosition );
		engRule->SetEnableOrientation( pEnableOrientation );
		engRule->SetEnableSize( pEnableSize );
		
		engRule->SetUseCurrentState( pUseCurrentState );
		engRule->SetApplicationType( pApplicationType );
		
		pTargetSelect.UpdateEngineTarget( GetAnimator(), engRule->GetTargetSelect() );
		
	}catch( const deException & ){
		if( subEngRule ){
			subEngRule->FreeReference();
		}
		if( engRule ){
			engRule->FreeReference();
		}
		throw;
	}
	
	// finished
	return engRule;
}



void aeRuleGroup::SetTreeListExpanded( bool expanded ){
	pTreeListExpanded = expanded;
}



aeRule *aeRuleGroup::CreateCopy() const{
	return new aeRuleGroup( *this );
}

void aeRuleGroup::ListLinks( aeLinkList &list ){
	const int count = pRules.GetCount();
	int i;
	
	aeRule::ListLinks( list );
	
	pTargetSelect.AddLinksToList( list );
	
	for( i=0; i<count; i++ ){
		pRules.GetAt( i )->ListLinks( list );
	}
}

void aeRuleGroup::OnParentAnimatorChanged(){
	aeAnimator * const animator = GetAnimator();
	const int count = pRules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pRules.GetAt( i )->SetAnimator( animator );
	}
}



// Operators
//////////////

aeRuleGroup &aeRuleGroup::operator=( const aeRuleGroup &copy ){
	SetEnablePosition( copy.pEnablePosition );
	SetEnableOrientation( copy.pEnableOrientation );
	SetEnableSize( copy.pEnableSize );
	SetTreeListExpanded( copy.pTreeListExpanded );
	SetUseCurrentState( copy.pUseCurrentState );
	SetApplicationType( copy.pApplicationType );
	pTargetSelect = copy.pTargetSelect;
	
	const int ruleCount = copy.pRules.GetCount();
	aeRule *rule = NULL;
	int i;
	
	RemoveAllRules();
	try{
		for( i=0; i<ruleCount; i++ ){
			rule = copy.pRules.GetAt( i )->CreateCopy();
			AddRule( rule );
			rule->SetParentGroup( this );
			rule->FreeReference();
			rule = NULL;
		}
		
	}catch( const deException & ){
		if( rule ){
			rule->FreeReference();
		}
		throw;
	}
	
	aeRule::operator=( copy );
	return *this;
}



// Private Functions
//////////////////////

void aeRuleGroup::pCleanUp(){
	RemoveAllRules();
}
