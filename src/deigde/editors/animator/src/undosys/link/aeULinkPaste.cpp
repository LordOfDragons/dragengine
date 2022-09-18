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

#include "aeULinkPaste.h"
#include "../../animator/controller/aeController.h"
#include "../../animator/link/aeLink.h"

#include <dragengine/common/exceptions.h>



// Class aeULinkPaste
///////////////////////

// Constructor, destructor
////////////////////////////

aeULinkPaste::aeULinkPaste( aeAnimator *animator, const aeLinkList &linkList ) :
pAnimator( animator )
{
	DEASSERT_NOTNULL( animator )
	const int linkCount = linkList.GetCount();
	DEASSERT_TRUE( linkCount > 0 )
	
	int i;
	for( i=0; i<linkCount; i++ ){
		pLinkList.Add( aeLink::Ref::New( new aeLink( *linkList.GetAt( i ) ) ) );
	}
}

aeULinkPaste::~aeULinkPaste(){
}



// Management
///////////////

void aeULinkPaste::Undo(){
	const int linkCount = pLinkList.GetCount();
	int i;
	
	// remove the links
	for( i=0; i<linkCount; i++ ){
		pAnimator->RemoveLink( pLinkList.GetAt( i ) );
	}
	
	// remove controller added in a prior redo
	const int controllerCount = pRemoveControllerList.GetCount();
	for( i=0; i<controllerCount; i++ ){
		pAnimator->RemoveController( pRemoveControllerList.GetAt( i ) );
	}
	pRemoveControllerList.RemoveAll();
}

void aeULinkPaste::Redo(){
	const aeControllerList &controllers = pAnimator->GetControllers();
	const int linkCount = pLinkList.GetCount();
	int i;
	
	pRemoveControllerList.RemoveAll();
	
	for( i=0; i<linkCount; i++ ){
		aeLink * const link = pLinkList.GetAt( i );
		
		// check if controllers exist in the animator. if not add them and mark them to
		// remove during undo. controllers are matched by name. if controller has been
		// added modify all links. this should be safe since only pasted links not present
		// in the animator can be potentially modified
		aeController * const controller = link->GetController();
		if( controller && ! controllers.Has( controller ) ){
			aeController * const sameNameController = controllers.GetNamed( controller->GetName() );
			
			if( sameNameController ){
				link->SetController( sameNameController, false );
				
			}else{
				link->SetController( controller, false );
				pRemoveControllerList.Add( controller );
				pAnimator->AddController( controller );
			}
		}
		
		// insert link
		pAnimator->AddLink( link );
	}
	
	pAnimator->SetActiveLink( pLinkList.GetAt( linkCount - 1 ) );
}
