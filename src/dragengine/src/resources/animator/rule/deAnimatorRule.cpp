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

#include "deAnimatorRule.h"
#include "deAnimatorRuleVisitor.h"
#include "../deAnimator.h"

#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deAnimatorRule
/////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorRule::deAnimatorRule(){
	pAnimator = NULL;
	pEnabled = true;
	pBlendMode = ebmBlend;
	pBlendFactor = 1.0f;
}

deAnimatorRule::~deAnimatorRule(){
}



// Management
///////////////

void deAnimatorRule::SetAnimator( deAnimator *animator ){
	pAnimator = animator;
}

void deAnimatorRule::SetEnabled( bool enabled ){
	pEnabled = enabled;
}

void deAnimatorRule::SetBlendMode( eBlendModes mode ){
	if( mode < ebmBlend || mode > ebmOverlay ){
		DETHROW( deeInvalidParam );
	}
	pBlendMode = mode;
}

void deAnimatorRule::SetBlendFactor( float factor ){
	pBlendFactor = factor;
}



// Visiting
/////////////

void deAnimatorRule::Visit( deAnimatorRuleVisitor &visitor ){
	visitor.VisitRule( *this );
}
