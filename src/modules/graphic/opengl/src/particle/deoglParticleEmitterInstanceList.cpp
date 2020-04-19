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
