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

#include "gdeURemoveObjectClass.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/objectClass/gdeObjectClass.h"

#include <dragengine/common/exceptions.h>



// Class gdeURemoveObjectClass
////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeURemoveObjectClass::gdeURemoveObjectClass( gdeGameDefinition *gameDefinition, gdeObjectClass *objectClass ) :
pGameDefinition( NULL ),
pObjectClass( NULL )
{
	if( ! gameDefinition || ! objectClass ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! gameDefinition->GetObjectClasses().Has( objectClass ) ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Remove object class" );
	
	pObjectClass = objectClass;
	objectClass->AddReference();
	
	pGameDefinition = gameDefinition;
	gameDefinition->AddReference();
}

gdeURemoveObjectClass::~gdeURemoveObjectClass(){
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
	if( pGameDefinition ){
		pGameDefinition->FreeReference();
	}
}



// Management
///////////////

void gdeURemoveObjectClass::Undo(){
	pGameDefinition->AddObjectClass( pObjectClass );
}

void gdeURemoveObjectClass::Redo(){
	pGameDefinition->RemoveObjectClass( pObjectClass );
}
