/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
	printf( "[ObjectDebug] AddReference: object=%p logname=%s refcount=%d where=%s\n",
		this, pLogName, pRefCount, trace.GetBacktrace().GetAt( pLogTraceIndex ).GetString() );
	
	pRefCount++;
}

void deObjectDebug::FreeReference(){
	const deeInvalidAction trace( __FILE__, __LINE__ );
	printf( "[ObjectDebug] FreeReference: object=%p logname=%s refcount=%d where=%s\n",
		this, pLogName, pRefCount, trace.GetBacktrace().GetAt( pLogTraceIndex ).GetString() );
	
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
