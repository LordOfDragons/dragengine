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

#include "deAnimatorRuleStateSnapshot.h"
#include "deAnimatorRuleVisitor.h"

#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deAnimatorRuleStateSnapshot
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorRuleStateSnapshot::deAnimatorRuleStateSnapshot() :
pEnablePosition( true ),
pEnableOrientation( true ),
pEnableSize( false ),
pEnableVertexPositionSet( true ),
pUseLastState( true ),
pID( 0 ){
}

deAnimatorRuleStateSnapshot::~deAnimatorRuleStateSnapshot(){
}



// Management
///////////////

void deAnimatorRuleStateSnapshot::SetEnablePosition( bool enabled ){
	pEnablePosition = enabled;
}

void deAnimatorRuleStateSnapshot::SetEnableOrientation( bool enabled ){
	pEnableOrientation = enabled;
}

void deAnimatorRuleStateSnapshot::SetEnableSize( bool enabled ){
	pEnableSize = enabled;
}

void deAnimatorRuleStateSnapshot::SetEnableVertexPositionSet( bool enabled ){
	pEnableVertexPositionSet = enabled;
}

void deAnimatorRuleStateSnapshot::SetUseLastState( bool useLastState ){
	pUseLastState = useLastState;
}

void deAnimatorRuleStateSnapshot::SetID( int id ){
	pID = id;
}



// Visiting
/////////////

void deAnimatorRuleStateSnapshot::Visit( deAnimatorRuleVisitor &visitor ){
	visitor.VisitStateSnapshot( *this );
}
