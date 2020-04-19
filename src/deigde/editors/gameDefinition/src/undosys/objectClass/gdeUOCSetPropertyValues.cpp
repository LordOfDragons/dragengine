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

#include "gdeUOCSetPropertyValues.h"
#include "../../gamedef/objectClass/gdeObjectClass.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCSetProperyValues
/////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCSetPropertyValues::gdeUOCSetPropertyValues( gdeObjectClass *objectClass,
const decStringDictionary &newValues ) :
pObjectClass( NULL )
{
	if( ! objectClass ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Object class set property values" );
	
	pOldValues = objectClass->GetPropertyValues();
	pNewValues = newValues;
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCSetPropertyValues::~gdeUOCSetPropertyValues(){
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCSetPropertyValues::Undo(){
	pObjectClass->GetPropertyValues() = pOldValues;
	pObjectClass->NotifyPropertyValuesChanged();
}

void gdeUOCSetPropertyValues::Redo(){
	pObjectClass->GetPropertyValues() = pNewValues;
	pObjectClass->NotifyPropertyValuesChanged();
}
