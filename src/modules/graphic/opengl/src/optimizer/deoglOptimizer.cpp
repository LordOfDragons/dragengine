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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deoglOptimizer.h"
#include "dragengine/common/exceptions.h"



// Class deoglOptimizer
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglOptimizer::deoglOptimizer(){
	SetPriority( epNormal );
	ResetRemainingTime();
}

deoglOptimizer::~deoglOptimizer(){
}



// Management
///////////////

void deoglOptimizer::DecreaseRemainingTime( int microSeconds ){
	pRemainingTime -= microSeconds;
}

void deoglOptimizer::ResetRemainingTime(){
	pRemainingTime = pMaxTime;
}

void deoglOptimizer::SetPriority( int priority ){
	if( priority == epLowest ){
		pMaxTime = 1000;
		
	}else if( priority == epLower ){
		pMaxTime = 5000;
		
	}else if( priority == epNormal ){
		pMaxTime = 10000;
		
	}else if( priority == epHigher ){
		pMaxTime = 50000;
		
	}else if( priority == epHighest ){
		pMaxTime = 100000;
		
	}else{
		DETHROW( deeInvalidParam );
	}
	
	pPriority = priority;
}

bool deoglOptimizer::Run( int timeSlice ){
	return false;
}
