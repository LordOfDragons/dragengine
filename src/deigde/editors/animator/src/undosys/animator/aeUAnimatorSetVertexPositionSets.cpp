/* 
 * Drag[en]gine IGDE Animator Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "aeUAnimatorSetVertexPositionSets.h"
#include "../../animator/aeAnimator.h"

#include <dragengine/common/exceptions.h>



// Class aeUAnimatorSetVertexPositionSets
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeUAnimatorSetVertexPositionSets::aeUAnimatorSetVertexPositionSets(
	aeAnimator *animator, const decStringSet &newValue ) :
pAnimator( animator ),
pNewValue( newValue )
{
	DEASSERT_NOTNULL( animator )
	
	pOldValue = animator->GetListVertexPositionSets();
	SetShortInfo( "Animator set vertex position sets" );
}

aeUAnimatorSetVertexPositionSets::~aeUAnimatorSetVertexPositionSets(){
}



// Management
///////////////

void aeUAnimatorSetVertexPositionSets::Undo(){
	pAnimator->SetListVertexPositionSets( pOldValue );
}

void aeUAnimatorSetVertexPositionSets::Redo(){
	pAnimator->SetListVertexPositionSets( pNewValue );
}
