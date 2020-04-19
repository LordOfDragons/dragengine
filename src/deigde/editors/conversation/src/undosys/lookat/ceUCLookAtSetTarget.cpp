/* 
 * Drag[en]gine IGDE Conversation Editor
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

#include "ceUCLookAtSetTarget.h"
#include "../../conversation/lookat/ceLookAt.h"

#include <dragengine/common/exceptions.h>



// Class ceUCLookAtSetTarget
//////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCLookAtSetTarget::ceUCLookAtSetTarget( ceLookAt *lookat, const char *newTarget ){
	if( ! lookat || ! newTarget ) DETHROW( deeInvalidParam );
	
	pLookAt = NULL;
	
	SetShortInfo( "LookAt Set Target" );
	
	pOldTarget = lookat->GetTarget();
	pNewTarget = newTarget;
	
	pLookAt = lookat;
	lookat->AddReference();
}

ceUCLookAtSetTarget::~ceUCLookAtSetTarget(){
	if( pLookAt ){
		pLookAt->FreeReference();
	}
}



// Management
///////////////

void ceUCLookAtSetTarget::Undo(){
	pLookAt->SetTarget( pOldTarget );
}

void ceUCLookAtSetTarget::Redo(){
	pLookAt->SetTarget( pNewTarget );
}
