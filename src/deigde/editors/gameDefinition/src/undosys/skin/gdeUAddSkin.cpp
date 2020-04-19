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

#include "gdeUAddSkin.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/skin/gdeSkin.h"

#include <dragengine/common/exceptions.h>



// Class gdeUAddSkin
//////////////////////

// Constructor, destructor
////////////////////////////

gdeUAddSkin::gdeUAddSkin( gdeGameDefinition *gameDefinition, gdeSkin *skin ) :
pGameDefinition( NULL ),
pSkin( NULL )
{
	if( ! gameDefinition || ! skin ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Add skin" );
	
	pSkin = skin;
	skin->AddReference();
	
	pGameDefinition = gameDefinition;
	gameDefinition->AddReference();
}

gdeUAddSkin::~gdeUAddSkin(){
	if( pSkin ){
		pSkin->FreeReference();
	}
	if( pGameDefinition ){
		pGameDefinition->FreeReference();
	}
}



// Management
///////////////

void gdeUAddSkin::Undo(){
	pGameDefinition->RemoveSkin( pSkin );
}

void gdeUAddSkin::Redo(){
	pGameDefinition->AddSkin( pSkin );
}
