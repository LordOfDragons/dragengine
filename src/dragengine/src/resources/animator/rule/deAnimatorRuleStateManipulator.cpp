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

#include "deAnimatorRuleStateManipulator.h"
#include "deAnimatorRuleVisitor.h"

#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deAnimatorRuleStateManipulator
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorRuleStateManipulator::deAnimatorRuleStateManipulator(){
	pMinSize.Set( 1.0f, 1.0f, 1.0f );
	pMaxSize.Set( 1.0f, 1.0f, 1.0f );
	pEnablePosition = false;
	pEnableRotation = true;
	pEnableSize = false;
}

deAnimatorRuleStateManipulator::~deAnimatorRuleStateManipulator(){
}



// Management
///////////////

void deAnimatorRuleStateManipulator::SetMinimumPosition( const decVector &position ){
	pMinPosition = position;
}

void deAnimatorRuleStateManipulator::SetMaximumPosition( const decVector &position ){
	pMaxPosition = position;
}

void deAnimatorRuleStateManipulator::SetMinimumRotation( const decVector &orientation ){
	pMinRotation = orientation;
}

void deAnimatorRuleStateManipulator::SetMaximumRotation( const decVector &orientation ){
	pMaxRotation = orientation;
}

void deAnimatorRuleStateManipulator::SetMinimumSize( const decVector &size ){
	pMinSize = size;
}

void deAnimatorRuleStateManipulator::SetMaximumSize( const decVector &size ){
	pMaxSize = size;
}

void deAnimatorRuleStateManipulator::SetEnablePosition( bool enable ){
	pEnablePosition = enable;
}

void deAnimatorRuleStateManipulator::SetEnableRotation( bool enable ){
	pEnableRotation = enable;
}

void deAnimatorRuleStateManipulator::SetEnableSize( bool enable ){
	pEnableSize = enable;
}



// Visiting
/////////////

void deAnimatorRuleStateManipulator::Visit( deAnimatorRuleVisitor &visitor ){
	visitor.VisitStateManipulator( *this );
}
