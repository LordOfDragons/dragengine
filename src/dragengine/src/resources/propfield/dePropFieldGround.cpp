/* 
 * Drag[en]gine Game Engine
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
#include <stdlib.h>
#include <string.h>

#include "dePropFieldGround.h"
#include "../component/deComponent.h"
#include "../terrain/heightmap/deHeightTerrain.h"
#include "../../common/exceptions.h"



// Class dePropFieldGround
////////////////////////////

// Constructor, destructor
////////////////////////////

dePropFieldGround::dePropFieldGround(){
	pHeightTerrain = NULL;
	
	pComponents = NULL;
	pComponentCount = 0;
	pComponentSize = 0;
}

dePropFieldGround::~dePropFieldGround(){
	RemoveAllComponents();
	if( pComponents ) delete [] pComponents;
	
	if( pHeightTerrain ) pHeightTerrain->FreeReference();
}



// Management
///////////////

void dePropFieldGround::Clear(){
	RemoveAllComponents();
	
	if( pHeightTerrain ){
		pHeightTerrain->FreeReference();
		pHeightTerrain = NULL;
	}
}

void dePropFieldGround::SetHeightTerrain( deHeightTerrain *heightTerrain ){
	if( heightTerrain != pHeightTerrain ){
		if( pHeightTerrain ) pHeightTerrain->FreeReference();
		pHeightTerrain = heightTerrain;
		if( heightTerrain ) heightTerrain->AddReference();
	}
}



// Components
//////////////

deComponent *dePropFieldGround::GetComponentAt( int index ) const{
	if( index < 0 || index >= pComponentCount ) DETHROW( deeOutOfBoundary );
	
	return pComponents[ index ];
}

int dePropFieldGround::IndexOfComponent( deComponent *component ) const{
	if( ! component ) DETHROW( deeInvalidParam );
	
	int t;
	
	for( t=0; t<pComponentCount; t++ ){
		if( component == pComponents[ t ] ){
			return t;
		}
	}
	
	return -1;
}

bool dePropFieldGround::HasComponent( deComponent *component ) const{
	if( ! component ) DETHROW( deeInvalidParam );
	
	int t;
	
	for( t=0; t<pComponentCount; t++ ){
		if( component == pComponents[ t ] ){
			return true;
		}
	}
	
	return false;
}

void dePropFieldGround::AddComponent( deComponent *component ){
	if( ! component ) DETHROW( deeInvalidParam );
	
	if( pComponentCount == pComponentSize ){
		int newSize = pComponentSize * 3 / 2 + 1;
		deComponent **newArray = new deComponent*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pComponents ){
			memcpy( newArray, pComponents, sizeof( deComponent* ) * pComponentSize );
			delete [] pComponents;
		}
		pComponents = newArray;
		pComponentSize = newSize;
	}
	
	pComponents[ pComponentCount ] = component;
	component->AddReference();
	pComponentCount++;
}

void dePropFieldGround::RemoveComponent( deComponent *component ){
	int i, index = IndexOfComponent( component );
	
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	for( i=index+1; i<pComponentCount; i++ ){
		pComponents[ i - 1 ] = pComponents[ i ];
	}
	pComponentCount--;
	
	component->FreeReference();
}

void dePropFieldGround::RemoveAllComponents(){
	while( pComponentCount > 0 ){
		pComponentCount--;
		pComponents[ pComponentCount ]->FreeReference();
	}
}
