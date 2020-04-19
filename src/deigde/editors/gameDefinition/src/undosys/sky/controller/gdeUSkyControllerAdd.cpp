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

#include "gdeUSkyControllerAdd.h"
#include "../../../gamedef/sky/gdeSky.h"
#include "../../../gamedef/sky/gdeSkyController.h"

#include <dragengine/common/exceptions.h>



// Class gdeUSkyControllerAdd
///////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUSkyControllerAdd::gdeUSkyControllerAdd( gdeSky *sky, gdeSkyController *controller ) :
pSky( NULL ),
pController( NULL )
{
	if( ! sky || ! controller ){
		DETHROW( deeInvalidParam );
	}
	
	if( sky->GetControllers().Has( controller ) ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Add sky controller" );
	
	pSky = sky;
	sky->AddReference();
	
	pController = controller;
	controller->AddReference();
}

gdeUSkyControllerAdd::~gdeUSkyControllerAdd(){
	if( pController ){
		pController->FreeReference();
	}
	if( pSky ){
		pSky->FreeReference();
	}
}



// Management
///////////////

void gdeUSkyControllerAdd::Undo(){
	pSky->RemoveController( pController );
}

void gdeUSkyControllerAdd::Redo(){
	pSky->AddController( pController );
}
