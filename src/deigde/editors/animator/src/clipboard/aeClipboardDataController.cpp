/* 
 * Drag[en]gine IGDE Animator Editor
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

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "aeClipboardDataController.h"
#include "../animator/controller/aeController.h"

#include <dragengine/common/exceptions.h>



// Class aeClipboardDataController
////////////////////////////////////

const char * const aeClipboardDataController::TYPE_NAME = "controller";

// Constructor, destructor
////////////////////////////

aeClipboardDataController::aeClipboardDataController( aeController *controller ) :
igdeClipboardData( TYPE_NAME )
{
	pControllers.Add( aeController::Ref::New( new aeController( *controller ) ) );
}

aeClipboardDataController::aeClipboardDataController( const aeControllerList &controllers ) :
igdeClipboardData( TYPE_NAME )
{
	const int count = controllers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pControllers.Add( aeController::Ref::New( new aeController( *controllers.GetAt( i ) ) ) );
	}
}

aeClipboardDataController::~aeClipboardDataController(){
	pControllers.RemoveAll();
}
