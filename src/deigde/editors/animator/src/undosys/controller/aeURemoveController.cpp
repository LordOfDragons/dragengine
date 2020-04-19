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

#include "aeURemoveController.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/controller/aeController.h"
#include "../../animator/link/aeLink.h"

#include <dragengine/common/exceptions.h>



// Class aeURemoveController
//////////////////////////////

// Constructor, destructor
////////////////////////////

aeURemoveController::aeURemoveController( aeAnimator *animator, aeController *controller ) :
pAnimator( NULL ),
pController( NULL ),
pIndex( -1 )
{
	if( ! animator || ! controller ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Remove controller" );
	
	pIndex = animator->GetControllers().IndexOf( controller );
	if( pIndex == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	const aeLinkList &links = animator->GetLinks();
	const int count = links.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		aeLink * const link = links.GetAt( i );
		if( link->GetController() == controller ){
			pLinksUsingController.Add( link );
		}
	}
	
	pAnimator = animator;
	animator->AddReference();
	
	pController = controller;
	controller->AddReference();
}

aeURemoveController::~aeURemoveController(){
	pCleanUp();
}



// Management
///////////////

void aeURemoveController::Undo(){
	pAnimator->InsertControllerAt( pController, pIndex );
	
	const int count = pLinksUsingController.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pLinksUsingController.GetAt( i )->SetController( pController, false );
	}
}

void aeURemoveController::Redo(){
	const int count = pLinksUsingController.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pLinksUsingController.GetAt( i )->SetController( NULL, false );
	}
	
	pAnimator->RemoveController( pController );
}



// Private Functions
//////////////////////

void aeURemoveController::pCleanUp(){
	if( pController ){
		pController->FreeReference();
	}
	if( pAnimator ){
		pAnimator->FreeReference();
	}
}
