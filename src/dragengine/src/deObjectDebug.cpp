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

#include <stdio.h>
#include <stdlib.h>

#include "deObjectDebug.h"
#include "common/exceptions.h"



// Class deObjectDebug
////////////////////////

// Constructor, destructor
////////////////////////////

deObjectDebug::deObjectDebug( const char *logname, int logTraceIndex ) :
pRefCount( 1 ),
pLogName( logname ),
pLogTraceIndex( logTraceIndex ){
}

deObjectDebug::~deObjectDebug(){
}



// Management
///////////////

void deObjectDebug::AddReference(){
	const deeInvalidAction trace( __FILE__, __LINE__ );
	printf( "[ObjectDebug] AddReference: logname=%s refcount=%d where=%s\n",
		pLogName, pRefCount, trace.GetBacktrace().GetAt( pLogTraceIndex ).GetString() );
	
	pRefCount++;
}

void deObjectDebug::FreeReference(){
	const deeInvalidAction trace( __FILE__, __LINE__ );
	printf( "[ObjectDebug] FreeReference: logname=%s refcount=%d where=%s\n",
		pLogName, pRefCount, trace.GetBacktrace().GetAt( pLogTraceIndex ).GetString() );
	
	pRefCount--;
	if( pRefCount > 0 ){
		return;
	}
	
	if( pRefCount < 0 ){
		deeInvalidParam( __FILE__, __LINE__ ).PrintError();
		return;
	}
	
	delete this;
}
