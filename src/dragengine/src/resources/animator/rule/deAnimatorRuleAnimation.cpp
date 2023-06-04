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

#include "deAnimatorRuleAnimation.h"
#include "deAnimatorRuleVisitor.h"

#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deAnimatorRuleAnimation
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorRuleAnimation::deAnimatorRuleAnimation() :
pMoveName( "idle" ),
pMoveTime( 0.0f ),
pEnablePosition( true ),
pEnableOrientation( true ),
pEnableSize( false ),
pEnableVertexPositionSet( true ){
}

deAnimatorRuleAnimation::~deAnimatorRuleAnimation(){
}



// Management
///////////////

void deAnimatorRuleAnimation::SetMoveName( const char *moveName ){
	pMoveName = moveName;
}

void deAnimatorRuleAnimation::SetMoveTime( float moveTime ){
	pMoveTime = moveTime;
}

void deAnimatorRuleAnimation::SetEnablePosition( bool enabled ){
	pEnablePosition = enabled;
}

void deAnimatorRuleAnimation::SetEnableOrientation( bool enabled ){
	pEnableOrientation = enabled;
}

void deAnimatorRuleAnimation::SetEnableSize( bool enabled ){
	pEnableSize = enabled;
}

void deAnimatorRuleAnimation::SetEnableVertexPositionSet( bool enabled ){
	pEnableVertexPositionSet = enabled;
}



// Visiting
/////////////

void deAnimatorRuleAnimation::Visit( deAnimatorRuleVisitor &visitor ){
	visitor.VisitAnimation( *this );
}
