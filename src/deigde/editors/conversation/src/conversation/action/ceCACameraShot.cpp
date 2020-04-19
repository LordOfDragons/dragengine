/* 
 * Drag[en]gine IGDE Conversation Editor
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

#include "ceCACameraShot.h"

#include <dragengine/common/exceptions.h>



// Class ceCACameraShot
/////////////////////////

// Constructor, destructor
////////////////////////////

ceCACameraShot::ceCACameraShot() : ceConversationAction( eatCameraShot ){
	pDuration = 5.0f;
}

ceCACameraShot::ceCACameraShot( const ceCACameraShot &action ) : ceConversationAction( action ){
	pName = action.GetName();
	pDuration = action.GetDuration();
	pCameraTarget = action.GetCameraTarget();
	pLookAtTarget = action.GetLookAtTarget();
}

ceCACameraShot::~ceCACameraShot(){
}



// Management
///////////////

void ceCACameraShot::SetName( const char *name ){
	if( ! name ) DETHROW( deeInvalidParam );
	
	pName = name;
}

void ceCACameraShot::SetDuration( float duration ){
	pDuration = duration;
}

void ceCACameraShot::SetCameraTarget( const char *target ){
	if( ! target ){
		DETHROW( deeInvalidParam );
	}
	
	pCameraTarget = target;
}

void ceCACameraShot::SetLookAtTarget( const char *target ){
	if( ! target ){
		DETHROW( deeInvalidParam );
	}
	
	pLookAtTarget = target;
}



ceConversationAction *ceCACameraShot::CreateCopy() const{
	return new ceCACameraShot( *this );
}
