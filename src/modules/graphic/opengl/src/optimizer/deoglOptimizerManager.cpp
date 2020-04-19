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
#include "deoglOptimizerManager.h"
#include "dragengine/common/utils/decTimer.h"
#include "dragengine/common/exceptions.h"



// Class deoglOptimizerManager
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglOptimizerManager::deoglOptimizerManager(){
	pOptimizers = NULL;
	pOptimizerCount = 0;
	pOptimizerSize = 0;
	pNextOptimizer = 0;
}

deoglOptimizerManager::~deoglOptimizerManager(){
	pCleanUp();
}



// Optimizer Management
/////////////////////////

deoglOptimizer *deoglOptimizerManager::GetOptimizerAt( int index ) const{
	if( index < 0 || index >= pOptimizerCount ) DETHROW( deeInvalidParam );
	return pOptimizers[ index ];
}

bool deoglOptimizerManager::HasOptimizer( deoglOptimizer *optimizer ) const{
	if( ! optimizer ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pOptimizerCount; i++ ){
		if( optimizer == pOptimizers[ i ] ) return true;
	}
	
	return false;
}

int deoglOptimizerManager::IndexOfOptimizer( deoglOptimizer *optimizer ) const{
	if( ! optimizer ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pOptimizerCount; i++ ){
		if( optimizer == pOptimizers[ i ] ) return i;
	}
	
	return -1;
}

void deoglOptimizerManager::AddOptimizer( deoglOptimizer *optimizer ){
	if( HasOptimizer( optimizer ) ) DETHROW( deeInvalidParam );
	
	if( pOptimizerCount == pOptimizerSize ){
		int newSize = pOptimizerSize * 3 / 2 + 1;
		deoglOptimizer **newArray = new deoglOptimizer*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pOptimizers ){
			memcpy( newArray, pOptimizers, sizeof( deoglOptimizer* ) * pOptimizerSize );
			delete [] pOptimizers;
		}
		pOptimizers = newArray;
		pOptimizerSize = newSize;
	}
	
	pOptimizers[ pOptimizerCount ] = optimizer;
	pOptimizerCount++;
	
	optimizer->ResetRemainingTime();
}

void deoglOptimizerManager::RemoveOptimizer( deoglOptimizer *optimizer ){
	int i, index = IndexOfOptimizer( optimizer );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	for( i=index+1; i<pOptimizerCount; i++ ){
		pOptimizers[ i - 1 ] = pOptimizers[ i ];
	}
	pOptimizerCount--;
	
	if( pNextOptimizer > index ) pNextOptimizer--;
	
	delete optimizer;
}

void deoglOptimizerManager::RemoveAllOptimizers(){
	while( pOptimizerCount > 0 ){
		pOptimizerCount--;
		delete pOptimizers[ pOptimizerCount ];
	}
}



// Management
///////////////

void deoglOptimizerManager::Run( int timeSlice ){
	int i, grantedTime, runOptimizers = 0;
	decTimer timer;
	
	// run optmizers until the time runs out
	while( timeSlice > 0 && runOptimizers < pOptimizerCount ){
		// run the next optimizer
		deoglOptimizer &optimizer = *pOptimizers[ pNextOptimizer ];
		grantedTime = optimizer.GetRemainingTime();
		if( grantedTime > timeSlice ){
			grantedTime = timeSlice;
		}
		
		if( optimizer.Run( grantedTime ) ){
			optimizer.DecreaseRemainingTime( grantedTime );
			if( optimizer.GetRemainingTime() <= 0 ){
				optimizer.ResetRemainingTime();
				pNextOptimizer++;
				runOptimizers++;
			}
			
		}else{
			delete pOptimizers[ pNextOptimizer ];
			
			for( i=pNextOptimizer+1; i<pOptimizerCount; i++ ){
				pOptimizers[ i - 1 ] = pOptimizers[ i ];
			}
			pOptimizerCount--;
		}
		
		if( pNextOptimizer == pOptimizerCount ) pNextOptimizer = 0;
		
		// adjust the remaining time
		timeSlice -= ( int )( timer.GetElapsedTime() * 1000000 );
	}
}




// Private Functions
//////////////////////

void deoglOptimizerManager::pCleanUp(){
	RemoveAllOptimizers();
	if( pOptimizers ) delete [] pOptimizers;
}
