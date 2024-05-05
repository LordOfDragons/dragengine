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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "deoglParticleEmitterInstanceList.h"
#include "deoglRParticleEmitterInstance.h"

#include <dragengine/common/exceptions.h>



// Class deoglParticleEmitterInstanceList
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglParticleEmitterInstanceList::deoglParticleEmitterInstanceList() :
pInstances( NULL ),
pInstanceCount( 0 ),
pInstanceSize( 0 ){
}

deoglParticleEmitterInstanceList::~deoglParticleEmitterInstanceList(){
	if( pInstances ){
		delete [] pInstances;
	}
}



// Management
///////////////

deoglRParticleEmitterInstance *deoglParticleEmitterInstanceList::GetAt( int index ) const{
	if( index < 0 || index >= pInstanceCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pInstances[ index ];
}

int deoglParticleEmitterInstanceList::IndexOf( deoglRParticleEmitterInstance *instance ) const{
	if( ! instance ){
		DETHROW( deeInvalidParam );
	}
	
	int l;
	
	for( l=0; l<pInstanceCount; l++ ){
		if( instance == pInstances[ l ] ){
			return l;
		}
	}
	
	return -1;
}

bool deoglParticleEmitterInstanceList::Has( deoglRParticleEmitterInstance *instance ) const{
	if( ! instance ){
		DETHROW( deeInvalidParam );
	}
	
	int l;
	
	for( l=0; l<pInstanceCount; l++ ){
		if( instance == pInstances[ l ] ){
			return true;
		}
	}
	
	return false;
}

void deoglParticleEmitterInstanceList::Add( deoglRParticleEmitterInstance *instance ){
	if( Has( instance ) ){
		DETHROW( deeInvalidParam );
	}
	
	pAddInstance( instance );
}

bool deoglParticleEmitterInstanceList::AddIfMissing( deoglRParticleEmitterInstance *instance ){
	if( Has( instance ) ){
		return false;
	}
	
	pAddInstance( instance );
	
	return true;
}

void deoglParticleEmitterInstanceList::Remove( deoglRParticleEmitterInstance *instance ){
	int index = IndexOf( instance );
	
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( index < pInstanceCount - 1 ){
		pInstances[ index ] = pInstances[ pInstanceCount - 1 ];
	}
	pInstanceCount--;
}

bool deoglParticleEmitterInstanceList::RemoveIfExisting( deoglRParticleEmitterInstance *instance ){
	int index = IndexOf( instance );
	
	if( index == -1 ){
		return false;
	}
	
	if( index < pInstanceCount - 1 ){
		pInstances[ index ] = pInstances[ pInstanceCount - 1 ];
	}
	pInstanceCount--;
	
	return true;
}

void deoglParticleEmitterInstanceList::RemoveFrom( int index ){
	if( index < 0 || index >= pInstanceCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( index < pInstanceCount - 1 ){
		pInstances[ index ] = pInstances[ pInstanceCount - 1 ];
	}
	pInstanceCount--;
}

void deoglParticleEmitterInstanceList::RemoveAll(){
	pInstanceCount = 0;
}



// Private Functions
//////////////////////

void deoglParticleEmitterInstanceList::pAddInstance( deoglRParticleEmitterInstance *instance ){
	if( pInstanceCount == pInstanceSize ){
		int newSize = pInstanceCount + 10; // * 3 / 2 + 1;
		deoglRParticleEmitterInstance **newArray = new deoglRParticleEmitterInstance*[ newSize ];
		
		if( pInstances ){
			memcpy( newArray, pInstances, sizeof( deoglRParticleEmitterInstance* ) * pInstanceSize );
			delete [] pInstances;
		}
		
		pInstances = newArray;
		pInstanceSize = newSize;
	}
	
	pInstances[ pInstanceCount++ ] = instance;
}
