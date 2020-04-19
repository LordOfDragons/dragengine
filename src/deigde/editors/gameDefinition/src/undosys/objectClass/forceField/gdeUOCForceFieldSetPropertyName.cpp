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

#include "gdeUOCForceFieldSetPropertyName.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCForceFieldSetPropertyName
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCForceFieldSetPropertyName::gdeUOCForceFieldSetPropertyName( gdeObjectClass *objectClass,
gdeOCForceField *forceField, gdeOCForceField::eProperties property, const char *newValue ) :
pObjectClass( NULL ),
pForceField( NULL ),
pProperty( property )
{
	if( ! objectClass || ! forceField ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Force field set property name" );
	
	pOldValue = forceField->GetPropertyName( property );
	pNewValue = newValue;
	
	pForceField = forceField;
	forceField->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCForceFieldSetPropertyName::~gdeUOCForceFieldSetPropertyName(){
	if( pForceField ){
		pForceField->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCForceFieldSetPropertyName::Undo(){
	pForceField->SetPropertyName( pProperty, pOldValue );
	pObjectClass->NotifyForceFieldChanged( pForceField );
}

void gdeUOCForceFieldSetPropertyName::Redo(){
	pForceField->SetPropertyName( pProperty, pNewValue );
	pObjectClass->NotifyForceFieldChanged( pForceField );
}
