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

#include "gdeUOCAddForceField.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/forceField/gdeOCForceField.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCAddForceField
//////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCAddForceField::gdeUOCAddForceField( gdeObjectClass *objectClass, gdeOCForceField *forceField ) :
pObjectClass( NULL ),
pForceField( NULL )
{
	if( ! objectClass || ! forceField ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Add force field" );
	
	pForceField = forceField;
	forceField->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCAddForceField::~gdeUOCAddForceField(){
	if( pForceField ){
		pForceField->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCAddForceField::Undo(){
	gdeGameDefinition * const gameDefinition = pObjectClass->GetGameDefinition();
	if( gameDefinition && gameDefinition->GetActiveOCForceField() ){
		if( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCForceField ){
			gameDefinition->SetSelectedObjectType( gdeGameDefinition::eotObjectClass );
		}
		gameDefinition->SetActiveOCForceField( NULL );
	}
	
	pObjectClass->GetForceFields().Remove( pForceField );
	pObjectClass->NotifyForceFieldsChanged();
}

void gdeUOCAddForceField::Redo(){
	pObjectClass->GetForceFields().Add( pForceField );
	pObjectClass->NotifyForceFieldsChanged();
}
