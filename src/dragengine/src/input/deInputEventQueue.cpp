/* 
 * Drag[en]gine Game Engine
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

#include <stdlib.h>

#include "deInputEvent.h"
#include "deInputEventQueue.h"
#include "../common/exceptions.h"



// Class deInputEventQueue
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deInputEventQueue::deInputEventQueue( int queueSize ){
	if( queueSize < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	pEvents = NULL;
	pEventCount = 0;
	pEventSize = 0;
	
	pEvents = new deInputEvent[ queueSize ];
	pEventSize = queueSize;
}

deInputEventQueue::~deInputEventQueue(){
	if( pEvents ){
		delete [] pEvents;
	}
}



// Management
///////////////

const deInputEvent &deInputEventQueue::GetEventAt( int index ) const{
	if( index < 0 || index >= pEventCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pEvents[ index ];
}

bool deInputEventQueue::AddEvent( const deInputEvent &event ){
	if( pEventCount == pEventSize ){
		return false;
	}
	
	pEvents[ pEventCount ].SetFrom( event );
	pEventCount++;
	return true;
}

void deInputEventQueue::RemoveAllEvents(){
	pEventCount = 0;
}
