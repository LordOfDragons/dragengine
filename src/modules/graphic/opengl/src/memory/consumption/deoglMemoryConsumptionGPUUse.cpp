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

#include "deoglMemoryConsumptionGPUUse.h"

#include "deoglMemoryConsumptionGPU.h"


// Class deoglMemoryConsumptionGPUUse
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglMemoryConsumptionGPUUse::deoglMemoryConsumptionGPUUse( deoglMemoryConsumptionGPU &tracker ) :
pTracker( tracker ),
pConsumption( 0 ){
}

deoglMemoryConsumptionGPUUse::~deoglMemoryConsumptionGPUUse(){
	SetConsumption( 0 );
}



// Management
///////////////

void deoglMemoryConsumptionGPUUse::SetConsumption( unsigned long long consumption ){
	if( consumption > pConsumption ){
		IncrementConsumption( consumption - pConsumption );
		
	}else if( consumption < pConsumption ){
		DecrementConsumption( pConsumption - consumption );
	}
}

void deoglMemoryConsumptionGPUUse::IncrementConsumption( unsigned long long consumption ){
	if( consumption == 0 ){
		return;
	}
	
	pTracker.IncrementConsumption( consumption );
	
	if( pConsumption == 0 ){
		pTracker.IncrementCount();
	}
	
	pConsumption += consumption;
}

void deoglMemoryConsumptionGPUUse::DecrementConsumption( unsigned long long consumption ){
	if( consumption == 0 ){
		return;
	}
	
	if( consumption < pConsumption ){
		pTracker.DecrementConsumption( consumption );
		pConsumption -= consumption;
		
	}else{
		pTracker.DecrementConsumption( pConsumption );
		pTracker.DecrementCount();
		pConsumption = 0;
	}
}



// Operators
//////////////

deoglMemoryConsumptionGPUUse &deoglMemoryConsumptionGPUUse::operator=( unsigned long long consumption ){
	SetConsumption( consumption );
	return *this;
}

deoglMemoryConsumptionGPUUse &deoglMemoryConsumptionGPUUse::operator+=( unsigned long long consumption ){
	IncrementConsumption( consumption );
	return *this;
}

deoglMemoryConsumptionGPUUse &deoglMemoryConsumptionGPUUse::operator-=( unsigned long long consumption ){
	DecrementConsumption( consumption );
	return *this;
}
