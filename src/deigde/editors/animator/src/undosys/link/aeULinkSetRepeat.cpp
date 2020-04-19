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
#include <stdlib.h>
#include <string.h>

#include "aeULinkSetRepeat.h"

#include "../../animator/link/aeLink.h"

#include <dragengine/common/exceptions.h>



// Class aeULinkSetRepeat
///////////////////////////

// Constructor, destructor
////////////////////////////

aeULinkSetRepeat::aeULinkSetRepeat( aeLink *link, int newRepeat ){
	if( ! link ){
		DETHROW( deeInvalidParam );
	}
	
	pLink = NULL;
	
	SetShortInfo( "Link set repeat" );
	
	pLink = link;
	pLink->AddReference();
	
	pOldRepeat = link->GetRepeat();
	pNewRepeat = newRepeat;
}

aeULinkSetRepeat::~aeULinkSetRepeat(){
	if( pLink ){
		pLink->FreeReference();
	}
}



// Management
///////////////

void aeULinkSetRepeat::Undo(){
	pLink->SetRepeat( pOldRepeat );
}

void aeULinkSetRepeat::Redo(){
	pLink->SetRepeat( pNewRepeat );
}
