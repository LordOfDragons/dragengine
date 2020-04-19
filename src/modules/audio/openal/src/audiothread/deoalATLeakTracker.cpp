/* 
 * Drag[en]gine OpenAL Audio Module
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

#include "../deoalBasics.h"

#include "deoalATLeakTracker.h"
#include "deoalAudioThread.h"
#include "deoalATLogger.h"

#include <dragengine/deObject.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/threading/deMutexGuard.h>



// Class deoalATLeakTracker
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoalATLeakTracker::deoalATLeakTracker( deoalAudioThread &audioThread ) :
pAudioThread( audioThread ){
}

deoalATLeakTracker::~deoalATLeakTracker(){
}



// Management
///////////////

void deoalATLeakTracker::ReportLeaks( const char *name, const decPointerSet &tracker ){
	deMutexGuard lock( pMutex );
	
	if( tracker.GetCount() == 0 ){
		return;
	}
	
	deoalATLogger &logger = pAudioThread.GetLogger();
	const int count = tracker.GetCount();
	const int entriesPerLine = 8; //10
	decString line;
	int i;
	
	logger.LogInfoFormat( "%i Leaks for %s", count, name );
	for( i=0; i<count; i++ ){
		line.AppendFormat( " %p(%i)", tracker.GetAt( i ),
			( ( deObject* )tracker.GetAt( i ) )->GetRefCount() );
		
		if( ( ( i + 1 ) % entriesPerLine ) == 0 ){
			logger.LogInfo( line );
			line.Empty();
		}
	}
	
	if( ! line.IsEmpty() ){
		logger.LogInfo( line );
	}
}



#ifdef ENABLE_LEAK_TRACKING
void deoalATLeakTracker::AddTracked( decPointerSet &list, void *object ){
	deMutexGuard lock( pMutex );
	list.Add( object );
}

void deoalATLeakTracker::RemoveTracked( decPointerSet &list, void *object ) {
	deMutexGuard lock( pMutex );
	list.Remove( object );
}
#endif
