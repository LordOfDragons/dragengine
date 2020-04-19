/* 
 * Drag[en]gine Bullet Physics Module
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

#include "debpDebugInformation.h"

#include <dragengine/common/exceptions.h>



// Class debpDebugInformation
///////////////////////////////

// Constructor, destructor
////////////////////////////

debpDebugInformation::debpDebugInformation( const char *name ) :
pName( name ),
pElapsedTime( 0.0f ),
pElapsedTimeAccum( 0.0f ),
pCounter( 0 ),
pHasElapsedTime( false ),
pHasCounter( false ){
}

debpDebugInformation::~debpDebugInformation(){
}



// Management
///////////////

void debpDebugInformation::SetName( const char *name ){
	pName = name;
}

void debpDebugInformation::Clear(){
	ClearElapsedTime();
	ClearCounter();
}



void debpDebugInformation::SetElapsedTime( float time ){
	pElapsedTimeAccum = time;
	pElapsedTime = time;
	pHasElapsedTime = true;
}

void debpDebugInformation::IncrementElapsedTime( float time ){
	pElapsedTimeAccum += time;
	pElapsedTime = pElapsedTimeAccum;
	pHasElapsedTime = true;
}

void debpDebugInformation::ClearElapsedTime(){
	if( ! pHasElapsedTime ){
		return;
	}
	
	pElapsedTimeAccum = 0.0f;
	pElapsedTime = 0.0f;
	pHasElapsedTime = false;
}



void debpDebugInformation::SetCounter( int counter ){
	pCounter = counter;
	pHasCounter = true;
}

void debpDebugInformation::IncrementCounter(){
	pCounter++;
	pHasCounter = true;
}

void debpDebugInformation::IncrementCounter( int count ){
	pCounter += count;
	pHasCounter = true;
}

void debpDebugInformation::ClearCounter(){
	pCounter = 0;
	pHasCounter = false;
}



void debpDebugInformation::SetText( const char* text ){
	pText = text;
}
