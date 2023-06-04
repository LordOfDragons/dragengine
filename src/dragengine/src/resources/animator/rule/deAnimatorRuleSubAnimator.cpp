/* 
 * Drag[en]gine Game Engine
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

#include "deAnimatorRuleSubAnimator.h"
#include "deAnimatorRuleVisitor.h"
#include "../deAnimator.h"
#include "../controller/deAnimatorController.h"

#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deAnimatorRuleSubAnimator
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorRuleSubAnimator::deAnimatorRuleSubAnimator() :
pEnableVertexPositionSet( true )
{
	pConnections = NULL;
	pConnectionCount = 0;
	
	pEnablePosition = true;
	pEnableOrientation = true;
	pEnableSize = true;
}

deAnimatorRuleSubAnimator::~deAnimatorRuleSubAnimator(){
	if( pConnections ){
		delete [] pConnections;
	}
}



// Management
///////////////

void deAnimatorRuleSubAnimator::SetSubAnimator( deAnimator *animator ){
	if( animator == pSubAnimator ){
		return;
	}
	
	pSubAnimator = animator;
	UpdateConnectionCount();
}

void deAnimatorRuleSubAnimator::SetEnablePosition( bool enabled ){
	pEnablePosition = enabled;
}

void deAnimatorRuleSubAnimator::SetEnableOrientation( bool enabled ){
	pEnableOrientation = enabled;
}

void deAnimatorRuleSubAnimator::SetEnableSize( bool enabled ){
	pEnableSize = enabled;
}

void deAnimatorRuleSubAnimator::SetEnableVertexPositionSet( bool enabled ){
	pEnableVertexPositionSet = enabled;
}



void deAnimatorRuleSubAnimator::UpdateConnectionCount(){
	if( pConnections ){
		delete [] pConnections;
		pConnections = NULL;
		pConnectionCount = 0;
	}
	
	if( ! pSubAnimator ){
		return;
	}
	
	int controllerCount = pSubAnimator->GetControllerCount();
	if( controllerCount == 0 ){
		return;
	}
	
	pConnections = new int[ controllerCount ];
	for( pConnectionCount=0; pConnectionCount<controllerCount; pConnectionCount++ ){
		pConnections[ pConnectionCount ] = -1;
	}
}

int deAnimatorRuleSubAnimator::GetConnectionAt( int targetController ) const{
	if( targetController < 0 || targetController >= pConnectionCount ) DETHROW( deeInvalidParam );
	
	return pConnections[ targetController ];
}

void deAnimatorRuleSubAnimator::SetConnectionAt( int targetController, int localController ){
	if( targetController < 0 || targetController >= pConnectionCount ) DETHROW( deeInvalidParam );
	if( localController < -1 ) DETHROW( deeInvalidParam );
	
	pConnections[ targetController ] = localController;
}

void deAnimatorRuleSubAnimator::SetMatchingConnections( const deAnimator &animator ){
	if( ! pSubAnimator ){
		ClearConnections();
		return;
	}
	
	int i;
	for( i=0; i<pConnectionCount; i++ ){
		pConnections[ i ] = animator.IndexOfControllerNamed( pSubAnimator->GetControllerAt( i )->GetName() );
	}
}

void deAnimatorRuleSubAnimator::ClearConnections(){
	int i;
	for( i=0; i<pConnectionCount; i++ ){
		pConnections[ i ] = -1;
	}
}



// Visiting
/////////////

void deAnimatorRuleSubAnimator::Visit( deAnimatorRuleVisitor &visitor ){
	visitor.VisitSubAnimator( *this );
}
