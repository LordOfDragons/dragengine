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

#include "ceStrip.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class ceStrip
//////////////////

// Constructor, destructor
////////////////////////////

ceStrip::ceStrip(){
	pPause = 0.0f,
	pDuration = 1.0f;
}

ceStrip::ceStrip( const char *id, float duration, float pause ){
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	pID = id;
	pPause = pause,
	pDuration = duration;
}

ceStrip::ceStrip( const ceStrip& strip ){
	pID = strip.GetID();
	pPause = strip.GetPause();
	pDuration = strip.GetDuration();
}

ceStrip::~ceStrip(){
}



// Management
///////////////

void ceStrip::SetID( const char *id ){
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	pID = id;
}

void ceStrip::SetPause( float pause ){
	pPause = pause;
}

void ceStrip::SetDuration( float duration ){
	pDuration = duration;
}



// Operators
//////////////

ceStrip &ceStrip::operator=( const ceStrip &strip ){
	pID = strip.GetID();
	pPause = strip.GetPause();
	pDuration = strip.GetDuration();
	return *this;
}

bool ceStrip::operator==( const ceStrip &strip ) const{
	return pID.Equals( strip.GetID() )
		&& fabsf( pPause - strip.GetPause() ) < FLOAT_SAFE_EPSILON
		&& fabsf( pDuration - strip.GetDuration() ) < FLOAT_SAFE_EPSILON;
}

bool ceStrip::operator!=( const ceStrip &strip ) const{
	return ! ( *this == strip );
}
