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

#include "aeRuleStateSnapshot.h"

#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateSnapshot.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>



// Class aeRuleStateSnapshot
//////////////////////////////

// Constructor, destructor
////////////////////////////

aeRuleStateSnapshot::aeRuleStateSnapshot() :
aeRule( deAnimatorRuleVisitorIdentify::ertStateSnapshot ),
pUseLastState( true ),
pID( 0 ),
pEnablePosition( true ),
pEnableOrientation( true ),
pEnableSize( false ),
pEnableVertexPositionSet( true )
{
	SetName( "State Snapshot" );
}

aeRuleStateSnapshot::aeRuleStateSnapshot( const aeRuleStateSnapshot &copy ) :
aeRule( copy ),
pUseLastState( copy.pUseLastState ),
pID( copy.pID ),
pEnablePosition( copy.pEnablePosition ),
pEnableOrientation( copy.pEnableOrientation ),
pEnableSize( copy.pEnableSize ),
pEnableVertexPositionSet( copy.pEnableVertexPositionSet ){
}

aeRuleStateSnapshot::~aeRuleStateSnapshot(){
}



// Management
///////////////

void aeRuleStateSnapshot::SetUseLastState( bool useLastState ){
	if( useLastState == pUseLastState ){
		return;
	}
	
	pUseLastState = useLastState;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleStateSnapshot* )GetEngineRule() )->SetUseLastState( useLastState );
		NotifyRuleChanged();
	}
}

void aeRuleStateSnapshot::SetID( int id ){
	if( id == pID ){
		return;
	}
	
	pID = id;
	
	if( GetEngineRule() ){
		( ( deAnimatorRuleStateSnapshot* )GetEngineRule() )->SetID( id );
		NotifyRuleChanged();
	}
}

void aeRuleStateSnapshot::SetEnablePosition( bool enabled ){
	if( enabled != pEnablePosition ){
		pEnablePosition = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleStateSnapshot* )GetEngineRule() )->SetEnablePosition( enabled );
			NotifyRuleChanged();
		}
	}
}

void aeRuleStateSnapshot::SetEnableOrientation( bool enabled ){
	if( enabled != pEnableOrientation ){
		pEnableOrientation = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleStateSnapshot* )GetEngineRule() )->SetEnableOrientation( enabled );
			NotifyRuleChanged();
		}
	}
}

void aeRuleStateSnapshot::SetEnableSize( bool enabled ){
	if( enabled != pEnableSize ){
		pEnableSize = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleStateSnapshot* )GetEngineRule() )->SetEnableSize( enabled );
			NotifyRuleChanged();
		}
	}
}

void aeRuleStateSnapshot::SetEnableVertexPositionSet( bool enabled ){
	if( enabled != pEnableVertexPositionSet ){
		pEnableVertexPositionSet = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleStateSnapshot* )GetEngineRule() )->SetEnableVertexPositionSet( enabled );
			NotifyRuleChanged();
		}
	}
}



deAnimatorRule *aeRuleStateSnapshot::CreateEngineRule(){
	deAnimatorRuleStateSnapshot *engRule = NULL;
	
	try{
		engRule = new deAnimatorRuleStateSnapshot;
		
		InitEngineRule( engRule );
		
		engRule->SetUseLastState( pUseLastState );
		engRule->SetID( pID );
		
		engRule->SetEnablePosition( pEnablePosition );
		engRule->SetEnableOrientation( pEnableOrientation );
		engRule->SetEnableSize( pEnableSize );
		engRule->SetEnableVertexPositionSet( pEnableVertexPositionSet );
		
	}catch( const deException & ){
		if( engRule ){
			engRule->FreeReference();
		}
		throw;
	}
	
	// finished
	return engRule;
}



aeRule *aeRuleStateSnapshot::CreateCopy() const{
	return new aeRuleStateSnapshot( *this );
}

void aeRuleStateSnapshot::ListLinks( aeLinkList &list ){
	aeRule::ListLinks( list );
}



// Operators
///////////////

aeRuleStateSnapshot &aeRuleStateSnapshot::operator=( const aeRuleStateSnapshot &copy ){
	SetUseLastState( copy.pUseLastState );
	SetID( copy.pID );
	SetEnablePosition( copy.pEnablePosition );
	SetEnableOrientation( copy.pEnableOrientation );
	SetEnableSize( copy.pEnableSize );
	SetEnableVertexPositionSet( copy.pEnableVertexPositionSet );
	aeRule::operator=( copy );
	return *this;
}
