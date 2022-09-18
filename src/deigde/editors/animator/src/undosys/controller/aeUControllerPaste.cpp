/* 
 * Drag[en]gine IGDE Animator Editor
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
#include <string.h>
#include <stdlib.h>

#include "aeUControllerPaste.h"
#include "../../animator/controller/aeController.h"

#include <dragengine/common/exceptions.h>



// Class aeUControllerPaste
/////////////////////////////

// Constructor, destructor
////////////////////////////

aeUControllerPaste::aeUControllerPaste( aeAnimator *animator, const aeControllerList &controllerList ) :
pAnimator( animator )
{
	DEASSERT_NOTNULL( animator )
	const int controllerCount = controllerList.GetCount();
	DEASSERT_TRUE( controllerCount > 0 )
	
	int i;
	for( i=0; i<controllerCount; i++ ){
		pControllerList.Add( aeController::Ref::New( new aeController( *controllerList.GetAt( i ) ) ) );
	}
	
	pIndex = animator->GetControllers().GetCount();
}

aeUControllerPaste::~aeUControllerPaste(){
}



// Management
///////////////

void aeUControllerPaste::Undo(){
	const int controllerCount = pControllerList.GetCount();
	int i;
	
	for( i=0; i<controllerCount; i++ ){
		pAnimator->RemoveController( pControllerList.GetAt( i ) );
	}
}

void aeUControllerPaste::Redo(){
	const int controllerCount = pControllerList.GetCount();
	int i;
	
	for( i=0; i<controllerCount; i++ ){
		pAnimator->InsertControllerAt( pControllerList.GetAt( i ), pIndex + i );
	}
	
	pAnimator->SetActiveController( pControllerList.GetAt( controllerCount - 1 ) );
}
