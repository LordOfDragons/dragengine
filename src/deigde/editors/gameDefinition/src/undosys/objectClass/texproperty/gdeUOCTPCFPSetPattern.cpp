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

#include "gdeUOCTPCFPSetPattern.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/property/gdeProperty.h"
#include "../../../gamedef/filepattern/gdeFilePattern.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCTPCFPSetPattern
////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCTPCFPSetPattern::gdeUOCTPCFPSetPattern( gdeObjectClass *objectClass,
gdeProperty *property, gdeFilePattern *filePattern, const char *newValue ) :
pObjectClass( NULL ),
pProperty( NULL ),
pFilePattern( NULL )
{
	if( ! objectClass || ! property || ! filePattern ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Object class texture property custom file pattern set pattern" );
	
	pOldValue = filePattern->GetPattern();
	pNewValue = newValue;
	
	pObjectClass = objectClass;
	objectClass->AddReference();
	
	pProperty = property;
	property->AddReference();
	
	pFilePattern = filePattern;
	filePattern->AddReference();
}

gdeUOCTPCFPSetPattern::~gdeUOCTPCFPSetPattern(){
	if( pFilePattern ){
		pFilePattern->FreeReference();
	}
	if( pProperty ){
		pProperty->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCTPCFPSetPattern::Undo(){
	pFilePattern->SetPattern( pOldValue );
	pObjectClass->NotifyTexturePropertyChanged( pProperty );
}

void gdeUOCTPCFPSetPattern::Redo(){
	pFilePattern->SetPattern( pNewValue );
	pObjectClass->NotifyTexturePropertyChanged( pProperty );
}
