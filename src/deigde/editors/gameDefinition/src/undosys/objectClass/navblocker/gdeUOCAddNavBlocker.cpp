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

#include "gdeUOCAddNavBlocker.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/navblocker/gdeOCNavigationBlocker.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCAddNavBlocker
//////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCAddNavBlocker::gdeUOCAddNavBlocker( gdeObjectClass *objectClass, gdeOCNavigationBlocker *navblocker ) :
pObjectClass( NULL ),
pNavBlocker( NULL )
{
	if( ! objectClass || ! navblocker ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Add navblocker" );
	
	pNavBlocker = navblocker;
	navblocker->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCAddNavBlocker::~gdeUOCAddNavBlocker(){
	if( pNavBlocker ){
		pNavBlocker->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCAddNavBlocker::Undo(){
	gdeGameDefinition * const gameDefinition = pObjectClass->GetGameDefinition();
	if( gameDefinition && gameDefinition->GetActiveOCNavigationBlocker() ){
		if( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCNavigationBlocker ){
			gameDefinition->SetSelectedObjectType( gdeGameDefinition::eotObjectClass );
		}
		gameDefinition->SetActiveOCNavigationBlocker( NULL );
	}
	
	pObjectClass->GetNavigationBlockers().Remove( pNavBlocker );
	pObjectClass->NotifyNavigationBlockersChanged();
}

void gdeUOCAddNavBlocker::Redo(){
	pObjectClass->GetNavigationBlockers().Add( pNavBlocker );
	pObjectClass->NotifyNavigationBlockersChanged();
}
