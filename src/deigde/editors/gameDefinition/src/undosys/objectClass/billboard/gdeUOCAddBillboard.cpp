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

#include "gdeUOCAddBillboard.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/billboard/gdeOCBillboard.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCAddBillboard
/////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCAddBillboard::gdeUOCAddBillboard( gdeObjectClass *objectClass, gdeOCBillboard *billboard ) :
pObjectClass( NULL ),
pBillboard( NULL )
{
	if( ! objectClass || ! billboard ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Add billboard" );
	
	pBillboard = billboard;
	billboard->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCAddBillboard::~gdeUOCAddBillboard(){
	if( pBillboard ){
		pBillboard->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCAddBillboard::Undo(){
	gdeGameDefinition * const gameDefinition = pObjectClass->GetGameDefinition();
	if( gameDefinition && gameDefinition->GetActiveOCBillboard() ){
		if( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCBillboard ){
			gameDefinition->SetSelectedObjectType( gdeGameDefinition::eotObjectClass );
		}
		gameDefinition->SetActiveOCBillboard( NULL );
	}
	
	pObjectClass->GetBillboards().Remove( pBillboard );
	pObjectClass->NotifyBillboardsChanged();
}

void gdeUOCAddBillboard::Redo(){
	pObjectClass->GetBillboards().Add( pBillboard );
	pObjectClass->NotifyBillboardsChanged();
	
	gdeGameDefinition * const gameDefinition = pObjectClass->GetGameDefinition();
	gameDefinition->SetActiveObjectClass( pObjectClass );
	gameDefinition->SetActiveOCBillboard( pBillboard );
	gameDefinition->SetSelectedObjectType( gdeGameDefinition::eotOCBillboard );
}
