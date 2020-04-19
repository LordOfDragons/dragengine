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

#include "gdeUOCNavBlockerSetPosition.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/navblocker/gdeOCNavigationBlocker.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCNavBlockerSetPosition
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCNavBlockerSetPosition::gdeUOCNavBlockerSetPosition( gdeObjectClass *objectClass,
gdeOCNavigationBlocker *navblocker, const decVector &newValue ) :
pObjectClass( NULL ),
pNavBlocker( NULL )
{
	if( ! objectClass || ! navblocker ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Nav-blocker set position" );
	
	pOldValue = navblocker->GetPosition();
	pNewValue = newValue;
	
	pNavBlocker = navblocker;
	navblocker->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCNavBlockerSetPosition::~gdeUOCNavBlockerSetPosition(){
	if( pNavBlocker ){
		pNavBlocker->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCNavBlockerSetPosition::Undo(){
	pNavBlocker->SetPosition( pOldValue );
	pObjectClass->NotifyNavigationBlockerChanged( pNavBlocker );
}

void gdeUOCNavBlockerSetPosition::Redo(){
	pNavBlocker->SetPosition( pNewValue );
	pObjectClass->NotifyNavigationBlockerChanged( pNavBlocker );
}
