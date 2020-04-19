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

#include "gdeUOCTPSetIdentifierGroup.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/property/gdeProperty.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCTPSetIdentifierGroup
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCTPSetIdentifierGroup::gdeUOCTPSetIdentifierGroup(
gdeObjectClass *objectClass, gdeProperty *property, const char *newValue ) :
pObjectClass( NULL ),
pProperty( NULL )
{
	if( ! objectClass || ! property ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Object class texture property set identifier group" );
	
	pOldValue = property->GetIdentifierGroup();
	pNewValue = newValue;
	
	pObjectClass = objectClass;
	objectClass->AddReference();
	
	pProperty = property;
	property->AddReference();
}

gdeUOCTPSetIdentifierGroup::~gdeUOCTPSetIdentifierGroup(){
	if( pProperty ){
		pProperty->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCTPSetIdentifierGroup::Undo(){
	pProperty->SetIdentifierGroup( pOldValue );
	pObjectClass->NotifyTexturePropertyChanged( pProperty );
}

void gdeUOCTPSetIdentifierGroup::Redo(){
	pProperty->SetIdentifierGroup( pNewValue );
	pObjectClass->NotifyTexturePropertyChanged( pProperty );
}
