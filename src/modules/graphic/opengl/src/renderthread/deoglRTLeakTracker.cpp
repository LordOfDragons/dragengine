/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "../deoglBasics.h"

#include "deoglRTLeakTracker.h"
#include "deoglRenderThread.h"
#include "deoglRTLogger.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/deObject.h>



// Class deoglRTLeakTracker
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTLeakTracker::deoglRTLeakTracker( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ){
}

deoglRTLeakTracker::~deoglRTLeakTracker(){
}



// Management
///////////////

void deoglRTLeakTracker::ReportLeaks( const char *name, const decPointerSet &tracker ){
	pMutex.Lock();
	
	if( tracker.GetCount() > 0 ){
		try{
			deoglRTLogger &logger = pRenderThread.GetLogger();
			const int count = tracker.GetCount();
			const int entriesPerLine = 8; //10
			decString line;
			int i;
			
			logger.LogInfoFormat( "%i Leaks for %s", count, name );
			for( i=0; i<count; i++ ){
				line.AppendFormat( " %p(%i)", tracker.GetAt( i ), ( ( deObject* )tracker.GetAt( i ) )->GetRefCount() );
				if( ( ( i + 1 ) % entriesPerLine ) == 0 ){
					logger.LogInfo( line );
					line.Empty();
				}
			}
			
			if( ! line.IsEmpty() ){
				logger.LogInfo( line );
			}
			
		}catch( const deException & ){
			pMutex.Unlock();
			throw;
		}
	}
	
	pMutex.Unlock();
}



#ifdef ENABLE_LEAK_TRACKING
void deoglRTLeakTracker::AddTracked( decPointerSet &list, void *object ){
	pMutex.Lock();
	
	try{
		list.Add( object );
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
	
	pMutex.Unlock();
}

void deoglRTLeakTracker::RemoveTracked( decPointerSet &list, void *object ) {
	pMutex.Lock();
	
	try{
		list.Remove( object );
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
	
	pMutex.Unlock();
}
#endif
