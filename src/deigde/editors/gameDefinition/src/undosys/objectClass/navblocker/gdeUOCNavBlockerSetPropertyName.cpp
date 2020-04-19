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

#include "gdeUOCNavBlockerSetPropertyName.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCNavBlockerSetPropertyName
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCNavBlockerSetPropertyName::gdeUOCNavBlockerSetPropertyName( gdeObjectClass *objectClass,
gdeOCNavigationBlocker *navblocker, gdeOCNavigationBlocker::eProperties property, const char *newValue ) :
pObjectClass( NULL ),
pNavBlocker( NULL ),
pProperty( property )
{
	if( ! objectClass || ! navblocker ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "NavBlocker set property name" );
	
	pOldValue = navblocker->GetPropertyName( property );
	pNewValue = newValue;
	
	pNavBlocker = navblocker;
	navblocker->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCNavBlockerSetPropertyName::~gdeUOCNavBlockerSetPropertyName(){
	if( pNavBlocker ){
		pNavBlocker->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCNavBlockerSetPropertyName::Undo(){
	pNavBlocker->SetPropertyName( pProperty, pOldValue );
	pObjectClass->NotifyNavigationBlockerChanged( pNavBlocker );
}

void gdeUOCNavBlockerSetPropertyName::Redo(){
	pNavBlocker->SetPropertyName( pProperty, pNewValue );
	pObjectClass->NotifyNavigationBlockerChanged( pNavBlocker );
}
