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

#ifdef IGDE_TOOLKIT_FOX

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "igdeNativeFoxTimer.h"
#include "../../igdeTimer.h"

#include <dragengine/common/exceptions.h>



// Event map
//////////////

FXDEFMAP( igdeNativeFoxTimer ) igdeNativeFoxTimerMap[] = {
	FXMAPFUNC( SEL_TIMEOUT, igdeNativeFoxTimer::ID_TIMEOUT, igdeNativeFoxTimer::onTimeout ),
};


FXIMPLEMENT( igdeNativeFoxTimer, FXObject, igdeNativeFoxTimerMap, ARRAYNUMBER( igdeNativeFoxTimerMap ) )



// Class igdeNativeFoxTimer
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeFoxTimer::igdeNativeFoxTimer(){
}

igdeNativeFoxTimer::igdeNativeFoxTimer( igdeTimer &owner, FXApp *app ) :
pApp( app ),
pOwner( &owner ){
}

igdeNativeFoxTimer::~igdeNativeFoxTimer(){
}

igdeNativeFoxTimer *igdeNativeFoxTimer::CreateNativeTimer( igdeTimer &owner ){
	return new igdeNativeFoxTimer( owner, FXApp::instance() );
}

void igdeNativeFoxTimer::DestroyNativeTimer(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxTimer::StartTimer(){
	pApp->addTimeout( this, ID_TIMEOUT, pOwner->GetTimeout() * 1000000 );
}

void igdeNativeFoxTimer::StopTimer(){
	pApp->removeTimeout( this, ID_TIMEOUT );
}



// Events
///////////

long igdeNativeFoxTimer::onTimeout( FXObject*, FXSelector, void* ){
	if( ! pOwner->GetRunning() ){
		// just in case FOX manages to send an event although the user stopped the timer
		return 1;
	}
	
	if( pOwner->GetRepeating() ){
		// FOX timers run only once
		StartTimer();
		
	}else{
		// this does call StopTimer() but this is not a problem
		pOwner->Stop();
	}
	
	pOwner->OnTimeout();
	return 1;
}

#endif
