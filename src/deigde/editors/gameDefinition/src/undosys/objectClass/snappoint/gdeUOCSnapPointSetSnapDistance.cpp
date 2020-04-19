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

#include "gdeUOCSnapPointSetSnapDistance.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/snappoint/gdeOCSnapPoint.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCSnapPointSetSnapDistance
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCSnapPointSetSnapDistance::gdeUOCSnapPointSetSnapDistance(
gdeObjectClass *objectClass, gdeOCSnapPoint *snapPoint, float newValue ) :
pObjectClass( NULL ),
pSnapPoint( NULL ),
pNewValue( newValue )
{
	if( ! objectClass || ! snapPoint ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Snap point set snap distance" );
	
	pOldValue = snapPoint->GetSnapDistance();
	
	pSnapPoint = snapPoint;
	snapPoint->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCSnapPointSetSnapDistance::~gdeUOCSnapPointSetSnapDistance(){
	if( pSnapPoint ){
		pSnapPoint->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCSnapPointSetSnapDistance::Undo(){
	pSnapPoint->SetSnapDistance( pOldValue );
	pObjectClass->NotifySnapPointChanged( pSnapPoint );
}

void gdeUOCSnapPointSetSnapDistance::Redo(){
	pSnapPoint->SetSnapDistance( pNewValue );
	pObjectClass->NotifySnapPointChanged( pSnapPoint );
}
