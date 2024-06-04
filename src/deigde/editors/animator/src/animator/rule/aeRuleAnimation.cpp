/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "aeRuleAnimation.h"
#include "../aeAnimator.h"

#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>



// Class aeRuleAnimation
//////////////////////////

// Constructor, destructor
////////////////////////////

aeRuleAnimation::aeRuleAnimation() :
aeRule( deAnimatorRuleVisitorIdentify::ertAnimation ),
pMoveName( "idle" ),
pMoveTime( 0.0f ),
pEnablePosition( true ),
pEnableOrientation( true ),
pEnableSize( false ),
pEnableVertexPositionSet( true )
{
	SetName( "Animation" );
}

aeRuleAnimation::aeRuleAnimation( const aeRuleAnimation &copy ) : 
aeRule( copy ),
pMoveName( copy.pMoveName ),
pMoveTime( copy.pMoveTime ),
pEnablePosition( copy.pEnablePosition ),
pEnableOrientation( copy.pEnableOrientation ),
pEnableSize( copy.pEnableSize ),
pEnableVertexPositionSet( copy.pEnableVertexPositionSet ),
pTargetMoveTime( copy.pTargetMoveTime ){
}

aeRuleAnimation::~aeRuleAnimation(){
}



// Management
///////////////

void aeRuleAnimation::SetMoveName( const char *moveName ){
	pMoveName = moveName;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleAnimation* )GetEngineRule() )->SetMoveName( moveName );
		NotifyRuleChanged();
	}
}

void aeRuleAnimation::SetMoveTime( float moveTime ){
	pMoveTime = moveTime;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleAnimation* )GetEngineRule() )->SetMoveTime( moveTime );
		NotifyRuleChanged();
	}
}

void aeRuleAnimation::SetEnablePosition( bool enabled ){
	if( enabled != pEnablePosition ){
		pEnablePosition = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleAnimation* )GetEngineRule() )->SetEnablePosition( enabled );
			NotifyRuleChanged();
		}
	}
}

void aeRuleAnimation::SetEnableOrientation( bool enabled ){
	if( enabled != pEnableOrientation ){
		pEnableOrientation = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleAnimation* )GetEngineRule() )->SetEnableOrientation( enabled );
			NotifyRuleChanged();
		}
	}
}

void aeRuleAnimation::SetEnableSize( bool enabled ){
	if( enabled != pEnableSize ){
		pEnableSize = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleAnimation* )GetEngineRule() )->SetEnableSize( enabled );
			NotifyRuleChanged();
		}
	}
}

void aeRuleAnimation::SetEnableVertexPositionSet( bool enabled ){
	if( enabled != pEnableVertexPositionSet ){
		pEnableVertexPositionSet = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleAnimation* )GetEngineRule() )->SetEnableVertexPositionSet( enabled );
			NotifyRuleChanged();
		}
	}
}



void aeRuleAnimation::UpdateTargets(){
	deAnimatorRuleAnimation *rule = ( deAnimatorRuleAnimation* )GetEngineRule();
	
	aeRule::UpdateTargets();
	
	if( rule ){
		pTargetMoveTime.UpdateEngineTarget( GetAnimator(), rule->GetTargetMoveTime() );
	}
}

int aeRuleAnimation::CountLinkUsage( aeLink *link ) const{
	int usageCount = aeRule::CountLinkUsage( link );
	
	if( pTargetMoveTime.HasLink( link ) ) usageCount++;
	
	return usageCount;
}

void aeRuleAnimation::RemoveLinkFromTargets( aeLink *link ){
	aeRule::RemoveLinkFromTargets( link );
	
	if( pTargetMoveTime.HasLink( link ) ){
		pTargetMoveTime.RemoveLink( link );
	}
	
	UpdateTargets();
}

void aeRuleAnimation::RemoveLinksFromAllTargets(){
	aeRule::RemoveLinksFromAllTargets();
	
	pTargetMoveTime.RemoveAllLinks();
	
	UpdateTargets();
}



deAnimatorRule *aeRuleAnimation::CreateEngineRule(){
	deAnimatorRuleAnimation *engRule = NULL;
	
	try{
		// create rule
		engRule = new deAnimatorRuleAnimation;
		if( ! engRule ) DETHROW( deeOutOfMemory );
		
		// init rule
		InitEngineRule( engRule );
		
		engRule->SetMoveName( pMoveName.GetString() );
		engRule->SetMoveTime( pMoveTime );
		engRule->SetEnablePosition( pEnablePosition );
		engRule->SetEnableOrientation( pEnableOrientation );
		engRule->SetEnableSize( pEnableSize );
		engRule->SetEnableVertexPositionSet( pEnableVertexPositionSet );
		
		pTargetMoveTime.UpdateEngineTarget( GetAnimator(), engRule->GetTargetMoveTime() );
		
	}catch( const deException & ){
		if( engRule ){
			engRule->FreeReference();
		}
		throw;
	}
	
	// finished
	return engRule;
}



aeRule *aeRuleAnimation::CreateCopy() const{
	return new aeRuleAnimation( *this );
}



void aeRuleAnimation::ListLinks( aeLinkList &list ){
	aeRule::ListLinks( list );
	pTargetMoveTime.AddLinksToList( list );
}



// Operators
//////////////

aeRuleAnimation &aeRuleAnimation::operator=( const aeRuleAnimation &copy ){
	SetMoveName( copy.pMoveName );
	SetMoveTime( copy.pMoveTime );
	SetEnablePosition( copy.pEnablePosition );
	SetEnableOrientation( copy.pEnableOrientation );
	SetEnableSize( copy.pEnableSize );
	SetEnableVertexPositionSet( copy.pEnableVertexPositionSet );
	pTargetMoveTime = copy.pTargetMoveTime;
	aeRule::operator=( copy );
	return *this;
}
