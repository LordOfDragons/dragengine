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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ceLookAt.h"
#include "../ceConversation.h"
#include "../actor/ceConversationActor.h"

#include <dragengine/common/exceptions.h>



// Class ceLookAt
///////////////////

// Constructor, destructor
////////////////////////////

ceLookAt::ceLookAt( const char *name ) :
pConversation( NULL ),
pName( name ){
}

ceLookAt::ceLookAt( const ceLookAt &target ){
	pConversation = NULL;
	pName = target.GetName();
	pTarget = target.GetTarget();
}

ceLookAt::~ceLookAt(){
}



// Management
///////////////

void ceLookAt::SetConversation( ceConversation *conversation ){
	pConversation = conversation;
}

void ceLookAt::SetName( const char *name ){
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! pName.Equals( name ) ){
		if( pConversation && pConversation->GetLookAtList().HasNamed( name ) ){
			DETHROW( deeInvalidParam );
		}
		
		pName = name;
		
		if( pConversation ){
			pConversation->NotifyLookAtChanged( this );
		}
	}
}

void ceLookAt::SetTarget( const char *target ){
	if( ! target ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! pTarget.Equals( target ) ){
		pTarget = target;
		
		if( pConversation ){
			pConversation->NotifyLookAtChanged( this );
		}
	}
}
