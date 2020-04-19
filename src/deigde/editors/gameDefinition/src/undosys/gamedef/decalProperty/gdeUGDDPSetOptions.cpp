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

#include "gdeUGDDPSetOptions.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/property/gdeProperty.h"

#include <dragengine/common/exceptions.h>



// Class gdeUGDDPSetOptions
////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUGDDPSetOptions::gdeUGDDPSetOptions( gdeGameDefinition *gamedef,
gdeProperty *property, const decStringList &newValue ) :
pGameDefinition( NULL ),
pProperty( NULL )
{
	if( ! gamedef || ! property ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Game definition property set options" );
	
	pOldValue = property->GetOptions();
	pNewValue = newValue;
	
	pGameDefinition = gamedef;
	gamedef->AddReference();
	
	pProperty = property;
	property->AddReference();
}

gdeUGDDPSetOptions::~gdeUGDDPSetOptions(){
	if( pProperty ){
		pProperty->FreeReference();
	}
	if( pGameDefinition ){
		pGameDefinition->FreeReference();
	}
}



// Management
///////////////

void gdeUGDDPSetOptions::Undo(){
	pProperty->GetOptions() = pOldValue;
	pGameDefinition->NotifyDecalPropertyChanged( pProperty );
}

void gdeUGDDPSetOptions::Redo(){
	pProperty->GetOptions() = pNewValue;
	pGameDefinition->NotifyDecalPropertyChanged( pProperty );
}
