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

#include "gdeUOCComponentSetPropertyName.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCComponentSetPropertyName
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCComponentSetPropertyName::gdeUOCComponentSetPropertyName( gdeObjectClass *objectClass,
gdeOCComponent *component, gdeOCComponent::eProperties property, const char *newValue ) :
pObjectClass( NULL ),
pComponent( NULL ),
pProperty( property )
{
	if( ! objectClass || ! component ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Component set property name" );
	
	pOldValue = component->GetPropertyName( property );
	pNewValue = newValue;
	
	pComponent = component;
	component->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCComponentSetPropertyName::~gdeUOCComponentSetPropertyName(){
	if( pComponent ){
		pComponent->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCComponentSetPropertyName::Undo(){
	pComponent->SetPropertyName( pProperty, pOldValue );
	pObjectClass->NotifyComponentChanged( pComponent );
}

void gdeUOCComponentSetPropertyName::Redo(){
	pComponent->SetPropertyName( pProperty, pNewValue );
	pObjectClass->NotifyComponentChanged( pComponent );
}
