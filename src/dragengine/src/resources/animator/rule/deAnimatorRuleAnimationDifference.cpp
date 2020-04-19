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

#include "deAnimatorRuleAnimationDifference.h"
#include "deAnimatorRuleVisitor.h"

#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deAnimatorRuleAnimationDifference
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorRuleAnimationDifference::deAnimatorRuleAnimationDifference() :
pMove1Name( "idle " ),
pMove1Time( 0.0f ),
pMove2Name( "idle" ),
pMove2Time( 0.0f ),
pUseSameMove( false ),
pEnablePosition( true ),
pEnableOrientation( true ),
pEnableSize( false ){
}

deAnimatorRuleAnimationDifference::~deAnimatorRuleAnimationDifference(){
}



// Management
///////////////

void deAnimatorRuleAnimationDifference::SetLeadingMoveName( const char *moveName ){
	if( ! moveName ) DETHROW( deeInvalidParam );
	
	pMove1Name = moveName;
}

void deAnimatorRuleAnimationDifference::SetLeadingMoveTime( float moveTime ){
	pMove1Time = moveTime;
}

void deAnimatorRuleAnimationDifference::SetReferenceMoveName( const char *moveName ){
	if( ! moveName ) DETHROW( deeInvalidParam );
	
	pMove2Name = moveName;
}

void deAnimatorRuleAnimationDifference::SetReferenceMoveTime( float moveTime ){
	pMove2Time = moveTime;
}

void deAnimatorRuleAnimationDifference::SetUseSameMove( bool useSameMove ){
	pUseSameMove = useSameMove;
}

void deAnimatorRuleAnimationDifference::SetEnablePosition( bool enabled ){
	pEnablePosition = enabled;
}

void deAnimatorRuleAnimationDifference::SetEnableOrientation( bool enabled ){
	pEnableOrientation = enabled;
}

void deAnimatorRuleAnimationDifference::SetEnableSize( bool enabled ){
	pEnableSize = enabled;
}



// Visiting
/////////////

void deAnimatorRuleAnimationDifference::Visit( deAnimatorRuleVisitor &visitor ){
	visitor.VisitAnimationDifference( *this );
}
