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
