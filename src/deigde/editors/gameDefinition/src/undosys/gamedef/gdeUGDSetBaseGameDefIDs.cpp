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

#include "gdeUGDSetBaseGameDefIDs.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gui/gdeWindowMain.h"

#include <dragengine/common/exceptions.h>



// Class gdeUGDSetBaseGameDefIDs
//////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUGDSetBaseGameDefIDs::gdeUGDSetBaseGameDefIDs( gdeWindowMain &windowMain,
gdeGameDefinition *gameDefinition, const decStringList &newValue ) :
pWindowMain( windowMain ),
pGameDefinition( NULL ),
pNewValue( newValue )
{
	if( ! gameDefinition ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Game definition set base game definition IDs" );
	
	pOldValue = gameDefinition->GetBaseGameDefinitionIDList();
	
	pGameDefinition = gameDefinition;
	gameDefinition->AddReference();
}

gdeUGDSetBaseGameDefIDs::~gdeUGDSetBaseGameDefIDs(){
	if( pGameDefinition ){
		pGameDefinition->FreeReference();
	}
}



// Management
///////////////

void gdeUGDSetBaseGameDefIDs::Undo(){
	pGameDefinition->SetBaseGameDefinitionIDList( pOldValue );
	pGameDefinition->UpdateBaseGameDefinitions( pWindowMain.GetLoadSaveSystem() );
}

void gdeUGDSetBaseGameDefIDs::Redo(){
	pGameDefinition->SetBaseGameDefinitionIDList( pNewValue );
	pGameDefinition->UpdateBaseGameDefinitions( pWindowMain.GetLoadSaveSystem() );
}
