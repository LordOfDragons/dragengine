/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeUAddSky.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/sky/gdeSky.h"

#include <dragengine/common/exceptions.h>



// Class gdeUAddSky
/////////////////////

// Constructor, destructor
////////////////////////////

gdeUAddSky::gdeUAddSky( gdeGameDefinition *gameDefinition, gdeSky *sky ) :
pGameDefinition( NULL ),
pSky( NULL )
{
	if( ! gameDefinition || ! sky ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Add sky" );
	
	pSky = sky;
	sky->AddReference();
	
	pGameDefinition = gameDefinition;
	gameDefinition->AddReference();
}

gdeUAddSky::~gdeUAddSky(){
	if( pSky ){
		pSky->FreeReference();
	}
	if( pGameDefinition ){
		pGameDefinition->FreeReference();
	}
}



// Management
///////////////

void gdeUAddSky::Undo(){
	pGameDefinition->RemoveSky( pSky );
}

void gdeUAddSky::Redo(){
	pGameDefinition->AddSky( pSky );
}
