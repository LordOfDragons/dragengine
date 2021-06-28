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
