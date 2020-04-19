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

#include "seULinkAdd.h"
#include "../../sky/seSky.h"
#include "../../sky/link/seLink.h"

#include <dragengine/common/exceptions.h>



// Class seULinkAdd
/////////////////////

// Constructor, destructor
////////////////////////////

seULinkAdd::seULinkAdd( seSky *sky, seLink *link ) :
pSky( NULL ),
pLink( NULL )
{
	if( ! sky || ! link ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Add Link" );
	
	pSky = sky;
	sky->AddReference();
	
	pLink = link;
	link->AddReference();
}

seULinkAdd::~seULinkAdd(){
	if( pLink ){
		pLink->FreeReference();
	}
	if( pSky ){
		pSky->FreeReference();
	}
}



// Management
///////////////

void seULinkAdd::Undo(){
	pSky->RemoveLink( pLink );
}

void seULinkAdd::Redo(){
	pSky->AddLink( pLink );
	pSky->SetActiveLink( pLink );
}
