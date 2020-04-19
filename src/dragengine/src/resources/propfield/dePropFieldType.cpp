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

#include "dePropFieldType.h"
#include "dePropFieldInstance.h"
#include "dePropFieldBendState.h"
#include "../model/deModel.h"
#include "../skin/deSkin.h"
#include "../../common/exceptions.h"



// Class dePropFieldType
//////////////////////////

// Constructor, destructor
////////////////////////////

dePropFieldType::dePropFieldType(){
	pRotationPerForce = 5.0f;
	pRestitution = 0.5f;
	
	pInstances = NULL;
	pInstanceCount = 0;
	
	pBendStates = NULL;
	pBendStateCount = 0;
}

dePropFieldType::~dePropFieldType(){
	if( pBendStates ) delete [] pBendStates;
	if( pInstances ) delete [] pInstances;
}



// Management
///////////////

void dePropFieldType::SetModel( deModel *model ){
	pModel = model;
}

void dePropFieldType::SetSkin( deSkin *skin ){
	pSkin = skin;
}

void dePropFieldType::SetRotationPerForce( float rotation ){
	if( rotation < 0.0f ) rotation = 0.0f;
	
	pRotationPerForce = rotation;
}

void dePropFieldType::SetRestitution( float restitution ){
	if( restitution < 0.0f ) restitution = 0.0f;
	
	pRestitution = restitution;
}

void dePropFieldType::SetCollisionFilter( const decCollisionFilter &collisionFilter ){
	pCollisionFilter = collisionFilter;
}

void dePropFieldType::SetInstanceCount( int count ){
	if( count < 0 ) DETHROW( deeInvalidParam );
	
	dePropFieldInstance *instances = NULL;
	
	if( count > 0 ){
		instances = new dePropFieldInstance[ count ];
		if( ! instances ) DETHROW( deeOutOfMemory );
	}
	
	if( pInstances ) delete [] pInstances;
	pInstances = instances;
	pInstanceCount = count;
}

dePropFieldInstance &dePropFieldType::GetInstanceAt( int index ) const{
	if( index < 0 || index >= pInstanceCount ) DETHROW( deeInvalidParam );
	
	return pInstances[ index ];
}



void dePropFieldType::SetBendStateCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count == pBendStateCount ){
		return;
	}
	
	dePropFieldBendState *bendStates = NULL;
	
	if( count > 0 ){
		bendStates = new dePropFieldBendState[ count ];
	}
	
	if( pBendStates ){
		delete [] pBendStates;
	}
	pBendStates = bendStates;
	pBendStateCount = count;
}

dePropFieldBendState &dePropFieldType::GetBendStateAt( int index ) const{
	if( index < 0 || index >= pBendStateCount ) DETHROW( deeInvalidParam );
	
	return pBendStates[ index ];
}
