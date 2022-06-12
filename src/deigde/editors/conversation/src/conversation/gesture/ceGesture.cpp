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

#include "ceGesture.h"
#include "../ceConversation.h"

#include <dragengine/common/exceptions.h>



// Class ceGesture
////////////////////

// Constructor, destructor
////////////////////////////

ceGesture::ceGesture( const char *name ) :
pConversation( NULL ),
pName( name ),
pHold( false ),
pDuration ( 2.0f ){
}

ceGesture::~ceGesture(){
}



// Management
///////////////

void ceGesture::SetConversation( ceConversation *conversation ){
	pConversation = conversation;
}

void ceGesture::SetName( const char *name ){
	if( pName.Equals( name ) ){
		return;
	}
	
	if( pConversation && pConversation->GetGestureList().HasNamed( name ) ){
		DETHROW( deeInvalidParam );
	}
	
	pName = name;
	
	if( pConversation ){
		pConversation->NotifyGestureChanged( this );
	}
}

void ceGesture::SetAnimator( const char *animator ){
	if( pAnimator.Equals( animator ) ){
		return;
	}
	
	pAnimator = animator;
	
	if( pConversation ){
		pConversation->NotifyGestureChanged( this );
	}
}

void ceGesture::SetHold( bool hold ){
	if( hold == pHold ){
		return;
	}
	
	pHold = hold;
	
	if( pConversation ){
		pConversation->NotifyGestureChanged( this );
	}
}

void ceGesture::SetDuration( float duration ){
	if( fabsf( duration - pDuration ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pDuration = duration;
	
	if( pConversation ){
		pConversation->NotifyGestureChanged( this );
	}
}
