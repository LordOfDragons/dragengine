/* 
 * Drag[en]gine IGDE Speech Animation Editor
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

#include "saeUSAnimSetNeutralMoveName.h"
#include "../../sanimation/saeSAnimation.h"

#include <dragengine/common/exceptions.h>



// Class saeUSAnimSetNeutralMoveName
////////////////////////////////

// Constructor, destructor
////////////////////////////

saeUSAnimSetNeutralMoveName::saeUSAnimSetNeutralMoveName( saeSAnimation *sanimation, const char *newName ){
	if( ! sanimation || ! newName ) DETHROW( deeInvalidParam );
	
	pSAnimation = NULL;
	
	SetShortInfo( "Set Neutral Move Name" );
	
	pOldName = sanimation->GetNeutralMoveName();
	pNewName = newName;
	
	pSAnimation = sanimation;
	sanimation->AddReference();
}

saeUSAnimSetNeutralMoveName::~saeUSAnimSetNeutralMoveName(){
	if( pSAnimation ){
		pSAnimation->FreeReference();
	}
}



// Management
///////////////

void saeUSAnimSetNeutralMoveName::Undo(){
	pSAnimation->SetNeutralMoveName( pOldName.GetString() );
}

void saeUSAnimSetNeutralMoveName::Redo(){
	pSAnimation->SetNeutralMoveName( pNewName.GetString() );
}
