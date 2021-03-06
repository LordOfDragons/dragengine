/* 
 * Drag[en]gine IGDE Sky Editor
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

#include "seULinkSetController.h"
#include "../../sky/seSky.h"
#include "../../sky/controller/seController.h"
#include "../../sky/link/seLink.h"

#include <dragengine/common/exceptions.h>



// Class seULinkSetController
///////////////////////////////

// Constructor, destructor
////////////////////////////

seULinkSetController::seULinkSetController( seLink *link, seController *newController ) :
pLink( NULL ),
pOldController( NULL ),
pNewController( NULL )
{
	if( ! link ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Set Link Controller" );
	
	pOldController = link->GetController();
	if( pOldController ){
		pOldController->AddReference();
	}
	
	pNewController = newController;
	if( pNewController ){
		pNewController->AddReference();
	}
	
	pLink = link;
	link->AddReference();
}

seULinkSetController::~seULinkSetController(){
	if( pOldController ){
		pOldController->FreeReference();
	}
	if( pNewController ){
		pNewController->FreeReference();
	}
	if( pLink ){
		pLink->FreeReference();
	}
}



// Management
///////////////

void seULinkSetController::Undo(){
	pLink->SetController( pOldController );
}

void seULinkSetController::Redo(){
	pLink->SetController( pNewController );
}
