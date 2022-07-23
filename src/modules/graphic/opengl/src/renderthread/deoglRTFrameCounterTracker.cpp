/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deoglRTFrameCounterTracker.h"
#include "deoglRenderThread.h"



// Class deoglRTFrameCounterTracker
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTFrameCounterTracker::deoglRTFrameCounterTracker( const deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pFrameCounter( 0 ),
pElapsedFrames( 0 ){
}

deoglRTFrameCounterTracker::~deoglRTFrameCounterTracker(){
}



// Management
///////////////

void deoglRTFrameCounterTracker::Update(){
	const uint32_t counter = pRenderThread.GetFrameCounter();
	
	if( counter == 0 ){
		pElapsedFrames = 0;
		
	}else{
		// this is safe against wrapping around
		pElapsedFrames = counter - pFrameCounter;
		pFrameCounter = counter;
	}
}
