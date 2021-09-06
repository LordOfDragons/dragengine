/* 
 * Drag[en]gine IGDE Animator Editor
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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
#include <string.h>
#include <stdlib.h>

#include "aeURuleBTransSetMinAngle.h"

#include <dragengine/common/exceptions.h>


// Class aeURuleBTransSetMinAngle
///////////////////////////////////

// Constructor, destructor
////////////////////////////

aeURuleBTransSetMinAngle::aeURuleBTransSetMinAngle( aeRuleBoneTransformator *rule, float newValue ) :
pRule( rule ),
pOldValue( rule->GetMinimumAngle() ),
pNewValue( newValue )
{
	SetShortInfo( "Bone transformator set minimum angle" );
}

aeURuleBTransSetMinAngle::~aeURuleBTransSetMinAngle(){
}



// Management
///////////////

void aeURuleBTransSetMinAngle::Undo(){
	pRule->SetMinimumAngle( pOldValue );
}

void aeURuleBTransSetMinAngle::Redo(){
	pRule->SetMinimumAngle( pNewValue );
}
