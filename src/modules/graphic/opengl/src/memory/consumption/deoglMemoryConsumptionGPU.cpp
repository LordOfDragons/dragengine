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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglMemoryConsumptionGPU.h"

#include <dragengine/common/exceptions.h>



// Class deoglMemoryConsumptionGPU
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglMemoryConsumptionGPU::deoglMemoryConsumptionGPU( const char *name ) :
pName( name ),
pCount( 0 ),
pConsumption( 0 ),
pGroup( nullptr ),
pGroup2( nullptr ){
}

deoglMemoryConsumptionGPU::deoglMemoryConsumptionGPU( const char *name, deoglMemoryConsumptionGPU &group ) :
pName( name ),
pCount( 0 ),
pConsumption( 0 ),
pGroup( &group ),
pGroup2( nullptr ){
}

deoglMemoryConsumptionGPU::deoglMemoryConsumptionGPU( const char *name,
	deoglMemoryConsumptionGPU &group, deoglMemoryConsumptionGPU &group2 ) :
pName( name ),
pCount( 0 ),
pConsumption( 0 ),
pGroup( &group ),
pGroup2( &group2 ){
}

deoglMemoryConsumptionGPU::~deoglMemoryConsumptionGPU(){
}



// Management
///////////////

void deoglMemoryConsumptionGPU::IncrementCount(){
	pCount++;
	
	if( pGroup ){
		pGroup->IncrementCount();
	}
	if( pGroup2 ){
		pGroup2->IncrementCount();
	}
}

void deoglMemoryConsumptionGPU::DecrementCount(){
	if( pGroup2 ){
		pGroup2->DecrementCount();
	}
	if( pGroup ){
		pGroup->DecrementCount();
	}
	
	pCount--;
}

void deoglMemoryConsumptionGPU::IncrementConsumption( unsigned long long bytes ){
	pConsumption += bytes;
	
	if( pGroup ){
		pGroup->IncrementConsumption( bytes );
	}
	if( pGroup2 ){
		pGroup2->IncrementConsumption( bytes );
	}
}

void deoglMemoryConsumptionGPU::DecrementConsumption( unsigned long long bytes ){
	if( pGroup2 ){
		pGroup2->DecrementConsumption( bytes );
	}
	if( pGroup ){
		pGroup->DecrementConsumption( bytes );
	}
	
	pConsumption -= bytes;
}
