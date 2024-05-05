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
