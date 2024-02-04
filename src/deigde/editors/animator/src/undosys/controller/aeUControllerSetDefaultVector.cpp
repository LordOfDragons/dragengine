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

#include "aeUControllerSetDefaultVector.h"
#include "../../animator/aeAnimator.h"

#include <dragengine/common/exceptions.h>



// Class aeUControllerSetDefaultVector
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeUControllerSetDefaultVector::aeUControllerSetDefaultVector(
	aeController *controller, const decVector &newVector ) :
pController( controller ),
pNewVector( newVector )
{
	DEASSERT_NOTNULL( controller )
	DEASSERT_NOTNULL( controller->GetAnimator() )
	
	pOldVector = controller->GetDefaultVector();
	SetShortInfo( "Set controller default vector" );
}

aeUControllerSetDefaultVector::~aeUControllerSetDefaultVector(){
}



// Management
///////////////

void aeUControllerSetDefaultVector::Undo(){
	pController->SetDefaultVector( pOldVector );
}

void aeUControllerSetDefaultVector::Redo(){
	pController->SetDefaultVector( pNewVector );
}
