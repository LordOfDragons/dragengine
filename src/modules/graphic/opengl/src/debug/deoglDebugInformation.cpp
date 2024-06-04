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
#include <string.h>
#include <stdlib.h>

#include "deoglDebugInformation.h"
#include "../deoglDebugFont.h"

#include <dragengine/common/exceptions.h>



// Class deoglDebugInformation
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDebugInformation::deoglDebugInformation( const char *name, const decColor &colorText, const decColor &colorBackground ) :
pName( name ),
pColorText( colorText ),
pColorBackground( colorBackground ),
pElapsedTime( 0.0f ),
pElapsedTimeAccum( 0.0f ),
pCounter( 0 ),
pHasElapsedTime( false ),
pHasCounter( false ),
pVisible( true ),
pLastElapsedTime( 0.0f ),
pSlidingElapsedTimeFactor( 0.05f ){
}

deoglDebugInformation::~deoglDebugInformation(){
}



// Management
///////////////

void deoglDebugInformation::SetName( const char *name ){
	pName = name;
}

void deoglDebugInformation::SetColorText( const decColor &color ){
	pColorText = color;
}

void deoglDebugInformation::SetColorBackground( const decColor &color ){
	pColorBackground = color;
}

void deoglDebugInformation::Clear(){
	ClearElapsedTime();
	ClearCounter();
}

void deoglDebugInformation::SetVisible( bool visible ){
	pVisible = visible;
}



void deoglDebugInformation::SetElapsedTime( float time ){
	pElapsedTimeAccum = time;
	pElapsedTime = decMath::mix( pLastElapsedTime, time, pSlidingElapsedTimeFactor );
	pHasElapsedTime = true;
}

void deoglDebugInformation::IncrementElapsedTime( float time ){
	pElapsedTimeAccum += time;
	pElapsedTime = decMath::mix( pLastElapsedTime, pElapsedTimeAccum, pSlidingElapsedTimeFactor );
	pHasElapsedTime = true;
}

void deoglDebugInformation::ClearElapsedTime(){
	if( ! pHasElapsedTime ){
		return;
	}
	
	pLastElapsedTime = pElapsedTime;
	pElapsedTimeAccum = 0.0f;
	pElapsedTime = 0.0f;
	pHasElapsedTime = false;
}

void deoglDebugInformation::SetSlidingElapsedTimeFactor( float factor ){
	pSlidingElapsedTimeFactor = decMath::clamp( factor, 0.0f, 1.0f );
}



void deoglDebugInformation::SetCounter( int counter ){
	pCounter = counter;
	pHasCounter = true;
}

void deoglDebugInformation::IncrementCounter(){
	pCounter++;
	pHasCounter = true;
}

void deoglDebugInformation::IncrementCounter( int count ){
	pCounter += count;
	pHasCounter = true;
}

void deoglDebugInformation::ClearCounter(){
	pCounter = 0;
	pHasCounter = false;
}

void deoglDebugInformation::CopyResults( const deoglDebugInformation &info ){
	pElapsedTime = info.pElapsedTime;
	pElapsedTimeAccum = info.pElapsedTimeAccum;
	pCounter = info.pCounter;
	pHasElapsedTime = info.pHasElapsedTime;
	pHasCounter = info.pHasCounter;
	pLastElapsedTime = info.pLastElapsedTime;
	pSlidingElapsedTimeFactor = info.pSlidingElapsedTimeFactor;
}



// Rendering
//////////////

void deoglDebugInformation::SetRenderText( const char* text ){
	pRenderText = text;
}

void deoglDebugInformation::SetRenderSize( const decPoint& size ){
	pRenderSize = size;
}

void deoglDebugInformation::SetRenderPosition( const decPoint& position ){
	pRenderPosition = position;
}
