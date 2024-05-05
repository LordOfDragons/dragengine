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

#include "deoalRayTraceHitElement.h"
#include "deoalRayTraceHitElementList.h"

#include <dragengine/common/exceptions.h>



// Class deoalRayTraceHitElementList
//////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalRayTraceHitElementList::deoalRayTraceHitElementList() :
pElements( NULL ),
pCount( 0 ),
pSize( 0 ){
}

deoalRayTraceHitElementList::~deoalRayTraceHitElementList(){
	if( pElements ){
		delete [] pElements;
	}
}



// Manegement
///////////////

const deoalRayTraceHitElement &deoalRayTraceHitElementList::GetAt( int index ) const{
	if( index < 0 || index >= pCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pElements[ index ];
}

void deoalRayTraceHitElementList::Add( const deoalRayTraceHitElement &element ){
	pBareAdd() = element;
}

void deoalRayTraceHitElementList::AddComponentFace( float distance, const decDVector &point,
const decDVector &normal, deoalAComponent *component, int face, bool frontFacing ){
	pBareAdd().SetComponentFace( distance, point, normal, component, face, frontFacing );
}

void deoalRayTraceHitElementList::RemoveAll(){
	pCount = 0;
}



// Private Functions
//////////////////////

deoalRayTraceHitElement &deoalRayTraceHitElementList::pBareAdd(){
	if( pCount == pSize ){
		const int newSize = pSize + 10;
		deoalRayTraceHitElement * const newArray = new deoalRayTraceHitElement[ newSize ];
		if( pElements ){
			memcpy( newArray, pElements, sizeof( deoalRayTraceHitElement ) * pSize );
			delete [] pElements;
		}
		pElements = newArray;
		pSize = newSize;
	}
	
	return pElements[ pCount++ ];
}
