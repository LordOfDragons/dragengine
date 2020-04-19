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

#include "gdeUOCTPSetPathPatternType.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCTPSetPathPatternType
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCTPSetPathPatternType::gdeUOCTPSetPathPatternType( gdeObjectClass *objectClass,
gdeProperty *property, gdeProperty::ePathPatternTypes newValue ) :
pObjectClass( NULL ),
pProperty( NULL )
{
	if( ! objectClass || ! property ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Object class texture property set path pattern type" );
	
	pOldValue = property->GetPathPatternType();
	pNewValue = newValue;
	
	pObjectClass = objectClass;
	objectClass->AddReference();
	
	pProperty = property;
	property->AddReference();
}

gdeUOCTPSetPathPatternType::~gdeUOCTPSetPathPatternType(){
	if( pProperty ){
		pProperty->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCTPSetPathPatternType::Undo(){
	pProperty->SetPathPatternType( pOldValue );
	pObjectClass->NotifyTexturePropertyChanged( pProperty );
}

void gdeUOCTPSetPathPatternType::Redo(){
	pProperty->SetPathPatternType( pNewValue );
	pObjectClass->NotifyTexturePropertyChanged( pProperty );
}
