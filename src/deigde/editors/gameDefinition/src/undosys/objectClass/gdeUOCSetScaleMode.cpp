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

#include "gdeUOCSetScaleMode.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCSetScaleMode
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCSetScaleMode::gdeUOCSetScaleMode( gdeObjectClass *objectClass,
gdeObjectClass::eScaleModes newValue ) :
pObjectClass( NULL )
{
	if( ! objectClass ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Object class set scale mode" );
	
	pOldValue = objectClass->GetScaleMode();
	pNewValue = newValue;
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCSetScaleMode::~gdeUOCSetScaleMode(){
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCSetScaleMode::Undo(){
	pObjectClass->SetScaleMode( pOldValue );
}

void gdeUOCSetScaleMode::Redo(){
	pObjectClass->SetScaleMode( pNewValue );
}
