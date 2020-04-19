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

#include "gdeUGDWPCFPSetName.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/property/gdeProperty.h"
#include "../../../gamedef/filepattern/gdeFilePattern.h"

#include <dragengine/common/exceptions.h>



// Class gdeUGDWPCFPSetName
////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUGDWPCFPSetName::gdeUGDWPCFPSetName( gdeGameDefinition *gamedef,
gdeProperty *property, gdeFilePattern *filePattern, const char *newValue ) :
pGameDefinition( NULL ),
pProperty( NULL ),
pFilePattern( NULL )
{
	if( ! gamedef || ! property || ! filePattern ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Game definition property custom file pattern set name" );
	
	pOldValue = filePattern->GetName();
	pNewValue = newValue;
	
	pGameDefinition = gamedef;
	gamedef->AddReference();
	
	pProperty = property;
	property->AddReference();
	
	pFilePattern = filePattern;
	filePattern->AddReference();
}

gdeUGDWPCFPSetName::~gdeUGDWPCFPSetName(){
	if( pFilePattern ){
		pFilePattern->FreeReference();
	}
	if( pProperty ){
		pProperty->FreeReference();
	}
	if( pGameDefinition ){
		pGameDefinition->FreeReference();
	}
}



// Management
///////////////

void gdeUGDWPCFPSetName::Undo(){
	pFilePattern->SetName( pOldValue );
	pGameDefinition->NotifyWorldPropertyChanged( pProperty );
}

void gdeUGDWPCFPSetName::Redo(){
	pFilePattern->SetName( pNewValue );
	pGameDefinition->NotifyWorldPropertyChanged( pProperty );
}
