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
#include <string.h>

#include "igdeTimer.h"
#include "native/toolkit.h"
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
	pNativeTimer = igdeNativeTimer::CreateNativeTimer( *this );
}

igdeTimer::~igdeTimer(){
	Stop();
	
	if( pNativeTimer ){
		( ( igdeNativeTimer* )pNativeTimer )->DestroyNativeTimer();
	}
}



// Management
///////////////

void igdeTimer::Start( int timeoutMS, bool repeating ){
	Stop();
	
	pTimeout = decMath::max( timeoutMS, 0 );
	pRepeating = repeating;
	pRunning = true;
	
	( ( igdeNativeTimer* )pNativeTimer )->StartTimer();
}

void igdeTimer::Start( double timeout, bool repeating ){
	Start( ( int )( timeout * 1000.0 + 0.5 ), repeating );
}

void igdeTimer::Stop(){
	if( pRunning ){
		( ( igdeNativeTimer* )pNativeTimer )->StopTimer();
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
