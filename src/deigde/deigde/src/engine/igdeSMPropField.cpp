/* 
 * Drag[en]gine IGDE
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
#include <stdlib.h>
#include <string.h>

#include "igdeSMPropField.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class igdeSMPropField
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeSMPropField::igdeSMPropField(){
	pDelegee = NULL;
}

igdeSMPropField::~igdeSMPropField(){
}



// Management
///////////////

void igdeSMPropField::SetDelegee( deBaseScriptingPropField *delegee ){
	pDelegee = delegee;
}



// Notifications
//////////////////

void igdeSMPropField::CreateInstances( float density ){
	if( pDelegee ){
		pDelegee->CreateInstances( density );
	}
}
