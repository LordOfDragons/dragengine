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

#include "native/toolkit.h"
#include "igdeTimer.h"
#include "native/fox/igdeNativeFoxTimer.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class igdeTimer
////////////////////

// Constructor, destructor
////////////////////////////

igdeTimer::igdeTimer( igdeEnvironment &environment ) :
pEnvironment( environment ),
pNativeTimer( NULL ),
pTimeout( 0 ),
pRepeating( false ),
pRunning( false )
{
	pNativeTimer = new igdeNativeFoxTimer( *this, FXApp::instance() );
}

igdeTimer::~igdeTimer(){
	Stop();
	
	if( pNativeTimer ){
		delete ( igdeNativeFoxTimer* )pNativeTimer;
	}
}



// Management
///////////////

void igdeTimer::Start( int timeoutMS, bool repeating ){
	Stop();
	
	pTimeout = decMath::max( timeoutMS, 0 );
	pRepeating = repeating;
	pRunning = true;
	
	( ( igdeNativeFoxTimer* )pNativeTimer )->StartTimer();
}

void igdeTimer::Start( double timeout, bool repeating ){
	Start( ( int )( timeout * 1000.0 + 0.5 ), repeating );
}

void igdeTimer::Stop(){
	if( pRunning ){
		( ( igdeNativeFoxTimer* )pNativeTimer )->StopTimer();
		pRunning = false;
	}
}

void igdeTimer::OnTimeout(){
}



// Protected Functions
////////////////////////

void igdeTimer::SetNativeTimer( void *nativeTimer ){
	pNativeTimer = nativeTimer;
}
