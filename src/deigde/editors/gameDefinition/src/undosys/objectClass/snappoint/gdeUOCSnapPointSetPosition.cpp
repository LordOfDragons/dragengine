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

#include "gdeUOCSnapPointSetPosition.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/snappoint/gdeOCSnapPoint.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCSnapPointSetPosition
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCSnapPointSetPosition::gdeUOCSnapPointSetPosition( gdeObjectClass *objectClass,
gdeOCSnapPoint *snapPoint, const decVector &newValue ) :
pObjectClass( NULL ),
pSnapPoint( NULL )
{
	if( ! objectClass || ! snapPoint ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Snap point set position" );
	
	pOldValue = snapPoint->GetPosition();
	pNewValue = newValue;
	
	pSnapPoint = snapPoint;
	snapPoint->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCSnapPointSetPosition::~gdeUOCSnapPointSetPosition(){
	if( pSnapPoint ){
		pSnapPoint->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCSnapPointSetPosition::Undo(){
	pSnapPoint->SetPosition( pOldValue );
	pObjectClass->NotifySnapPointChanged( pSnapPoint );
}

void gdeUOCSnapPointSetPosition::Redo(){
	pSnapPoint->SetPosition( pNewValue );
	pObjectClass->NotifySnapPointChanged( pSnapPoint );
}
